/** test_intervalos.c
 */ 
#include <stdio.h>
#include <string.h>
//#define NDEBUG
#include <assert.h>
#include <stdlib.h>
#include "intervalos.h"

int main(int argc,char *argv[])
{
  int n=2;
  struct fecha f1;
  f1.d=4;
  f1.m=10;
  f1.a=2019;
  struct disponibilidad Ddf1;
  Ddf1.cant_de_interv=n;
  Ddf1.intervPt=(struct intervalo*)malloc( 
n*sizeof(struct intervalo));
  strcpy(Ddf1.intervPt->inicio,"14:35");
  strcpy(Ddf1.intervPt->final,"15:20"); 
  strcpy((Ddf1.intervPt+1)->inicio,"15:30");
  strcpy((Ddf1.intervPt+1)->final,"15:55"); 
  struct intervalos_en_la_fecha Inf1;
  Inf1.F=f1;
  Inf1.D=Ddf1;
void print_intervalos_en_la_fecha(struct 
intervalos_en_la_fecha *);
  print_intervalos_en_la_fecha(&Inf1);
  
  return 0;
}/*end main()*/

