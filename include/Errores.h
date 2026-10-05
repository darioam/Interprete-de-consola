/* ====================================================================
 * Archivo: Errores.h
 * Descripción: Contiene la declaracion de funciones para la gestion
 *              de errores en el analisis del código fuente.
 * ==================================================================== */

#ifndef ERRORES_H
#define ERRORES_H

/**
 * @brief Maneja el error cuando se excede el tamaño máximo permitido.
 * 
 * Esta función se encarga de reportar un error cuando se detecta que
 * un elemento supera el tamaño permitido en el código fuente.
 * 
 * @param fila Número de fila donde ocurrió el error.
 * @param columna Número de columna donde ocurrió el error.
 */
void error_tamamo_maximo(int fila, int columna);

#endif