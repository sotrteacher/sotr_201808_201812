/** asignacion_boleta_num_de_funcion.cpp
 */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NDEBUG
#include <assert.h>
using namespace std;

/**
 * Este programa se usa para borrar archivo fuente con nombre de la 
 * forma <NumDBoleta>.c y para remplazar una cadena de la forma 
 * <NumDBoleta> por otra cadena de la forma funcion<Num de funci\\'on>
 * en un archivo make (makefile) en la l\\'inea adecuada.
 */
int main(int argc,char *argv[])
{
  if(argc<3){
    printf("FORMA DE USO: %s <NumDBoleta> <Num de funci\\'on>\n",argv[0]);
    return 1;
  }
  int NumDBoleta=atoi(argv[1]);
  int num_de_funcion=atoi(argv[2]);

  /*Primero proceso los c\'odigos fuente*/
  /*Leer el archivo funcion$(num_de_funcion).c para escribir su contenido 
    en el archivo $(NumDBoleta).c con la edici\'on adecuada. */
  char file_funcion[64],file_boleta[64],centinela[64],centinela1[64];
  char file_makefile[64]="makefile",file_makefile1[64]="makefile1";
  char file_to_remove[64];
  string line,line_modified,string_centinela;
  size_t index;
  sprintf(file_to_remove,"%d.c",NumDBoleta);

  if(remove(file_to_remove)!=0){
    printf("ERROR, No se puede remover el archivo %s\n",file_to_remove);
    return (1);
  }

  sprintf(centinela,"%d",NumDBoleta);
  string_centinela=string(centinela);
  ifstream in1(file_makefile);
  if(in1){
    ofstream out1(file_makefile1);
    if(out1){
      while(getline(in1,line)){
        if((index=line.find(centinela,0))!=string::npos){
          if(num_de_funcion<10){
            sprintf(centinela,"FUNCION0%d:=funcion0%d",num_de_funcion,num_de_funcion);
          }else{
            sprintf(centinela,"FUNCION%d:=funcion%d",num_de_funcion,num_de_funcion);
          }
          line_modified=string(centinela);
          out1<<line_modified<<endl;
          continue;
        }
        out1<<line<<endl;
      }//end while()
      out1.close();
      in1.close();
    }//end if(out)
    else{
      printf("Fallo al abrir o crear %s\n",file_boleta);
      return -4;
    }
  }//end if(in1)
  else{
    printf("Fallo al abrir  %s\n",file_funcion);
    return -3;
  }
  /*Ahora sobreescribo el archivo make con los cambios apropiados (I hope)*/
  ifstream in2(file_makefile1);
  if(in2){
    ofstream out2(file_makefile);
    if(out2){
      while(getline(in2,line)){
        out2<<line<<endl;
      }
      out2.close();
      in2.close();
    }
    else{
      printf("Fallo al abrir o crear %s\n",file_makefile);
      return -6;
    }
  }
  else{
      printf("Fallo al abrir o crear %s\n",file_makefile1);
      return -5;
  }
  

  return 0;
}//end main()


