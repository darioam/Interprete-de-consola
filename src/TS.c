#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "TS.h"
#include "hash.h"
#include "funciones.h"
#include "definitions.h"
#include "parser.h"
#include <math.h>
#include <dlfcn.h>

// Array con los componentes lexicos de las palabras reservadas
componente_lexico palabras_reservadas[] = {
    {PI, "PI", VAR, .valor=3.14159},
    {E, "E", VAR, .valor=2.71828},
    {SIN, "sin", FUNC, .funcion=vsin},
    {COS, "cos", FUNC, .funcion=vcos},
    {TAN, "tan", FUNC, .funcion=vtan},
    {EXP, "exp", FUNC, .funcion=vexp},
    {LOG, "log", FUNC, .funcion=vlog},
    {FUNCION, "clear", FUNC, .funcion=clear},
    {FUNCION, "load", FUNC, .funcion=load},
    {FUNCION, "help", FUNC, .funcion=help},
    {FUNCION, "quit", FUNC, .funcion=quit},
    {FUNCION, "clean", FUNC, .funcion=clean},
    {FUNCION, "workspace", FUNC, .funcion=workspace},
    {FUNCION, "import", FUNC, .funcion=import}
    };

int error = NO_ERROR;

// Número de palabras reservadas
#define NUM_COMPONENTES (sizeof(palabras_reservadas) / sizeof(componente_lexico))

// Estructura de datos hash de la tabla de simbolos
static hash_t *tabla_simbolos = NULL;

/**
 * @brief Funcion para liberar un componente lexico de la tabla de simbolos
 * 
 * Esta funcion es empleada por la estructura hash para saber como liberar 
 * la memoria de los componentes que contiene cuando es destruida
 * 
 * @param dato Elemento de la tabla hash
 */
void destruir_componente_lexico(void *dato)
{
    if(dato == NULL) return; // Si apunta a Null devolvemos

    componente_lexico *comp = (componente_lexico *)dato; // dato es un componente lexico

    if(comp->id != ID) return; // No liberamos las palabras reservadas pues son memoria estatica

    free(comp->lexema); // Liberar la cadena lexema
    free(comp);         // Liberar el struct

}

/**
 * @brief Inicializa la tabla de simbolos
 * 
 * Configura y reserva los recursos necesarios para el correcto
 * funcionamiento de la tabla de simbolos
 */
void iniciar_TS()
{
    // Si la tabla de símbolos ya existe, la destruimos antes de crear una nueva
    if (tabla_simbolos)
    {
        hash_destruir(tabla_simbolos);
    }

    // Crear una nueva tabla hash con la función de destrucción
    tabla_simbolos = hash_crear(destruir_componente_lexico);

    // Guardar las palabras reservadas en la tabla
    for (int i = 0; i < (int) NUM_COMPONENTES; i++)
    {
        hash_guardar(tabla_simbolos, palabras_reservadas[i].lexema, (void *)&palabras_reservadas[i]);
    }
}

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
componente_lexico *buscar_TS(char *lexema)
{
    // Intentar buscar el lexema en la tabla de símbolos
    componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, lexema);

    // Si encontramos el componente lo liberamos
    if (comp != NULL) {
        free(lexema); // El lexema original ya no se emplea
        return comp;
    }
    // Crear un nuevo componente léxico
    comp = malloc(sizeof(componente_lexico));

    Funcion f = (Funcion)buscar_funcion(lexema);
    comp->id = ID;                
    comp->lexema = lexema;
    if(f != NULL) {
        comp->tipo=FUNC;
        comp->funcion=f;
        hash_guardar(tabla_simbolos, comp->lexema, comp);
        return comp;
    }

    // Asignar valores predeterminados
    comp->id = ID;                
    comp->lexema = lexema;
    comp->tipo = NOASIG;

    // Guardar el nuevo componente
    hash_guardar(tabla_simbolos, comp->lexema, comp);

    // Devolver el nuevo componente
    return comp;
}

void asignar_valor(char *lexema, double valor) {
    error = NO_ERROR;
    componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, lexema);
    // Si encontramos el componente lo liberamos
    if (comp == NULL) {
        error = NO_EXIST;
        return;
    }

    //comp->tipo != NOASIG && comp->tipo != VAR
    if (comp->id!=ID) {
        error = NO_VAR;
        return;
    }

    comp->tipo = VAR;
    comp->valor = valor;
    
}

double leer_valor(char *lexema) {
    error=NO_ERROR;
    componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, lexema);

    if (comp == NULL) {
        error=NO_EXIST;
        return 0 ;
    }

    if (comp->tipo != VAR) {
        error = NO_VAR;
        return 0;
    }

    return comp->valor;
}

