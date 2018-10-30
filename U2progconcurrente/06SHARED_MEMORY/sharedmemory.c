#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <limits.h>
#include <unistd.h>

int main(){
  int shmid; /*Identificador de la zona de mem compartida*/
  int *N,status;
  pid_t pid;
  /*Peticion de memoria compartida*/
  shmid=shmget(IPC_PRIVATE,5*sizeof(int),IPC_CREAT|0600);
  if(shmid==-1){
    perror("Error en peticion de memoria compartida");
    exit(-1);
  }
  /*Union de la zona de memoria compartida a nuestro 
    espacio de direcciones*/
  if((N=(int*)shmat(shmid,0,0))==(int*)-1){
    perror("Error al adjuntar zona de memoria compartida");
    exit(-1);
  }
  /*Inicializar variable compartida*/
  *N=0;
  if((pid=fork())<0){
    perror("Error en fork()");
    exit(-1);
  }else if(pid==0){/*hijo*/
    *N=20;
    exit(0);
  }else{/*padre*/
    if(wait(&status)!=pid){
      fprintf(stderr,"Una senial debio interrumpir la espera\n");
    }else{
      fprintf(stderr,"El hijo ha terminado su ejecuci\\'on!\n");
      printf("La variable compartida *N=%d\n",*N);
    }
  }
  

  /*Separacion de la zona de memoria compartida de nuestro espacio
    de direcciones virtuales*/
  shmdt(N);

  /*Borrado de la zona de memoria compartida*/
  shmctl(shmid,IPC_RMID,0);
  return 0;
}//end main()

