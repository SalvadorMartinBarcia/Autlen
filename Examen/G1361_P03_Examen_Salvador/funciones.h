#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include "transforma.h"
#include "stack_int.h"


char *arrayACadena(AFND *afnd, int *array, int longitud);
int dosArraysSonIguales(int *array1, int longitud1, int *array2, int longitud2);
int tipoEstadoNoDeterminista(AFND * afnd, int *indices_estado, int longitud);
int de_pila_a_estado(AFND* afnd, Stack *p, int *num_estados_afnd, estado *conj_estados);
int de_pila_a_estado_2(AFND* afnd, Stack *p, int *num_estados_afnd, estado *conj_estados, int id);
void transitividad_lambda_elementos_pila(AFND * afnd, Stack *p);
void aniadir_transicion_a_estructura(int *num_estados_afd, estado *conj_estados, transicion *conj_transiciones, int *num_transiciones_afd, char *simbolo, int iOri, int iDest);
void aniadir_transicion_a_estructura_2(int *num_estados_afd, estado *conj_estados, transicion *conj_transiciones, int *num_transiciones_afd, char *simbolo, int iOri, int iDest);
Bool mismaClase(AFND* afd, int estado1, int estado2, int simbolo, int **matriz, int num_estados);

#endif
