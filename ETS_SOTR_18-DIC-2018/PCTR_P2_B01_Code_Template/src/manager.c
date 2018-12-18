/*
====================================================================
Concurrent and Real-Time Programming
Faculty of Computer Science
University of Castilla-La Mancha (Spain)

Contact info: http://www.libropctr.com

You can redistribute and/or modify this file under the terms of the
GNU General Public License ad published by the Free Software
Foundation, either version 3 of the License, or (at your option) and
later version. See <http://www.gnu.org/licenses/>.

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
====================================================================
*/

#define _POSIX_SOURCE
#define _BSD_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <definitions.h>
#include <semaphoreI.h>

/* Total number of processes */
int g_nProcesses;
/* 'Process table' (child processes) */
struct TProcess_t *g_process_table;

/* Process management */
void create_processes_by_class(enum ProcessClass_t class, int n_processes, int index_process_table);
pid_t create_single_process(const char *class, const char *path, const char *argv);
void get_str_process_info(enum ProcessClass_t class, char **path, char **str_process_class);
void init_process_table(int n_decoders, int n_symbol_decoders);
void terminate_processes();

/* Semaphores and shared memory management */
void create_shm_segments(int *shm_data, int *shm_task, int *shm_symbol,
			 struct TData_t **p_data, struct TTask_t **p_task, 
			 char *encoded_input_data, int *n_input_data);
void create_sems(sem_t **p_sem_task_ready, sem_t **p_sem_task_read, sem_t **p_sem_task_processed);
void close_shared_memory_segments(int shm_data, int shm_task, int shm_symbol);

/* Task management */
void notify_tasks(sem_t *sem_task_ready, sem_t *sem_task_read, struct TTask_t *task,
		  int max_task_size, int *n_tasks, int n_input_data);
void wait_tasks_termination(sem_t *sem_task_processed, int n_tasks);

/* Auxiliar functions */
void free_resources();
void install_signal_handler();
void parse_argv(int argc, char *argv[], char **p_encoded_input_data,
		int *n_decoders, int *max_task_size);
void print_result(struct TData_t *data, int n_input_data);
void signal_handler(int signo);

/******************** Main function ********************/

int main(int argc, char *argv[]) {
  struct TData_t *data;
  struct TTask_t *task;
  int shm_data, shm_task, shm_symbol;
  sem_t *sem_task_ready, *sem_task_read, *sem_task_processed;

  char *encoded_input_data;
  int n_decoders, max_task_size, n_input_data, n_tasks;

  /* Install signal handler and parse arguments*/
  install_signal_handler();
  parse_argv(argc, argv, &encoded_input_data, &n_decoders, &max_task_size);

  /* Init the process table*/
  init_process_table(n_decoders, NUM_SYMBOL_DECODERS);

  /* Create shared memory segments and semaphores */
  create_shm_segments(&shm_data, &shm_task, &shm_symbol,
		      &data, &task, encoded_input_data, &n_input_data);
  create_sems(&sem_task_ready, &sem_task_read, &sem_task_processed);

  /* Create processes */
  create_processes_by_class(DECODER, n_decoders, 0);
  create_processes_by_class(SYMBOL_DECODER, NUM_SYMBOL_DECODERS, n_decoders);

  /* Manage tasks */
  notify_tasks(sem_task_ready, sem_task_read, task, max_task_size, &n_tasks,n_input_data);
  wait_tasks_termination(sem_task_processed, n_tasks);

  /* Print the decoded text */
  print_result (data, n_input_data);

  /* Free resources and terminate */
  close_shared_memory_segments(shm_data, shm_task, shm_symbol);
  terminate_processes();
  free_resources();

  return EXIT_SUCCESS;
}

/******************** Process Management ********************/