void eliminar_valor(char *lexema) {
    error=NO_ERROR;
    componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, lexema);

    if(comp == NULL) {
        error=NO_EXIST;
        return;
    }
    if(comp->id != ID) {
        error=NO_VAR;
        return;
    }

    hash_borrar(tabla_simbolos, lexema);
    destruir_componente_lexico((void *) comp);
}

void asignar_funcion(char *lexema, Funcion func) {
    error=NO_ERROR;
    componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, lexema);

    if (comp == NULL) {
        error=TS_ERROR;
        return;
    }

    if (comp->tipo != NOASIG) {
        error=NO_FUN;
        return;
    }

    comp->tipo = FUNC;
    comp->funcion = func;
}

Funcion leer_funcion(char *lexema) {
    error=NO_ERROR;
    componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, lexema);

    
    if (comp == NULL) {
        error=NO_EXIST;
        return NULL;
    }
    
    if (comp->tipo != FUNC) {
        error=NO_FUN;
        return NULL;
    }

    
    return comp->funcion;
}

/**
 * @brief Imprime el contenido de la tabla de simbolos
 * 
 * Imprime por pantalla todos los componentes lexicos almacenados
 * en la tabla de simbolos.
 * 
 */
void imprimirContenido() {
    hash_iter_t *iter = hash_iter_crear(tabla_simbolos); // Crear el iterador

    if(iter == NULL) { // Comprobar la creacion del iterador
        return;
    }

    /**
     * Inicio de la impresion del contenido de la tabla de simboloes
     */
    int contador = 1;
    do{
        const char *clave = hash_iter_ver_actual(iter);
        componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, clave);
        printf("Elemento - %3d || Lexema: %-15s, ID_COMPONENTE: %d\n", contador, comp->lexema, comp->id);
        contador++;
    }while( hash_iter_avanzar(iter));
    /**
     * Fin de la impresion del contenido de la tabla de simboloes
     */

    hash_iter_destruir(iter); // Liberar iterador
}

void imprimirWorkspace() {
    hash_iter_t *iter = hash_iter_crear(tabla_simbolos); // Crear el iterador

    if(iter == NULL) { // Comprobar la creacion del iterador
        return;
    }


    printf("ESPACIO DE TRABAJO\n");
    printf("==================\n");
    int contador = 1;
    do{
        const char *clave = hash_iter_ver_actual(iter);
        componente_lexico *comp = (componente_lexico *)hash_obtener(tabla_simbolos, clave);
        if(comp->id==ID && comp->tipo==VAR) {
            printf("%3d || Variable: %-10s, valor: %.5f\n", contador, comp->lexema, comp->valor);
            contador++;
        }
    }while( hash_iter_avanzar(iter));


    if(contador == 1) {
        printf("Espacio de trabajo vacio.\n");
    }

    hash_iter_destruir(iter); // Liberar iterador
}

/**
 * @brief Destruye la tabla de simbolos
 * 
 * Se encarga de liberar la memoria y los recursos asignados a la TS.
 */
void destruirTS() {
    hash_destruir(tabla_simbolos);
}

// Estructura para almacenar las bibliotecas cargadas
typedef struct {
    void **handles;
    int cantidad;
} GestorBibliotecas;

GestorBibliotecas gestor = {NULL, 0};

// Función para importar una biblioteca dinámica
int incluir_biblioteca(const char *nombre) {
    void *handle = dlopen(nombre, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error al cargar %s: %s\n", nombre, dlerror());
        return 0;
    }

    // Redimensionar el array dinámicamente
    gestor.handles = realloc(gestor.handles, (gestor.cantidad + 1) * sizeof(void *));
    if (!gestor.handles) {
        perror("Error al redimensionar memoria");
        dlclose(handle);
        return 0;
    }

    gestor.handles[gestor.cantidad++] = handle;
    printf("Biblioteca %s importada correctamente.\n", nombre);
    return 1;
}

// Función para buscar una función en las bibliotecas cargadas
void *buscar_funcion(const char *nombre) {
    for (int i = 0; i < gestor.cantidad; i++) {
        void *func = dlsym(gestor.handles[i], nombre);
        if (func) {
            return func;
        }
    }
    return NULL;  // No se encontró la función
}

//Función para liberar todas las bibliotecas cargadas
void liberar_bibliotecas() {
    for (int i = 0; i < gestor.cantidad; i++) {
        if (gestor.handles[i]) {
            dlclose(gestor.handles[i]);
        }
    }
    free(gestor.handles);
    gestor.handles = NULL;
    gestor.cantidad = 0;
    printf("Todas las bibliotecas han sido liberadas.\n");
}