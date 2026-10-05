#include <stdio.h>

/**
 * @brief Maneja el error cuando se excede el tamaño máximo permitido.
 * 
 * Esta función se encarga de reportar un error cuando se detecta que
 * un elemento supera el tamaño permitido en el código fuente.
 * 
 * @param fila Número de fila donde ocurrió el error.
 * @param columna Número de columna donde ocurrió el error.
 */
void error_tamamo_maximo(int fila, int columna) {
    printf("Se alcanza el tamaño máximo de lexema en la fila %d, columna %d\n", fila, columna); 
}