void create_processes_by_class(enum ProcessClass_t class, int n_processes, int index_process_table) {
  char *path = NULL, *str_process_class = NULL;
  int i;
  pid_t pid;

  get_str_process_info(class, &path, &str_process_class);

  for (i = index_process_table; i < (index_process_table + n_processes); i++) {
    pid = create_single_process(path, str_process_class, NULL);

    g_process_table[i].class = class;
    g_process_table[i].pid = pid;
    g_process_table[i].str_process_class = str_process_class;
  }

  printf("[MANAGER] %d %s processes created.\n", n_processes, str_process_class);
  sleep(1);
}

pid_t create_single_process(const char *path, const char *class, const char *argv) {
  pid_t pid;

  switch (pid = fork()) {
  case -1 :
    fprintf(stderr, "[MANAGER] Error creating %s process: %s.\n", 
	    class, strerror(errno));
    terminate_processes();
    free_resources();
    exit(EXIT_FAILURE);
    /* Child process */
  case 0 : 
    if (execl(path, class, argv, NULL) == -1) {
      fprintf(stderr, "[MANAGER] Error using execl() in %s process: %s.\n", 
	      class, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  /* Child PID */
  return pid;
}

void get_str_process_info(enum ProcessClass_t class, char **path, char **str_process_class) {
  switch (class) {
  case DECODER:
    *path = DECODER_PATH;
    *str_process_class = DECODER_CLASS;
    break;
  case SYMBOL_DECODER:
    *path = SYMBOL_DECODER_PATH;
    *str_process_class = SYMBOL_DECODER_CLASS;
    break;
  }
}

void init_process_table(int n_decoders, int n_symbol_decoders) {
  int i;

  /* Number of processes to be created */
  g_nProcesses = n_decoders + n_symbol_decoders; 
  /* Allocate memory for the 'process table' */
  g_process_table = malloc(g_nProcesses * sizeof(struct TProcess_t)); 

  /* Init the 'process table' */
  for (i = 0; i < g_nProcesses; i++) {
    g_process_table[i].pid = 0;
  }
}

void terminate_processes() {
  int i;
  
  printf("\n----- [MANAGER] Terminating running child processes ----- \n");
  for (i = 0; i < g_nProcesses; i++) {
    /* Child process alive */
    if (g_process_table[i].pid != 0) { 
      printf("[MANAGER] Terminating %s process [%d]...\n", 
	     g_process_table[i].str_process_class, g_process_table[i].pid);
      if (kill(g_process_table[i].pid, SIGINT) == -1) {
	fprintf(stderr, "[MANAGER] Error using kill() on process %d: %s.\n", 
		g_process_table[i].pid, strerror(errno));
      }
    }
  }
}

/******************** Semaphores and shared memory management ********************/

void create_shm_segments(int *shm_data, int *shm_task, int *shm_symbol,
			 struct TData_t **p_data, struct TTask_t **p_task, 
			 char *encoded_input_data, int *n_input_data) {
  int i = 0;
  char *encoded_character;

  /* Create and initialize shared memory segments */
  *shm_data = shm_open(SHM_DATA, O_CREAT | O_RDWR, 0644); 
  ftruncate(*shm_data, sizeof(struct TData_t));          
  *p_data = mmap(NULL, sizeof(struct TData_t), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_data, 0);

  *shm_task = shm_open(SHM_TASK, O_CREAT | O_RDWR, 0644);
  ftruncate(*shm_task, sizeof(struct TTask_t));
  *p_task = mmap(NULL, sizeof(struct TTask_t), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_task, 0);

  *shm_symbol = shm_open(SHM_SYMBOL, O_CREAT | O_RDWR, 0644);
  ftruncate(*shm_symbol, sizeof(char));
  /* No need to map shm_symbol since the manager process does not use it */

  /* Load encoded data */
  (*p_data)->vector[i++] = atoi(strtok(encoded_input_data, SEPARATOR));
  while ((encoded_character = strtok(NULL, SEPARATOR)) != NULL) {
    (*p_data)->vector[i++] = atoi(encoded_character);
  }
  *n_input_data = i;
}

void create_sems(sem_t **p_sem_task_ready, sem_t **p_sem_task_read, sem_t **p_sem_task_processed) {  
  /* Create and initialize semaphores */
  *p_sem_task_ready = create_semaphore(SEM_TASK_READY, /* SEM INIT VALUE */); 
  *p_sem_task_read = create_semaphore(SEM_TASK_READ, /* SEM INIT VALUE */);
  *p_sem_task_processed = create_semaphore(SEM_TASK_PROCESSED, /* SEM INIT VALUE */);
  /* The manager process only initializes the rest, but it does not use them */
  create_semaphore(SEM_MUTEX, /* SEM INIT VALUE */);
  create_semaphore(SEM_SYMBOL_READY, /* SEM INIT VALUE */);
  create_semaphore(SEM_SYMBOL_DECODED, /* SEM INIT VALUE */);
}

void close_shared_memory_segments(int shm_data, int shm_task, int shm_symbol) {
  close(shm_data);
  close(shm_task);
  close(shm_symbol);
}

/******************** Task management ********************/

void notify_tasks(sem_t *sem_task_ready, sem_t *sem_task_read, struct TTask_t *task,
		  int max_task_size, int *n_tasks, int n_input_data) {
  int current_task = 0;

  /* Number of subvectors */
  *n_tasks = ceil(n_input_data / (float)max_task_size);
  
  while (current_task < *n_tasks) {
    /* Compute the task info */
    task->begin = current_task * max_task_size;
    task->end = task->begin + max_task_size - 1;
    if (task->end > (n_input_data - 1)) {
      task->end = n_input_data - 1;
    }    
    current_task++;

    /* Task notification through rendezvous */
    signal_semaphore(sem_task_ready);
    wait_semaphore(sem_task_read);
  }
}

void wait_tasks_termination(sem_t *sem_task_processed, int n_tasks) {
  int n_tasks_processed = 0;

  while (n_tasks_processed < n_tasks) {
    wait_semaphore(sem_task_processed);                         
    n_tasks_processed++;   
  }
}

/******************** Auxiliar functions ********************/

void free_resources() {
  printf("\n----- [MANAGER] Freeing resources ----- \n");

  /* Free the 'process table' memory */
  free(g_process_table); 

  /* Semaphores */ 
  remove_semaphore(SEM_TASK_READY);
  remove_semaphore(SEM_TASK_READ);
  remove_semaphore(SEM_TASK_PROCESSED);
  remove_semaphore(SEM_MUTEX);
  remove_semaphore(SEM_SYMBOL_READY);
  remove_semaphore(SEM_SYMBOL_DECODED);

  /* Shared memory segments*/
  shm_unlink(SHM_TASK);
  shm_unlink(SHM_DATA);
  shm_unlink(SHM_SYMBOL);
}

void install_signal_handler() {
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    fprintf(stderr, "[MANAGER] Error installing signal handler: %s.\n", strerror(errno));    
    exit(EXIT_FAILURE);
  }
}

void parse_argv(int argc, char *argv[], char **p_encoded_input_data,
		int *n_decoders, int *max_task_size) {
  if (argc != 4) {
    fprintf(stderr, "Synopsis: ./exec/manager <data> <n_decoders> <max_task_size>.\n");    
    exit(EXIT_FAILURE); 
  }
  
  *p_encoded_input_data = argv[1];
  *n_decoders = atoi(argv[2]);  
  *max_task_size = atoi(argv[3]);
}

void print_result(struct TData_t *data, int n_input_data) {
  int i;

  printf("\n----- [MANAGER] Printing result ----- \n");
  printf("Decoded result: ");
  for (i = 0; i < n_input_data; i++) {
    printf("%c", data->vector[i]); 
  }
  printf("\n");
}

void signal_handler(int signo) {
  printf("\n[MANAGER] Program termination (Ctrl + C).\n");
  terminate_processes();
  free_resources();
  exit(EXIT_SUCCESS);
}
