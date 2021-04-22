#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "transforma.h"
#include "minimiza.h"
#include "funciones.h"
#include "stack_int.h"

#define MAX_FILAS 100

AFND* AFNDMinimiza(AFND * afnd){
    Stack *stack = NULL;
	int numero_estados_minimo = 0;
	int numero_transiciones_minimo = 0;
	int num_simbolos = -1;
	int num_estados_original = -1;
	int flag1 = 0;
	AFND * automata_determinista = NULL;
	AFND * automata_minimo = NULL;
	char *nombre_estado1_aux = NULL;
	char *nombre_estado2_aux = NULL;
	int estados_aux_count = 0;
	int *estados_aux = NULL;
	/*indices utilizados para los bucles*/
	int i;
	int j;
	int z;
	int w;

	int **matriz_distinguible = NULL; /*0 = indistinguible && 1 = distinguible*/
	/*Almacenaremos los estados del nuevo autómata*/
	estado *estados_AFD = NULL;
	transicion *transiciones_AFD = NULL;

	/*Pasamos el algoritmo de transforma para quitar estados a los que no se puede llegar*/
	automata_determinista = AFNDTransforma(afnd);
	/*A partir de ahora usaremos este automata sin estados inutiles*/

	/*Cogemos el numero de simbolos y estados*/
	num_simbolos = AFNDNumSimbolos(automata_determinista);
	if(num_simbolos == -1) return ERR;
	num_estados_original = AFNDNumEstados(automata_determinista);
	if(num_estados_original == -1) return ERR;

	/*Array en la que almacenaremos los estados que ya se han usado y por lo tanto no hay que tratar de nuevo*/
	estados_aux = (int*)malloc(num_estados_original*sizeof(int));
	if(estados_aux == NULL){
		perror ("\nError reservando memoria para estados_aux\n");
		return ERR;
	}
	for(i=0; i<num_estados_original; i++)
		estados_aux[i] = -1;

	/*Reserva de martriz para saber si son distinguibles o no los estados*/
	matriz_distinguible = (int **)malloc(num_estados_original*sizeof(int*));  /*num_estados columnas*/
	if (matriz_distinguible == NULL){
		perror ("\nError reservando memoria para matriz_distinguible\n");
		return ERR;
	}
	for (i=0;i<num_estados_original;i++){/*Columnas*/
		matriz_distinguible[i] = (int*)malloc(num_estados_original*sizeof(int)); /*num_estados columnas*/
	}

	for(i = 0 ; i < num_estados_original; i++){
		for(j = 0; j < num_estados_original; j++){
			matriz_distinguible[i][j] = -1;
		} 
	}
	/*-----------------------------------------------------------------------------------------------*/
	/*Reserva de los estados y transiciones que seran los del automata final minimo*/
	estados_AFD = (estado *)malloc(100 * sizeof(estado));
	if (estados_AFD == NULL){
		perror ("\nError reservando memoria para estados_AFD\n");
		return ERR;
	}

	transiciones_AFD = (transicion *)malloc(100 * sizeof(transicion));
	if (transiciones_AFD == NULL){
		perror ("\nError reservando memoria para transiciones_AFD\n");
		return ERR;
	}
	
	/*Primera matriz de estados distinguibles e indistinguibles, solo usamos la parte superior ya que es simetrica*/
	for(i=0; i<num_estados_original; i++){
		for(j=i; j<num_estados_original; j++){
			z = AFNDTipoEstadoEn(automata_determinista, i);
			w = AFNDTipoEstadoEn(automata_determinista, j);
			if((z == INICIAL || z == NORMAL) && (w == INICIAL || w == NORMAL)){
				matriz_distinguible[i][j] = 0;
			}
			else if((z == FINAL || z == INICIAL_Y_FINAL) && (w == FINAL || w == INICIAL_Y_FINAL)){
				matriz_distinguible[i][j] = 0;
			}
			else{
				matriz_distinguible[i][j] = 1;
			}
		}
	}
	/*-----------------------------------------------------------------------------------------------*/


	do {
		flag1 = 0;/*Flag que nos dira si se ha modificado la matriz_distinguible en cuyo caso se hara otra iteracion del algoritmo*/
		/*Funcion que copia una matriz en la auxiliar para la futura comprobacion o el flag no se si podre hacerlo con flag*/
		for(i = 0 ; i < num_estados_original; i++) {
			for(j = i; j < num_estados_original; j++){
				if(matriz_distinguible[i][j] == 0){ /*Son del mismo grupo*/
					for(z = 0; z < num_simbolos; z++){
						if(mismaClase(automata_determinista, i, j, z, matriz_distinguible, num_estados_original) == FALSE){/*Hay que hacer esta funcion broooo*/
							matriz_distinguible[i][j] = 1; /*Marcamos como distinguible*/
							flag1 = 1; /*La matriz ha sido cambiada*/
							break;
						}
					}
				}
			/*El break saca a la matriz aqui*/
			}
		}
	} while(flag1 == 1);
	/*Al acabar este bucle tenemos la matriz_distinguibles final de la que podremos sacar los estados mínimos*/

	/*Crear una pila de ENTEROS, que almacenara indicies de los estados*/
	stack = stack_ini();
	if(!stack) return ERR;

	/*Este bucle nos dejara todos los nuevos estados en estados_AFD*/
	for(i=0; i<num_estados_original; i++){
		flag1=0;/*Ahora usaremos esta flag para saber si el estado que vamos a comprobar de la matriz ya esta metido*/
		for(j=0;j < estados_aux_count; j++){
			if(i == estados_aux[j])/*Ya esta metido en otro estado por lo tanto no lo consideramos*/
				flag1 = 1;
		}
		if(flag1 == 0){
			for(j=i; j<num_estados_original; j++){
				if(matriz_distinguible[i][j]  == 0){
					stack_push(stack, j);
					estados_aux[estados_aux_count] = j;
					estados_aux_count++;
				}
			}
		}
		de_pila_a_estado(automata_determinista, stack, &numero_estados_minimo, estados_AFD);
	}
	/*Reservamos memoria para el automata minimo*/
	automata_minimo = AFNDNuevo("afdm", numero_estados_minimo, num_simbolos);
	if (automata_minimo == NULL){
		perror ("\nNO SE HA PODIDO CREAR EL AFDM\n");
		stack_destroy(stack);
		exit (-1);
	}

	/*Insertamos todos los simbolos del AFD en el AFDM*/
	for(z = 0; z < num_simbolos; z++){
		AFNDInsertaSimbolo(automata_minimo, AFNDSimboloEn(automata_determinista, z));
	}
	/*Insertamos todos los estados nuevos*/
	for (z = 0; z < numero_estados_minimo; z++){
		nombre_estado1_aux = arrayACadena(automata_determinista, estados_AFD[z].estados, estados_AFD[z].longitud_estado);
		printf ("\nNOMBRE DEL ESTADO QUE ESTOY INSERTANDO: %s\n", nombre_estado1_aux);
		AFNDInsertaEstado(automata_minimo, nombre_estado1_aux, estados_AFD[z].tipo);
		free(nombre_estado1_aux);
	}

	/*Bucle que añade las transiciones de los estados nuevos a la array de transiciones*/
	for(i=0; i < numero_estados_minimo; i++){
		for(j=0; j < num_simbolos; j++){
			for(z=0; z < numero_estados_minimo; z++){
				for(w=0; w < estados_AFD[z].longitud_estado; w++){
					if(AFNDTransicionIndicesEstadoiSimboloEstadof(automata_determinista, estados_AFD[i].estados[0], j, estados_AFD[z].estados[w]) == 1){
						aniadir_transicion_a_estructura(&numero_estados_minimo, estados_AFD, transiciones_AFD, &numero_transiciones_minimo, AFNDSimboloEn(automata_determinista, j), i, z);
						printf("%d -%s> %d\n", i, AFNDSimboloEn(automata_determinista, j), z);
					}
				}
			}
		}
	}
	/*Insertamos todas las transiciones del transiciones_AFD en el AFD*/
	for (i = 0; i < numero_transiciones_minimo; i++){
		nombre_estado1_aux = arrayACadena(automata_determinista, transiciones_AFD[i].origen, transiciones_AFD[i].longitud_origen);
		nombre_estado2_aux = arrayACadena(automata_determinista, transiciones_AFD[i].destino, transiciones_AFD[i].longitud_destino);
		AFNDInsertaTransicion(automata_minimo, nombre_estado1_aux, transiciones_AFD[i].simbolo, nombre_estado2_aux);
		free(nombre_estado1_aux);
		free(nombre_estado2_aux);
	}

	
	for (i=0;i<num_estados_original;i++){
		free(matriz_distinguible[i]);
	}
	free(matriz_distinguible);
	stack_destroy(stack);
	free(estados_AFD);
	free(transiciones_AFD);
	free(estados_aux);
	AFNDElimina(automata_determinista);

	return automata_minimo;
}