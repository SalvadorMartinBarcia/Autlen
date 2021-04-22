#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transforma.h"
#include "funciones.h"

/*Funcion que mira si dados dos arrays de enteros tienen el
 * mismo contenido sin importar su orden.
 * Devuelve: -1 en caso de falso, 1, en caso de acierto
 */
int dosArraysSonIguales(int *array1, int longitud1, int *array2, int longitud2){
	int flag_iguales = 0;
	int i;
	int j;

	if (longitud1 != longitud2){
		return -1;
	}
	for (i = 0; i < longitud1; i++){
		int elemento = array1[i];
		flag_iguales = 0;
		for (j = 0; j < longitud2; j++){
			if (array2[j] == elemento){
				flag_iguales = 1;
				break;
			}
		}
		if (flag_iguales == 0){
			return -1;
		}
	}
	return 1;
	}


/*Funcion que nos dice si el estado es inicial (0), Final(1), inicial_y_final(2) normal (3)*/
/*Le pasamos un array con los índices del estado en el que estamos actualmente, y la longitud de este array*/
int tipoEstadoNoDeterminista(AFND * afnd, int *indices_estado, int longitud){
	int flag_inicial = 0;
	int flag_final = 0;
	int i;
	for (i = 0; i<longitud; i++){
		if (AFNDTipoEstadoEn(afnd, indices_estado[i]) == INICIAL)
			flag_inicial = 1;
		else if (AFNDTipoEstadoEn(afnd, indices_estado[i]) == FINAL)
			flag_final = 1;
		else if (AFNDTipoEstadoEn(afnd, indices_estado[i]) == INICIAL_Y_FINAL){
			flag_inicial = 1;
			flag_final = 1;
		}
	}
	if (flag_inicial && flag_final)
		return INICIAL_Y_FINAL;
	else if (flag_inicial)
		return INICIAL;
	else if (flag_final)
		return FINAL;
	return NORMAL;
	}

/*Funcion que pasa el estado o estados almacenados en la pila al array de estados pasado por argumento
*transformado en un solo estado del nuevo automata*/
int de_pila_a_estado(AFND *afnd, Stack *p, int *num_estados_afd, estado *conj_estados){
	int i;
	int *indices_estado = NULL;
	int contador = 0;

	if (stack_isEmpty(p) == TRUE){
		return -1;
		}
	indices_estado = (int *) malloc(ELEMENTOS_ESTADO*sizeof(int));
	if (indices_estado == NULL){
		perror("Error reservando memoria en de_pila_a_estado");
		exit(-1);
		}

	/*Usamos este contador para almacenar la variable longitud_estado*/
	for (i = 0;  stack_isEmpty(p) == FALSE; i++){
		indices_estado[i] = stack_pop(p);
		contador ++;
	}


	/*Primero comprobamos si ya existe un estado que tenga la información del estado que estamos intentando
	* introducir, para no introducirlo de nuevo*/
	if (*num_estados_afd > 0){
		for (i = 0; i < *num_estados_afd; i++){
			if (dosArraysSonIguales(indices_estado, contador, conj_estados[i].estados, conj_estados[i].longitud_estado) == 1){
				free (indices_estado);
				return i;
				}
		}
	}

	/*Metemos en el conjunto_estados la informacion*/
	conj_estados[*num_estados_afd].tipo = tipoEstadoNoDeterminista(afnd, indices_estado, contador);
	for (i = 0; i < contador; i++){
		conj_estados[*num_estados_afd].estados[i] = indices_estado[i];
	}

	/*Ponemos la variable explorado a 0, porque todavia no hemos mirado las posibles transiciones que tiene ese estado*/
	conj_estados[*num_estados_afd].explorado = 0;
	conj_estados[*num_estados_afd].longitud_estado = contador;
	free (indices_estado);
	*num_estados_afd += 1;
	return (*num_estados_afd) - 1;
}

int de_pila_a_estado_2(AFND *afnd, Stack *p, int *num_estados_afd, estado *conj_estados, int id){
	int i;
	int *indices_estado = NULL;
	int contador = 0;

	if (stack_isEmpty(p) == TRUE){
		return -1;
		}
	indices_estado = (int *) malloc(ELEMENTOS_ESTADO*sizeof(int));
	if (indices_estado == NULL){
		perror("Error reservando memoria en de_pila_a_estado");
		exit(-1);
		}

	/*Usamos este contador para almacenar la variable longitud_estado*/
	for (i = 0;  stack_isEmpty(p) == FALSE; i++){
		indices_estado[i] = stack_pop(p);
		contador ++;
	}


	/*Primero comprobamos si ya existe un estado que tenga la información del estado que estamos intentando
	* introducir, para no introducirlo de nuevo*/
	if (*num_estados_afd > 0){
		for (i = 0; i < *num_estados_afd; i++){
			if (dosArraysSonIguales(indices_estado, contador, conj_estados[i].estados, conj_estados[i].longitud_estado) == 1){
				free (indices_estado);
				return i;
				}
		}
	}

	/*Metemos en el conjunto_estados la informacion*/
	conj_estados[*num_estados_afd].tipo = tipoEstadoNoDeterminista(afnd, indices_estado, contador);
	for (i = 0; i < contador; i++){
		conj_estados[*num_estados_afd].estados[i] = indices_estado[i];
	}

	/*Ponemos la variable explorado a 0, porque todavia no hemos mirado las posibles transiciones que tiene ese estado*/
	conj_estados[*num_estados_afd].explorado = 0;
	conj_estados[*num_estados_afd].longitud_estado = contador;
	conj_estados[*num_estados_afd].new_id = id;
	free (indices_estado);
	*num_estados_afd += 1;
	return (*num_estados_afd) - 1;
}

