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

#ifndef __SEMAPHOREI_H__
#define __SEMAPHOREI_H__

#include <semaphore.h>

sem_t *create_semaphore (const char *name, unsigned int value);
sem_t *get_semaphore    (const char *name);
void remove_semaphore   (const char *name);
void signal_semaphore   (sem_t *sem);
void wait_semaphore     (sem_t *sem);

#endif
