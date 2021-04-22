#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "transforma.h"
#include "funciones.h"
#include "stack_int.h"



AFND* AFNDTransforma(AFND * afnd){
	Stack *stack = NULL;
	int iDest = -1;
	int numero_estados_determinista = 0;
	int numero_transiciones_determinista = 0;
	int indice_estado_inicial = -1;
	int num_simbolos = -1;
	int num_estados_original = -1;
  	estado auxiliar;
	int flag = 0;
	AFND * automata_nuevo = NULL;
	char *nombre_estado1_aux = NULL;
	char *nombre_estado2_aux = NULL;
	/*indices utilizados para los bucles*/
	int i;
	int j;
	int z;
	int w;
	/*Almacenaremos los estados del nuevo autómata*/
	estado *estados_AFD = NULL;
	transicion *transiciones_AFD = NULL;

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

	num_simbolos = AFNDNumSimbolos(afnd);
	if(num_simbolos == -1) return ERR;
	num_estados_original = AFNDNumEstados(afnd);
	if(num_estados_original == -1) return ERR;

	/*Crear una pila de ENTEROS, que almacenara indicies de los estados*/
	stack = stack_ini();
	if(!stack) return ERR;


	/*Guardamos el indice del estado inicial*/
	indice_estado_inicial = AFNDIndiceEstadoInicial(afnd);
	if(indice_estado_inicial == -1) return ERR;

	/*Metemos el indice del estado inicial en la pila*/
	if(stack_push(stack, indice_estado_inicial) == ERROR){
		perror ("\nError introduciendo el estado inicial en la pila\n");
		return ERR;
	}

	/*Vemos a donde podemos llegar con lambdas desde el primer estado*/
	transitividad_lambda_elementos_pila(afnd, stack);

	/*Añadimos a nuestra array de estados la informacion que hay en la pila*/
	if(de_pila_a_estado(afnd, stack, &numero_estados_determinista, estados_AFD) == ERROR) return ERR;


	/*Vemos las transiciones desde el estado inicial*/
	for (z = 0; z < numero_estados_determinista; z++){
		auxiliar = estados_AFD[z];

		/*Si no hemos explorado el estado*/
		if (auxiliar.explorado == FALSE){

			/*Para cada símbolo, miramos a que conjunto de estados se puede ir*/
			for (j = 0; j < num_simbolos; j++){
				for (i = 0; i < auxiliar.longitud_estado; i++){

					for (w = 0; w < num_estados_original; w++){
					/*Buscamos transiciones*/
						if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, auxiliar.estados[i], j, w)){
							if(intIsIn(stack, w) == FALSE){
								stack_push(stack, w);
							}
						}
					}
				}
				/*Buscamos transiciones con lambda*/
				transitividad_lambda_elementos_pila(afnd, stack);

				/*Este flag lo usamos para saber si tenemos que insertar transiciones o no.
				Si la pila está vacía, significa que no hemos encontrado transiciones
				Entre el estado que estabamos explorando, consumiendo el simbolo j,
				con ningun otro estado*/
				flag = 0;
				if (stack_isEmpty(stack)==FALSE){
					flag = 1;
				}

				/*Añadimos las nuevas transciones*/
				iDest = de_pila_a_estado(afnd, stack, &numero_estados_determinista, estados_AFD);
				if (flag == 1){
					aniadir_transicion_a_estructura(&numero_estados_determinista, estados_AFD, transiciones_AFD, &numero_transiciones_determinista, AFNDSimboloEn(afnd, j), z, iDest);
				}

			}
			auxiliar.explorado = TRUE;
		}

	}


	/*Creamos el automata con el numero de estados y simbolos ya conseguido*/

	automata_nuevo = AFNDNuevo("afd", numero_estados_determinista, num_simbolos);
	if (automata_nuevo == NULL){
		perror ("\nNO SE HA PODIDO CREAR EL AFD\n");
		stack_destroy(stack);
		exit (-1);
	}

	/*Insertamos todos los simbolos del AFND en el AFD*/
	for(z = 0; z < num_simbolos; z++){
		AFNDInsertaSimbolo(automata_nuevo, AFNDSimboloEn(afnd, z));
	}

	/*Insertamos todos los estados del estados_AFD en el AFD*/
	for (z = 0; z < numero_estados_determinista; z++){
		nombre_estado1_aux = arrayACadena(afnd, estados_AFD[z].estados, estados_AFD[z].longitud_estado);
		AFNDInsertaEstado(automata_nuevo, nombre_estado1_aux, estados_AFD[z].tipo);
		free(nombre_estado1_aux);
	}

	/*Insertamos todas las transiciones del transiciones_AFD en el AFD*/
	for (i = 0; i < numero_transiciones_determinista; i++){
		nombre_estado1_aux = arrayACadena(afnd, transiciones_AFD[i].origen, transiciones_AFD[i].longitud_origen);
		nombre_estado2_aux = arrayACadena(afnd, transiciones_AFD[i].destino, transiciones_AFD[i].longitud_destino);
		AFNDInsertaTransicion(automata_nuevo, nombre_estado1_aux, transiciones_AFD[i].simbolo, nombre_estado2_aux);
		free(nombre_estado1_aux);
		free(nombre_estado2_aux);
	}
	/*Liberamos memoria de todo*/
	stack_destroy(stack);
	free(estados_AFD);
	free(transiciones_AFD);
	return automata_nuevo;
}
