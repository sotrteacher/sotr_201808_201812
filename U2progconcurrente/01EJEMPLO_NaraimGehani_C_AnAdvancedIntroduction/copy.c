#include <stdio.h>

int
main(argc,argv)
int argc;
char *argv[];
{
	FILE *fopen(),*fs,*fd;
	int fclose();
	int c;

	fs=fopen(argv[1],"r");
	fd=fopen(argv[2],"w");

	while((c=fgetc(fs))!=EOF)
		fputc(c,fd);
	fclose(fs);
	fclose(fd);
	return 0;
}/*end main()*/

