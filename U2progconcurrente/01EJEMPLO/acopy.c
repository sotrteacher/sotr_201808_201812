#include <stdio.h>
#include <unistd.h>

int
acopy(src,dest)
	char *src,*dest;
{
	FILE *fopen(),*fs,*fd;
	int fclose();
	int fork(),pid;

	if((fs=fopen(src,"r"))==NULL){
		printf("acopy:cannot open file %s\n",src);
		return -1;
	}
	if((fd=fopen(dest,"w"))==NULL){
		printf("acopy:cannot open file %s\n",dest);
		fclose(fs);
		return -1;
	}
	fclose(fs);
	fclose(fd);

	switch(pid=fork()){
		case 0:{execl("/home/beto/U2progconcurrente/01EJEMPLO/copy","copy",src,dest,0);break;}
		case -1:{
				printf("acopy: cannot fork a process\n");
				return -1;}
		default:{return 0;}
	}
}/*acopy()*/

int
main(argc,argv)
	int argc;
	char *argv[];
{
	acopy(argv[1],argv[2]);
	return 0;

}/*main()*/


