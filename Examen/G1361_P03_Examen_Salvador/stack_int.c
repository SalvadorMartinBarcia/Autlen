/* 
 * File:   main.c
 * Author: Patricia Matos y Blanca Mercado
 *
 * Created on 22 de febrero de 2018, 14:24
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack_int.h"
#define MAXSTACK 100


struct _Stack {
	int top;
	int item[MAXSTACK];
};

/*------------------------------------------------------------------
Inicializa la pila reservando memoria. Salida: NULL si ha habido error o la pila si ha ido bien
------------------------------------------------------------------*/
Stack * stack_ini(){
    Stack *ps = NULL;
    int i;

    ps = (Stack *) malloc (sizeof(Stack));
    if(!ps){
        return NULL;
    }

    for (i = 0; i<MAXSTACK; i++){
        ps->item[i] = -1;
    }

    ps->top = -1;

    return ps;
}
/**------------------------------------------------------------------
Elimina la pila Entrada: la pila que se va a eliminar
------------------------------------------------------------------*/
void stack_destroy(Stack *stack){
    int i;

    if(stack){
        for(i=0; i<= (stack->top+1); i++){
            stack->item[i] = 0;
        }
        free(stack);
    }
}
/**------------------------------------------------------------------
Inserta un int en la pila. Entrada: un int y la pila donde insertarlo. Salida: ERROR si no logra insertarlo, OK si lo logra
------------------------------------------------------------------*/
Status stack_push(Stack *stack, const int num){
	int pnum;

	if (!stack || num < 0 || stack_isFull (stack) == TRUE){
		return ERROR;
}
	pnum = num;

	stack->top++;
	stack->item[stack->top] = pnum;

	return OK;


}
/**-----------------------------------------f-------------------------
Extrae un int en la pila. Entrada: la pila de donde extraerlo. Salida: NULL si no logra extraerlo o el int extraido si lo logra. Nótese que la pila quedará modificada
------------------------------------------------------------------*/
int stack_pop (Stack *stack){
	int pnum;

	if (!stack || stack_isEmpty(stack) == TRUE){
		return -1;
	}

	pnum = stack->item[stack->top];

	stack->item[stack->top] = -1;

	stack->top--;

	return pnum;

}
/**------------------------------------------------------------------
Copia un int sin modificar el top de la pila. Entrada: la pila de donde copiarlo. Salida: NULL si no logra copiarlo o el int si lo logra
------------------------------------------------------------------*/
int stack_top(const Stack *stack){
	int pnum;

	if (!stack || stack_isEmpty(stack) == TRUE){
		return -1;
	}

	pnum = stack->item[stack->top];

	return pnum;


}
/**------------------------------------------------------------------
Comprueba si la pila esta vacia. Entrada: pila. Salida: TRUE si está vacia o FALSE si no lo esta
------------------------------------------------------------------*/
Bool stack_isEmpty(const Stack *stack){

	if (stack == NULL){
		return TRUE;
	}

	if((stack->top) == -1){
		return TRUE;
}

	return FALSE;
}
/**------------------------------------------------------------------
Comprueba si la pila esta llena. Entrada: pila. Salida: TRUE si está llena o FALSE si no lo esta
------------------------------------------------------------------*/
Bool stack_isFull(const Stack *stack){
	if (!stack){
		return TRUE;
	}

	if(stack->top ==  MAXSTACK-1){
		return TRUE;
	}

	return FALSE;
}
/**------------------------------------------------------------------
Imprime toda la pila, colocando el int en la cima al principio de la impresión (y un int por línea). Entrada: pila y fichero donde imprimirla. Salida: Devuelve el número de caracteres escritos.
------------------------------------------------------------------*/
int stack_print(FILE*file, Stack *stack){
	int i, nCaracteres;

	if(!file || !stack){
		return -1;
	}

for (i= stack->top, nCaracteres = 0; i != -1; i--){

	fprintf(file, "%d\n", stack->item[i]);
	nCaracteres++;

}

return nCaracteres;

}
/**------------------------------------------------------------------
Comprueba si el elemento pasado por argumento esta en la pila. Entrada: pila, elemento a evaluar Salida: TRUE si está en la pila o FALSE si no lo esta
------------------------------------------------------------------*/
Bool intIsIn(Stack *stack, int elemento){
	int i;
	for(i = 0; i < MAXSTACK; i++){
		if(stack->item[i] == elemento)
			return TRUE;
	}
	return FALSE;
}
