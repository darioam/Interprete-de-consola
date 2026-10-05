#ifndef LEXER_H
#define LEXER_H

extern char *yytext;
extern int yyleng;

// Declaración de la función yylex generada por Flex
int yylex();

void yyerror(const char *s);

// Declaración de la función para cambiar el archivo de entrada
void set_input_file(const char *filename);

void cargar_linea(char *buffer);

int yylex_destroy();

void liberar_flex();

#endif