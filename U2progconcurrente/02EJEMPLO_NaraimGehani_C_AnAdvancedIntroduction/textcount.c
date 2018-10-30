/*-------------------------------------------------*/
/* textcount: Count the number of characters in    */
/*            the file after deleting all the      */
/*            text formatting commands, lines      */
/*            that begin with a "." in column 1    */
/*-------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>  /*pipe(int p[2])*/
#include <stdlib.h>  /*exit()*/

#define R	0
#define W	1
#define TRUE	1
#define FALSE	0
#define PERIOD	'.'

int
main()
{
  int pid;
  int p[2],q[2];

  FILE *fdopen(),*fp;
  int c;
  int newline=TRUE;
  int total;
  pipe(p);
  pipe(q);
  switch(pid=fork()){
    case 0:{
//      close(p[W]);
                     /* setup the pipes */
                     /* child will read from pipe "p" and write */
                     /* into pipe "q"; consequently, the write end */
                     /* of "p" and the read end of "q" are */
                     /* closed; the standard input of the child */
                     /* is made synonymous with the read end of */
                     /* "p" and the write end of "q" is made */
                     /* synonymous with standard output (of child)*/
                     /* File descriptor 0 = standard input  */ 
                     /* File descriptor 1 = standard output */
      close(R);dup(p[R]);close(p[R]);close(p[W]);
      close(W);dup(q[W]);close(q[R]);close(q[W]);
      execl("/home/beto/U2progconcurrente/02EJEMPLO/count","count",0);
//      printf("textcount: exec failed");
      perror("textcount: exec failed");
      exit(1);
    }
    case -1:{
//      printf("textcount could not fork a process");
      perror("textcount could not fork a process");
      exit(1);
    }
    default:{
      close(p[R]);
      close(q[W]);
      fp=fdopen(p[W],"w");
    }
  }//end switch
  while((c=getchar())!=EOF){
    switch(newline){
      case TRUE:{
      if(c=='\n'){
        putc(c,fp);
      }else if(c==PERIOD){
        while((c=getchar())!=EOF && c!='\n')
        ;
      }else{
        putc(c,fp);
        newline=FALSE;
      }
      break;
      }
      default:{
        putc(c,fp);
        if(c=='\n'){
          newline=TRUE;
        }
      }
    }//end switch
  }//end while()
  fclose(fp);          /* the write end of the pipe     */
                       /* must be closed so the process */
                       /* at the read end of the pipe   */
                       /* can sense an EOF              */
  close(R);dup(q[R]);close(q[R]);close(q[W]);
  scanf("%d",&total);
  printf("Total number of characters %d\n",total);
  exit(0);
}//end main()

