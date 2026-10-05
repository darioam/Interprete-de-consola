# Intérprete de consola

Implementación de un **intérprete de expresiones matemáticas** desarrollado en C. El proyecto permite introducir expresiones directamente desde una consola, evaluarlas y trabajar con variables y funciones mediante un conjunto de comandos integrados.

El análisis de las expresiones se realiza mediante **Flex** para el análisis léxico y **Bison** para el análisis sintáctico.

## Características

### Operaciones y funciones matemáticas

El intérprete permite realizar las operaciones aritméticas básicas:

* `+` — suma
* `-` — resta
* `*` — multiplicación
* `/` — división

También incluye las siguientes funciones matemáticas:

* `sin()`
* `cos()`
* `tan()`
* `exp()`
* `log()`

Las funciones deben escribirse en minúsculas y requieren paréntesis. Por ejemplo:

```text
cos(23)
sin(1.57)
log(10)
```

Se admiten espacios entre los distintos elementos de una expresión.

### Variables

Es posible asignar valores a variables y utilizarlas posteriormente en otras expresiones:

```text
a = 2
b = 3
a + b
```

El comando `workspace()` permite consultar las variables que tienen actualmente un valor asignado.

También es posible eliminar una variable mediante:

```text
clear("a")
```

### Comandos especiales

| Comando              | Descripción                                       |
| -------------------- | ------------------------------------------------- |
| `help()`             | Muestra la ayuda del intérprete                   |
| `workspace()`        | Muestra las variables definidas y sus valores     |
| `clear("var")`       | Elimina una variable                              |
| `clean()`            | Limpia la terminal                                |
| `load("file")`       | Carga y ejecuta los comandos de un fichero        |
| `import("lib-path")` | Importa funciones desde una biblioteca compartida |
| `quit()`             | Finaliza la ejecución del intérprete              |

### Control de salida

Es posible evitar la impresión del resultado de una expresión utilizando `;` al final:

```text
3 + 2;
```

La expresión se ejecuta, pero su resultado no se muestra por pantalla.

### Carga de ficheros

El comando `load()` permite utilizar un fichero como entrada para el intérprete:

```text
load("ejecutame")
```

Las instrucciones contenidas en el fichero se ejecutan línea a línea.

El repositorio incluye el fichero `ejecutame`, que contiene ejemplos de expresiones y comandos válidos.

### Bibliotecas compartidas

El intérprete permite importar funciones definidas en bibliotecas compartidas mediante:

```text
import("lib-path")
```

Esto permite ampliar las funcionalidades disponibles en el intérprete sin modificar directamente su código principal.

## Detección de errores

El intérprete incorpora detección de distintos errores durante el análisis y evaluación de las expresiones, entre ellos:

* División por cero.
* Argumentos no válidos para `log()`.
* Paréntesis sin cerrar.
* Uso de variables que no han sido definidas.
* Intentos de asignación a funciones o constantes.
* Uso de funciones no reconocidas.

Por ejemplo:

```text
(3 * (4 + 2)
```

produce un error debido al paréntesis sin cerrar.

De igual forma, una expresión como:

```text
a + 2
```

produce un error si previamente no se ha definido la variable `a`.

## Arquitectura

El procesamiento de las expresiones se divide principalmente en los siguientes pasos:

1. Entrada del usuario
2. (Flex) Análisis léxico
3. (Bison) Análisis sintáctico
4. Evaluación de la expresión
5. Resultado

### Flex

**Flex** se utiliza para implementar el analizador léxico. Su función es reconocer los distintos elementos de la entrada, como:

* Números
* Identificadores
* Operadores
* Paréntesis
* Palabras reservadas y comandos
* Cadenas utilizadas como argumentos de determinados comandos

### Bison

**Bison** se utiliza para implementar el analizador sintáctico. A partir de los tokens proporcionados por Flex, comprueba que las expresiones siguen la gramática definida y construye su estructura para poder evaluarlas.

Las reglas gramaticales incorporan las acciones semánticas necesarias para evaluar las expresiones y realizar las operaciones correspondientes.

## Compilación

El proyecto incluye scripts para automatizar la compilación y limpieza de los archivos generados.

Para compilar:

```bash
./compilar.sh
```

El script genera los analizadores correspondientes a Flex y Bison y compila el código fuente del intérprete.

Para eliminar los archivos generados:

```bash
./limpiar.sh
```

## Requisitos

El proyecto fue desarrollado y probado con las siguientes versiones:

| Herramienta       | Versión              |
| ----------------- | -------------------- |
| Sistema operativo | Debian 12 (Bookworm) |
| GCC               | 12.2.0               |
| GNU Make          | 4.3                  |
| Flex              | 2.6.4                |
| Bison             | 3.8.2                |
| Valgrind          | 3.19.0               |

`Valgrind` se utilizó para comprobar posibles errores relacionados con la gestión de memoria.

## Contenido del repositorio

```text
.
├── biblioteca/
│   └── lib.so
├── ejecutame
├── compilar.sh
├── limpiar.sh
└── ...
```

* **`biblioteca/lib.so`** — biblioteca compartida utilizada para probar `import()`.
* **`ejecutame`** — fichero con ejemplos de expresiones y comandos válidos.
* **`compilar.sh`** — script de compilación.
* **`limpiar.sh`** — script para eliminar archivos generados.

## Ejemplo de uso

Una sesión de ejemplo podría ser:

```text
> x = 3
> y = 2 * pi * x
> z = x * sin(y + pi/6)
> workspace()

x = 3
y = ...
z = ...

> cos(0)
1
> 3 + 2
5
> 3 + 2;
> quit()
```

También pueden ejecutarse directamente los ejemplos incluidos en el fichero `ejecutame`:

```text
> load("ejecutame")
```

## Tecnologías utilizadas

* **C** — implementación del intérprete y de las acciones semánticas.
* **Flex** — generación del analizador léxico.
* **Bison** — generación del analizador sintáctico.
* **GNU Make** — automatización de la compilación.
* **Valgrind** — comprobación de errores de memoria.
