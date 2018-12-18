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
void get_shm_segments(int *shm_symbol, struct TSymbol_t **p_symbol);
void get_sems(sem_t **p_sem_symbol_ready, sem_t **p_sem_symbol_decoded);

/* Task management */
void get_and_process_task(sem_t *sem_symbol_ready, sem_t *sem_symbol_decoded,
			  struct TSymbol_t *symbol);

/* INCLUDE REMAINING CODE HERE */
