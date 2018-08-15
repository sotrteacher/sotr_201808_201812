#include <stdio.h>

struct boleta{
  int intBoleta;
  char nombre[100];
};

void mostrar_boleta_alumno(struct boleta *boletaPt);

int
main(int argc,char *argv[])
{
  struct boleta bol1;
  struct boleta *bolPt=&bol1;
  bolPt->intBoleta=2016640001;
  bolPt->nombre="Juan Hernandez Lara";
  mostrar_boleta_alumno(bolPt);

  free(bolPt);
  return 0;
}

/**
 * Imprime en pantalla el n\'umero de boleta y el 
 * nombre del alumno.
 */
void mostrar_boleta_alumno(struct boleta *boletaPt){
  // Teclee su c\'odigo aqu\'i
}
