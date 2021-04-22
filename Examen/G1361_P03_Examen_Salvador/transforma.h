#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include <stdio.h>
#include "afnd.h"

typedef enum {FALSE, TRUE} Bool;

typedef enum {
ERROR = -1, OK = 0
} Status;

#define ERR NULL

/*Nos define el número máximo de elementos por los que puede estar formado un único estado*/
#define ELEMENTOS_ESTADO 20

/*Es el tamaño maximo del nombre de un estado*/
#define TAMANIO_NOMBRE 100

AFND* AFNDTransforma(AFND * afnd);

typedef struct{
    int tipo;
    int estados[ELEMENTOS_ESTADO];
    int new_id;
    int longitud_estado;
    Bool explorado;
} estado;

typedef struct{
    int origen[ELEMENTOS_ESTADO];
    int longitud_origen;
    int origen_new_id;
    int destino[ELEMENTOS_ESTADO];
    int destino_new_id;
    int longitud_destino;
	char *simbolo;
} transicion;

#endif
