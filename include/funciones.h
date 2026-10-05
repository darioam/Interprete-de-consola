#ifndef FUNCIONES_H
#define FUNCIONES_H

double vsin(void *d);

double vcos(void *d);

double vtan(void *d);

double vexp(void *d);

double vlog(void *d);

// Limpia la terminal (llama al sistema operativo)
double clean(void *d);

// Carga un archivo y ejecuta las instrucciones línea por línea
double load(void *nombre_archivo);

// Muestra información de ayuda sobre las funciones disponibles
double help(void *d);

// Finaliza la ejecución del intérprete (pendiente de implementación)
double quit(void *d);

// Limpia una variable específica (elimina su valor)
double clear(void *lexema);

double workspace(void *d);

double import(void *d);

#endif // FUNCIONES_H