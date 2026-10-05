%{
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include "TS.h"
#include "definitions.h"
#include "lexer.h"
#include "parser.h"

int buscar_tabla();
YY_BUFFER_STATE buf_state = NULL;
%}

%option noyywrap
%option noinput
%option nounput

letter  [a-zA-Z_]
digit   [0-9]

identifier {letter}({letter}|{digit})*

int_lit     {decimal_lit}
decimal_digits {digit}(_?{digit})*
decimal_lit (0|[1-9])_?{decimal_digits}?

raw_string_lit  `([^`]|\\`)*`
interpreted_string_lit  \"([^"\\]|\\.)*\"
string_lit  {raw_string_lit}|{interpreted_string_lit}

decimal_exponent    [eE][+-]?{decimal_digits}
decimal_float_lit  {decimal_digits}\.{decimal_digits}?{decimal_exponent}?|{decimal_digits}{decimal_exponent}|\.{decimal_digits}{decimal_exponent}? 

operadores  [-+\*\/\(\)=;]

%%

{decimal_lit}        { yylval.numVal = strtod(yytext, NULL); return NUM; }
{decimal_float_lit}      { printf("INT2\n"); return NUM; }

{string_lit}  {char* lexema = malloc((yyleng-1)*sizeof(char));strncpy(lexema, yytext+1, yyleng-2);lexema[yyleng-2]='\0';yylval.idVal = lexema;return STRING;}
{identifier} { buscar_tabla(); return ID;}

{operadores} {return yytext[0];}

<<EOF>> { return 0; }

[ \t\n]     ;


.           {printf("NO RECONOCIDO\n");}

%%

int buscar_tabla() {
    char* lexema = malloc((yyleng + 1)*sizeof(char));
    strcpy(lexema, yytext);
    componente_lexico *comp = buscar_TS(lexema); 
    int id = comp->id;
    
    yylval.idVal = comp->lexema;
    return id;
}

void set_input_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return;
    }
    yyin = file;  
}

void cargar_linea(char *buffer) {
    yy_delete_buffer(buf_state);
    buf_state = yy_scan_string(buffer);
}

void liberar_flex() {
    yy_delete_buffer(buf_state);
    yylex_destroy();
}