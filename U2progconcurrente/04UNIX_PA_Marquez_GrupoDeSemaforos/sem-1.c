#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

#define SEM_HIJO	0
#define SEM_PADRE	1

//#define CON_SEMAFOROS

int
main(int arcg,char *argv[])
{
  int i=10,semid,pid;
  struct sembuf operacion;
  key_t llave;

#ifdef CON_SEMAFOROS
  llave=ftok(argv[0],'K');
  if((semid=semget(llave,2,IPC_CREAT | 0600)) == -1){
    perror("semget failed");
    exit(-1);
  } 
  semctl(semid,SEM_HIJO,SETVAL,0);
  semctl(semid,SEM_PADRE,SETVAL,1);
#endif /*CON_SEMAFOROS*/
  if((pid=fork())==-1){
    perror("fork failed");
    exit(-1);
  }else if(pid==0){
    while(i){
#ifdef CON_SEMAFOROS
      operacion.sem_num=SEM_HIJO;
      operacion.sem_op=-1;
      operacion.sem_flg=0;
      semop(semid,&operacion,1);
#endif /*CON_SEMAFOROS*/
      printf("PROCESO HIJO: %3d\n",i--);

#ifdef CON_SEMAFOROS
      operacion.sem_num=SEM_PADRE;
      operacion.sem_op=1;
      semop(semid,&operacion,1);
#endif /*CON_SEMAFOROS*/
    }

#ifdef CON_SEMAFOROS
    semctl(semid,0,IPC_RMID,0);
#endif /*CON_SEMAFOROS*/
  }else{
#ifdef CON_SEMAFOROS
    operacion.sem_flg=0;
#endif /*CON_SEMAFOROS*/
    while(i){
#ifdef CON_SEMAFOROS
      operacion.sem_num=SEM_PADRE;
      operacion.sem_op=-1;
      semop(semid,&operacion,1);
#endif /*CON_SEMAFOROS*/
      printf("PROCESO PADRE: %2d\n",i--);

#ifdef CON_SEMAFOROS
      operacion.sem_num=SEM_HIJO;
      operacion.sem_op=1;
      semop(semid,&operacion,1);
#endif /*CON_SEMAFOROS*/
    }

#ifdef CON_SEMAFOROS
    semctl(semid,0,IPC_RMID,0);
#endif /*CON_SEMAFOROS*/
  }

}//end main()

