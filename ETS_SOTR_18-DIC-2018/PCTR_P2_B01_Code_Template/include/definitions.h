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

#define SEM_TASK_READY     "sem_task_ready"
#define SEM_TASK_READ      "sem_task_read"
#define SEM_TASK_PROCESSED "sem_task_processed"
#define SHM_TASK           "shm_task"
#define SHM_DATA           "shm_data"

#define SEM_MUTEX          "sem_mutex"
#define SEM_SYMBOL_READY   "sem_symbol_ready"
#define SEM_SYMBOL_DECODED "sem_symbol_decoded"
#define SHM_SYMBOL         "shm_symbol"

#define DECODER_CLASS        "DECODER"
#define DECODER_PATH         "./exec/decoder"
#define SYMBOL_DECODER_CLASS "SYMBOL_DECODER"
#define SYMBOL_DECODER_PATH  "./exec/symbol_decoder"

#define MAX_ARRAY_SIZE 1000
#define NUM_SYMBOL_DECODERS 1
#define SEPARATOR "."
#define TRUE 1
#define FALSE 0

struct TData_t {
  char vector[MAX_ARRAY_SIZE];
};

struct TTask_t {
  int begin;
  int end;
};

struct TSymbol_t {
  char value;
};

enum ProcessClass_t {DECODER, SYMBOL_DECODER}; 

struct TProcess_t {          
  enum ProcessClass_t class; /* DECODER or SYMBOL_DECODER */
  pid_t pid;                 /* Process ID */
  char *str_process_class;   /* String representation of the process class */
};
