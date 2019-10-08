/** asignacion_boleta_num_de_funcion.cpp
 */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

/**
 * Este programa se usa para generar un archivo fuente con nombre de la 
 * forma <NumDBoleta>.c a partir de un archivo fuente 
 * funcion<Num de funci\\'on>.c 
 * y para modificar un archivo make remplazando una cadena de la forma 
 * funcion<Num de funci\\'on> por otra cadena de la forma 
 * <NumDBoleta> en la l\'inea adecuada.
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
  string line,line_modified,string_centinela;
  size_t index;
  if(num_de_funcion<10){
    sprintf(centinela,"#define DISC0%d",num_de_funcion);
  }else{
    sprintf(centinela,"#define DISC%d",num_de_funcion);
  }
  if(num_de_funcion<10){
    sprintf(centinela1,"#ifndef DISC0%d",num_de_funcion);
  }else{
    sprintf(centinela1,"#define DISC%d",num_de_funcion);
  }
  sprintf(file_boleta,"%d.c",NumDBoleta);
  if(num_de_funcion<10){
    sprintf(file_funcion,"funcion0%d.c",num_de_funcion);
  }else{
    sprintf(file_funcion,"funcion%d.c",num_de_funcion);
  }
  ifstream in(file_funcion);
  if(in){
    ofstream out(file_boleta);
    if(out){
      while(getline(in,line)){
        if(!strcmp(line.c_str(),centinela)){
          sprintf(centinela,"#define DISC%d\n",NumDBoleta);
          //line=string(centinela);
          continue;
        }
        if(!strcmp(line.c_str(),centinela1)){
          sprintf(centinela1,"#ifndef DISC%d\n",NumDBoleta);
          line=string(centinela1);
        }
        out<<line<<endl;
      }//end while()
      out.close();
      in.close();
    }//end if(out)
    else{
      printf("Fallo al abrir o crear %s\n",file_boleta);
      return -2;
    }
  }//end if(in)
  else{
    printf("Fallo al abrir  %s\n",file_funcion);
    return -1;
  }

  /*Ahora proceso el archivo makefile*/
  if(num_de_funcion<10){
    sprintf(centinela,"funcion0%d",num_de_funcion);
  }else{
    sprintf(centinela,"funcion%d",num_de_funcion);
  }
  string_centinela=string(centinela);
  ifstream in1(file_makefile);
  if(in1){
    ofstream out1(file_makefile1);
    if(out1){
      while(getline(in1,line)){
        if((index=line.find(centinela,0))!=string::npos){
          if(num_de_funcion<10){
            sprintf(centinela,"FUNCION0%d:=%d",num_de_funcion,NumDBoleta);
          }else{
            sprintf(centinela,"FUNCION%d:=%d",num_de_funcion,NumDBoleta);
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


