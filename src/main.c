#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "TS.h"
#include "lexer.h"


int main() {
    char input[256];
    /**
     * Primera sección: Inicialización. Se inicializa el sistema de entrada
     * con el archivo correspondiente, la tabla de simbolos y arrancamos
     * el analisis desde el analizador sintactico
     * 
     */

    iniciar_TS();

    printf("Bienvenido al interprete. Introduzca help() para conocer el funcionamiento o quit() para salir.\n");

    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);

        // Poner la entrada en el buffer de Flex
        cargar_linea(input);
        
        // Procesar la expresión línea por línea
        yyparse();
    }

    return 0;
}