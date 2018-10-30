/**
 * shmem_unsemph.c - shared memory and unnamed semaphore.
 * Usa memoria compartida para crear una variable compartida 
 * utilizada para contar usando dos procesos. Se usa un 
 * semaforo anonimo para lograr exclusion mutua entre los dos 
 * procesos.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <limits.h>
#include <unistd.h>
#include <semaphore.h>

#define USAR_SEMAFORO_SINNOMBRE
#define DELAYH	100
#define DELAYP	75
#define CON_DELAYS
struct NySem{
  int N;
#ifdef USAR_SEMAFORO_SINNOMBRE
  sem_t mutex; /*Semaforo binario (Mutex)*/
#endif /*USAR_SEMAFORO_SINNOMBRE*/
};

int main(){
  int shmid, /*Identificador de la zona de mem compartida*/
       i,    /*Variable indice (local a cada proceso)*/
    Temp,    /*Variable Temporal (local a cada proceso)*/
  status;    /*Status devuelto por el hijo, recibido por el padre
               en la funcion wait() */
  struct NySem *ptr;  /*apuntador a estructura Numero entero 
                        y semaforo*/
  pid_t pid;   /*Identificador de proceso*/
  
  /*Peticion de memoria compartida*/
  shmid=shmget(IPC_PRIVATE,sizeof(struct NySem),IPC_CREAT|0600);
  if(shmid==-1){
    perror("Error en peticion de memoria compartida");
    exit(-1);
  }

  /*Union de la zona de memoria compartida a nuestro 
    espacio de direcciones*/
  if((ptr=(struct NySem*)shmat(shmid,0,0))==(struct NySem*)-1){
    perror("Error al adjuntar zona de memoria compartida");
    exit(-1);
  }

  /*Inicializar contador*/
  ptr->N=0;

  /*Inicializar semaforo*/
#ifdef USAR_SEMAFORO_SINNOMBRE
  if(sem_init(&ptr->mutex,  /*Address of an unnamed semaphore*/
                        1,  /*if 0, then the semaphore is shared between 
                              the threads of a process, and should be 
                              located at some address that is visible 
                              to all threads; If nonzero, then the 
                              semaphore is shared between processes,
                              and should be located in a region of shared
                              memory. Vease man sem_init.*/
                        1   /*specifies the inial value for the semaphore.*/
                    )==-1){
    perror("No puede iniciar el semafor mutex");
    exit(1);
  }
#endif /*USAR_SEMAFORO_SINNOMBRE*/

  if((pid=fork())<0){
    perror("Error en fork()");
    exit(-1);
  }else if(pid>0){/*padre*/
    for(i=1;i<=10;i++){
      /*Entrada a seccion critica*/
#ifdef USAR_SEMAFORO_SINNOMBRE
      if(sem_wait(&ptr->mutex)==-1){
        perror("Semaforo no valido");
        exit(1);
      }
#endif /*USAR_SEMAFORO_SINNOMBRE*/
      /*Inicio de seccion critica*/
#ifdef CON_DELAYS
      usleep(DELAYP);
#endif /*CON_DELAYS*/
      Temp=ptr->N;
      ptr->N=Temp+1;
      /*Fin de seccion critica*/
      /*Salida de seccion critica*/
#ifdef USAR_SEMAFORO_SINNOMBRE
      if(sem_post(&ptr->mutex)==-1){
        perror("Error semaforo al enviar signal");
        exit(1);
      }
#endif /*USAR_SEMAFORO_SINNOMBRE*/
    }//end for()
    if(wait(&status)!=pid){
      fprintf(stderr,"Una senial debio interrumpir la espera\n");
    }else{
      fprintf(stderr,"Padre e hijo han terminado de contar\n");
    }
  }else{/*hijo*/
    for(i=1;i<=10;i++){
      /*Entrada a seccion critica*/
#ifdef USAR_SEMAFORO_SINNOMBRE
      if(sem_wait(&ptr->mutex)==-1){
        perror("Semaforo no valido");
        exit(1);
      }
#endif /*USAR_SEMAFORO_SINNOMBRE*/
      /*Inicio de seccion critica*/
      Temp=ptr->N;
#ifdef CON_DELAYS
      usleep(DELAYH);
#endif /*CON_DELAYS*/
      ptr->N=Temp+1;
      /*Fin de seccion critica*/
      /*Salida de seccion critica*/
#ifdef USAR_SEMAFORO_SINNOMBRE
      if(sem_post(&ptr->mutex)==-1){
        perror("Error semaforo al enviar signal");
        exit(1);
      }
#endif /*USAR_SEMAFORO_SINNOMBRE*/
    }//end for()
    exit(0);
  }

  printf("\nEl valor de ptr->N es %d\n",ptr->N);
  
  /*Separacion de la zona de memoria compartida de nuestro espacio
    de direcciones virtuales*/
  shmdt(ptr);

  /*Borrado de la zona de memoria compartida*/
  shmctl(shmid,IPC_RMID,0);
  
  return 0;
}


