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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <definitions.h>
#include <semaphoreI.h>

/* Semaphores and shared memory retrieval */
void get_shm_segments(int *shm_data, int *shm_task, int *shm_symbol,
		      struct TData_t **p_data, struct TTask_t **p_task, struct TSymbol_t **p_symbol);

void get_sems(sem_t **p_sem_task_ready, sem_t **p_sem_task_read, 
	      sem_t **p_sem_task_processed, sem_t **p_sem_mutex, 
	      sem_t **p_sem_symbol_ready, sem_t **p_sem_symbol_decoded);

/* Task management */
void get_and_process_task(sem_t *sem_task_ready, sem_t *sem_task_read, 
			  sem_t *sem_mutex, sem_t *sem_symbol_ready, sem_t *sem_symbol_decoded,
			  struct TData_t *data, const struct TTask_t *task, 
			  struct TSymbol_t *symbol);
void notify_task_completed(sem_t *sem_task_processed);

/******************** Main function ********************/

int main(int argc, char *argv[]) {
  struct TData_t *data;
  struct TTask_t *task;
  struct TSymbol_t *symbol;
  int shm_data, shm_task, shm_symbol;
  sem_t *sem_task_ready, *sem_task_read, *sem_task_processed;
  sem_t *sem_mutex, *sem_symbol_ready, *sem_symbol_decoded;

  /* Get shared memory segments and semaphores */
  get_shm_segments(&shm_data, &shm_task, &shm_symbol, &data, &task, &symbol);
  get_sems(&sem_task_ready, &sem_task_read, &sem_task_processed,
	   &sem_mutex, &sem_symbol_ready, &sem_symbol_decoded);
  
  /* Will work until killed by the manager */
  while (TRUE) {
    get_and_process_task(sem_task_ready, sem_task_read, sem_mutex, sem_symbol_ready, 
			 sem_symbol_decoded, data, task, symbol);
    notify_task_completed(sem_task_processed);
  }

  return EXIT_SUCCESS;
}

/******************** Semaphores and shared memory retrieval ********************/

void get_shm_segments(int *shm_data, int *shm_task, int *shm_symbol,
		      struct TData_t **data, struct TTask_t **task, struct TSymbol_t **symbol) {
  *shm_data = shm_open(SHM_DATA, O_RDWR, 0644);
  *data = mmap(NULL, sizeof(struct TData_t), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_data, 0);

  *shm_task = shm_open(SHM_TASK, O_RDWR, 0644);
  *task = mmap(NULL, sizeof(struct TTask_t), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_task, 0);

  *shm_symbol = shm_open(SHM_SYMBOL, O_RDWR, 0644);
  *symbol = mmap(NULL, sizeof(struct TSymbol_t), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_symbol, 0);
}

void get_sems(sem_t **p_sem_task_ready, sem_t **p_sem_task_read, 
	      sem_t **p_sem_task_processed, sem_t **p_sem_mutex, 
	      sem_t **p_sem_symbol_ready, sem_t **p_sem_symbol_decoded) {
  *p_sem_task_ready = get_semaphore(SEM_TASK_READY);
  *p_sem_task_read = get_semaphore(SEM_TASK_READ);
  *p_sem_task_processed = get_semaphore(SEM_TASK_PROCESSED);
  *p_sem_mutex = get_semaphore(SEM_MUTEX);
  *p_sem_symbol_ready = get_semaphore(SEM_SYMBOL_READY);
  *p_sem_symbol_decoded = get_semaphore(SEM_SYMBOL_DECODED);
}

/******************** Task management ********************/

void get_and_process_task(sem_t *sem_task_ready, sem_t *sem_task_read, 
			  sem_t *sem_mutex, sem_t *sem_symbol_ready, sem_t *sem_symbol_decoded,
			  struct TData_t *data, const struct TTask_t * task,
			  struct TSymbol_t *symbol) {

  /* INCLUDE REMAINING CODE HERE */
  
}

void notify_task_completed(sem_t *sem_task_processed) {
  signal_semaphore(sem_task_processed);
}
