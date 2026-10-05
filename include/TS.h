/* ====================================================================
 * Archivo: TS.h
 * Descripción: Descripcion: Define las funciones y estructuras para 
 *              manejar la tabla de simbolos del compilador, incluyendo 
 *              la estructura componente_lexico y las funciones para 
 *              inicializar, buscar, imprimir y destruir la tabla de 
 *              símbolos.
 * ==================================================================== */

#ifndef TS_H
#define TS_H

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/**
 * @brief Componente lexico
 * 
 * Se define la estructura "componente_lexico", que representa cada entrada
 * en la Tabla de Símbolos (TS). Cada componente contiene un identificador del
 * tipo de token y el lexema correspondiente.
 */
// typedef double (*ddfunc)(double);
// typedef void (*vcfunc)(char *);

typedef double (*Funcion)(void *);

typedef enum {
    VAR,    // Variable
    FUNC,    // Función
    NFUNC,  // Funcion solitaria
    NOASIG
} TipoSimbolo;

extern int error;

typedef enum {
    NO_ERROR,
    NO_EXIST,
    NO_VAR, // No es una variable desclarada por el usuario
    NO_FUN,
    TS_ERROR
} errores;

typedef struct {
    int id;
    char *lexema;
    TipoSimbolo tipo;      // Tipo de símbolo (variable o función)
    union {
        double valor;     // Solo válido si es una variable
        Funcion funcion; // Solo válido si es una función
    };
} componente_lexico;


/* ******************************************************************
 *                    PRIMITIVAS DE LA TS
 * *****************************************************************/

/**
 * @brief Inicializa la tabla de simbolos
 * 
 * Configura y reserva los recursos necesarios para el correcto
 * funcionamiento de la tabla de simbolos
 */
void iniciar_TS();

/**
 * @brief Busca un componente lexico a partir del lexema
 * 
 * Esta funcion devuelve el componente lexico asociado a un lexema 
 * exista o no en la tabla. Si existe devuelve el componente almacenado.
 * Si no existe en la tabla de simbolos crea un nuevo componente de
 * tipo identificador a lo almacena asociado al lexema y lo devuelve.
 * 
 * @param lexema Clave para la busqueda del componente
 * @return componente_lexico* Componente lexico almacenado en la tabla
 */
componente_lexico *buscar_TS(char *lexema);

void asignar_valor(char *lexema, double valor);

double leer_valor(char *lexema);

void eliminar_valor(char *lexema);

void asignar_funcion(char *lexema, Funcion func);

Funcion leer_funcion(char *lexema);

/**
 * @brief Imprime el contenido de la tabla de simbolos
 * 
 * Imprime por pantalla todos los componentes lexicos almacenados
 * en la tabla de simbolos.
 * 
 */
void imprimirContenido();

void imprimirWorkspace();

/**
 * @brief Destruye la tabla de simbolos
 * 
 * Se encarga de liberar la memoria y los recursos asignados a la TS.
 */
void destruirTS();

int incluir_biblioteca(const char *nombre);

void *buscar_funcion(const char *nombre);

void liberar_bibliotecas();

#endif