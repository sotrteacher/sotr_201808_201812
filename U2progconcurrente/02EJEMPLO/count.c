#include <stdio.h>

int
main()
{
  int count=0;
  while(getchar()!=EOF){
    count++;
  }
  printf("%d\n",count);
  return 0;
}