/*Esta funcion añade a la pila que nos da como parametro los estados
 * a los que se puede transitar usando solo transiciones lambda (desde los estados
 * que se encuentran dentro de la pila*/
void transitividad_lambda_elementos_pila(AFND * afnd, Stack *p){
	int i;
	int j;
	Stack * pila_aux = NULL;
	if (stack_isEmpty(p) == TRUE) return;
	pila_aux = stack_ini();

	for (i = 0; stack_isEmpty(p) == FALSE; i++){
		int estado = stack_pop(p);
		stack_push(pila_aux, estado);

		/*Recorremos todos los estados, para ver a cuales se puede llegar usando la transicion lambda*/
		/*Lo añadimos a la pila auxiliar, que luego copiaremos a la pila original*/
		for (j = 0; j < AFNDNumEstados(afnd); j++){
			if (estado != j && AFNDCierreLTransicionIJ(afnd, estado, j)){
				stack_push(pila_aux, j);
			}
		}
	}

	if (stack_isEmpty(pila_aux)==TRUE){
		return;
	}

	/*Pasamos todo lo de pila aux a la pila original*/
	for (i = 0; stack_isEmpty(pila_aux) == FALSE; i++){
		int estado = stack_pop(pila_aux);
		stack_push(p, estado);
	}


	/*Liberar memoria de la pila auxiliar*/
	stack_destroy(pila_aux);
}


/*Esta función añade una nueva transicion, entre el estado que esta
 * en el conjunto de elementos indicado por iOri, y el estado iDest
 */
void aniadir_transicion_a_estructura(int *num_estados_afd, estado *conj_estados, transicion *conj_transiciones, int *num_transiciones_afd, char *simbolo, int iOri, int iDest){
	int *origen = NULL;
	int *destino = NULL;
	int i;

	origen = conj_estados[iOri].estados;

	destino = conj_estados[iDest].estados;

	for (i = 0; i < conj_estados[iOri].longitud_estado; i++){
		conj_transiciones[*num_transiciones_afd].origen[i] = origen[i];
	}
	conj_transiciones[*num_transiciones_afd].longitud_origen = conj_estados[iOri].longitud_estado;



	for (i = 0; i < conj_estados[iDest].longitud_estado; i++){
		conj_transiciones[*num_transiciones_afd].destino[i] = destino[i];
	}
	conj_transiciones[*num_transiciones_afd].longitud_destino = conj_estados[iDest].longitud_estado;

	conj_transiciones[*num_transiciones_afd].simbolo = simbolo;
	*num_transiciones_afd += 1;
}

void aniadir_transicion_a_estructura_2(int *num_estados_afd, estado *conj_estados, transicion *conj_transiciones, int *num_transiciones_afd, char *simbolo, int iOri, int iDest){
	int *origen = NULL;
	int *destino = NULL;
	int i;

	origen = conj_estados[iOri].estados;

	destino = conj_estados[iDest].estados;

	for (i = 0; i < conj_estados[iOri].longitud_estado; i++){
		conj_transiciones[*num_transiciones_afd].origen[i] = origen[i];
	}
	conj_transiciones[*num_transiciones_afd].longitud_origen = conj_estados[iOri].longitud_estado;
	conj_transiciones[*num_transiciones_afd].origen_new_id = conj_estados[iOri].new_id;



	for (i = 0; i < conj_estados[iDest].longitud_estado; i++){
		conj_transiciones[*num_transiciones_afd].destino[i] = destino[i];
	}
	conj_transiciones[*num_transiciones_afd].longitud_destino = conj_estados[iDest].longitud_estado;
	conj_transiciones[*num_transiciones_afd].destino_new_id = conj_estados[iDest].new_id;

	conj_transiciones[*num_transiciones_afd].simbolo = simbolo;
	*num_transiciones_afd += 1;
}

/*Funcion que dado los indices de un automata y su correspondiente automata crea el nombre para el nuevo estado*/
char *arrayACadena(AFND *afnd, int *array, int longitud){
	char *aux = NULL;
	char *nombre_estado = NULL;
	int i;
	aux = (char *)calloc(TAMANIO_NOMBRE, sizeof(char));
	if (aux == NULL){
		perror("\nNo se ha podido reservar memoria para la cadena del nombre\n");
		exit(-1);
	}

	for (i = 0; i < longitud; i++){
		int elemento = array[i];
		nombre_estado = AFNDNombreEstadoEn(afnd, elemento);
		strcat(aux, nombre_estado);
	}
	return aux;
}
	
/*Esta función nos dira si dados dos estados y un simbolo los estados transicionan a la misma clase*/
Bool mismaClase(AFND* afd, int estado1, int estado2, int simbolo, int **matriz, int num_estados){
	int i, final1 = -1, final2 = -1, aux;
	/*Para el estado 1*/
	for(i=0; i < num_estados; i++){
		if(AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado1, simbolo, i) == 1)
			final1 = i;
	}
	/*Para el estado 2*/
	for(i=0; i < num_estados; i++){
		if(AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado2, simbolo, i) == 1)
			final2 = i;
	}
	
	if(final1 == -1 && final2 == -1)
		return TRUE;

	if(final1 == -1 && final2 != -1)
		return FALSE;

	if(final1 != -1 && final2 == -1)
		return FALSE;

	/*Usaremos el auxiliar para que final2 sea el mayor ya que usaremos la parte superior de la matriz*/
	if(final1 > final2){
		aux = final2;
		final2 = final1;
		final1 = aux;
	}
	
	if (matriz[final1][final2] == 0)
	 	return TRUE;
	else if(matriz[final1][final2] == -1)
		printf("ERROR EN LOS INDICES ACCESO A LA PARTE BAJA DE LA MATRIZ\n");
	return FALSE;
}
