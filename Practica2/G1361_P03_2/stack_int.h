

#ifndef STACK_INT_H
#define STACK_INT_H
#include "transforma.h"


typedef struct _Stack Stack;
/**------------------------------------------------------------------
Inicializa la pila reservando memoria. Salida: NULL si ha habido error o la pila si ha ido bien
------------------------------------------------------------------*/
Stack * stack_ini();
/**------------------------------------------------------------------
Elimina la pila Entrada: la pila que se va a eliminar
------------------------------------------------------------------*/
void stack_destroy(Stack *stack);
/**------------------------------------------------------------------
Inserta un int en la pila. Entrada: un int y la pila donde insertarlo. Salida: ERROR si no logra insertarlo, OK si lo logra
------------------------------------------------------------------*/
Status stack_push(Stack *stack, const int num);
/**------------------------------------------------------------------
Extrae un int en la pila. Entrada: la pila de donde extraerlo. Salida: NULL si no logra extraerlo o el int extraido si lo logra. Nótese que la pila quedará modificada
------------------------------------------------------------------*/
int stack_pop(Stack *stack);
/**------------------------------------------------------------------
Copia un int (reservando memoria) sin modificar el top de la pila. Entrada: la pila de donde copiarlo. Salida: NULL si no logra copiarlo o el int si lo logra
------------------------------------------------------------------*/
int stack_top(const Stack *stack);
/**------------------------------------------------------------------
Comprueba si la pila esta vacia. Entrada: pila. Salida: TRUE si está vacia o FALSE si no lo esta
------------------------------------------------------------------*/
Bool stack_isEmpty(const Stack *stack);
/**------------------------------------------------------------------
Comprueba si la pila esta llena. Entrada: pila. Salida: TRUE si está llena o FALSE si no lo esta
------------------------------------------------------------------*/
Bool stack_isFull(const Stack *stack);
/**------------------------------------------------------------------
Imprime toda la pila, colocando el int en la cima al principio de la impresión (y un int por línea). Entrada: pila y fichero donde imprimirla. Salida: Devuelve el número de caracteres escritos.
------------------------------------------------------------------*/
int stack_print(FILE*file,  Stack *stack);
/**------------------------------------------------------------------
Comprueba si el elemento pasado por argumento esta en la pila. Entrada: pila, elemento a evaluar Salida: TRUE si está en la pila o FALSE si no lo esta
------------------------------------------------------------------*/
Bool intIsIn(Stack *stack, int elemento);
#endif /* STACK_INT_H */

