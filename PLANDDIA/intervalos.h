/** intervalos.h
 */ 
#ifndef INTERVALOS_H
#define INTERVALOS_H
#define LONGIT	20    /*LONGITud*/
struct intervalo{
  char inicio[LONGIT];       /* Ejemplos: "07:30", "08:30" */
  char final[LONGIT];        /* Ejemplos: "08:20", "09:20" */
  int intInicioNSecs;        /* entero Inicio en Segundos */
  int intFinalNSecs;         /* entero Final en Segundos */  
};

struct disponibilidad{
  int cant_de_interv;        /* cantIDAD_de_intervALOS */
  struct intervalo *intervPt;/* apuntador a cant_de_interv intervalos */
};

struct fecha{ 
  int d;                     /* dia */
  int m;                     /* mes */
  int a;                     /* a\~no */
};

struct intervalos_en_la_fecha{
  struct fecha F;            /* fecha */     
  struct disponibilidad D;   /* intervalos disponibles en la fecha F */
};
#endif /*INTERVALOS_H*/

