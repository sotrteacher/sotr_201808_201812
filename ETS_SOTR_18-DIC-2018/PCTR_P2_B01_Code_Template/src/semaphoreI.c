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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <semaphoreI.h>

sem_t *create_semaphore (const char *name, unsigned int value) {
  sem_t *sem;

  if ((sem = sem_open(name, O_CREAT, 0644, value)) == SEM_FAILED) {
    fprintf(stderr, "Error creating semaphore <%s>: %s\n", name, strerror(errno));
    exit(EXIT_FAILURE);
  }

  return sem;
}

sem_t *get_semaphore (const char *name) {
  sem_t *sem;

  if ((sem = sem_open(name, O_RDWR)) == SEM_FAILED) {
    fprintf(stderr, "Error retrieving semaphore <%s>: %s\n", name, strerror(errno));
    exit(EXIT_FAILURE);
  }

  return sem;
}

void remove_semaphore (const char *name) {
  sem_t *sem = get_semaphore(name);

  if ((sem_close(sem)) == -1) {
    fprintf(stderr, "Error closing semaphore <%s>: %s\n", name, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if ((sem_unlink(name)) == -1) {
    fprintf(stderr, "Error removing semaphore <%s>: %s\n", name, strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void signal_semaphore (sem_t *sem) {
  if ((sem_post(sem)) == -1) {
    fprintf(stderr, "Error incrementing the semaphore: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void wait_semaphore (sem_t *sem) {
  if ((sem_wait(sem)) == -1) {
    fprintf(stderr, "Error decrementing the semaphore: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}
