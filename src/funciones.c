
#include "funciones.h"  // Incluir el archivo de encabezado
#include "parser.h"
#include "lexer.h"
#include "TS.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define MAX_LINE_LENGTH 1024
FILE *archivo;

double vsin(void *d) {
    double valor = *(double *)d;
    return sin(valor);
}

double vcos(void *d) {
    double valor= *(double *)d;
    return cos(valor);
}

double vtan(void *d) {
    double valor= *(double *)d;
    return tan(valor);
}

double vexp(void *d) {
    double valor= *(double *)d;
    return exp(valor);
}

double vlog(void *d) {
    double valor= *(double *)d;
    if(valor <= 0) {
        yyerror("Intento de calcular logaritmo de un número no positivo.");
        return NAN;
    }
    return log(valor);
}

double clean(void *) {
    system("clear");
    return 0.0;
}

double load(void *nombre_archivo) {
    
    char linea[MAX_LINE_LENGTH];

    // Abrir el archivo para lectura
    archivo = fopen((char *)nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NAN;
    }

    // Leer el archivo línea por línea
    while (fgets(linea, sizeof(linea), archivo)) {

        cargar_linea(linea);
        
        // Procesar la expresión línea por línea
        yyparse();
    }
    // Cerrar el archivo
    fclose(archivo);
    archivo = NULL;
    return 0.0;
    }

double help(void *) {
    printf("BASICO\n");
    printf("======\n");
    printf("> Operaciones aritmeticas básicas: + - / *\n");
    printf("> Funciones matematicas basicas: sin() cos() tan() exp() log()\n");
    printf("> Notas:\n");
    printf(">>> Se admiten espacioes entre expresiones\n");
    printf(">>> Las funciones se escriben en minusculas y requieren parentesis. Ej. cos(23), quit()...\n");
    printf("\n");
    
    printf("OTRAS FUNCIONES:\n");
    printf("================\n");
    printf("> Para evitar la impresion de una operacion usar ';'. Ej. 3+2; no imprime el resultado.\n");
    printf("> workspace(): Imprime las variables con valores asignados\n");
    printf("> clear(\"var\"): Libera la variables var");
    printf("> clean(): Limpia la terminal\n");
    printf("> load(\"file\"): Toma como entrada el archivo de nombre file y ejecuta linea a linea\n");
    printf("> quit(): Termina la ejecucion del interprete\n");
    printf("> import(\"lib-path\"): Importa las funciones de una biblioteca compartida.\n");

    printf("\n");
    printf("ERRORES DETECTADOS:\n");
    printf("===================\n");
    printf("> División por cero\n");
    printf("> Entrada no positiva para el logaritmo\n");
    printf("> Parentesis no cerado. Ej: (3*(4+2)\n");
    printf("> Variable no reconocida. Ej: Si no se realizo una operación de asignación (a=2), la operación a+2 dara error.\n");
    printf("> Intentar asignar valor a una función o constante\n");
    printf("> Funcion no reconocida\n");

    printf("\n");
    printf("EJEMPLO:\n");
    printf("========\n");
    printf("En el fichero ejecutame que puede ser cargado con load() se encuentran ejemplos de comandos validos.\n");

    return 0.0;
}

double quit(void *) {
    if(archivo != NULL) {
        fclose(archivo);
    }
    liberar_bibliotecas();
    destruirTS();
    liberar_flex();
    exit(0);
    return 0.0;
}

double clear(void *lexema) {
    eliminar_valor((char *)lexema);
    return 0.0;
}

double workspace(void *d) {
    (void)d;
    imprimirWorkspace();

    return 0.0;
}

double import(void *d) {
    char *lib = (char *)d;
    incluir_biblioteca(lib);
    return 0.0;

}