%{
#include <stdio.h>
#include "TS.h"
#include <math.h>
int yylex();
void yyerror(const char *s);
#include "parser.h"
double buscar_identificador(const char *id);
double evaluar_exp(double izquierda, char operador, double derecha);
int ejecutarFuncion(char *nombre, void *arg);
char mensaje[100];
int contador_parentesis = 0;
%}

%union {
    double numVal;      // Para valores numéricos
    char *idVal;        // Para identificadores (nombres de variables)
}

// Declaración de tokens
%token <numVal> NUM
%token <idVal> ID
%token <idVal> STRING

// Declaración de tipos para no terminales
%type <numVal> expr
%type <numVal> func
%type <numVal> asig
%left '+' '-'
%left '*' '/'
%left UMINUS   // Precedencia alta (para el operador unario negativo)
%nonassoc '='
%%

line: 
    | expr  {if(!isnan($1)){printf("%.5f\n", $1);}}
    | expr ';' 
    | asig  {printf("%.5f\n", $1);}
    | asig ';'
    | func_ind
    ;
expr: func 
    | expr '+' expr   { $$ = evaluar_exp($1, '+', $3); }
    | expr '-' expr   { $$ = evaluar_exp($1, '-', $3); }
    | expr '*' expr   { $$ = evaluar_exp($1, '*', $3); }
    | expr '/' expr   { if($3==0){yyerror("No se puede dividir entre cero");YYERROR;} $$ = evaluar_exp($1, '/', $3); }
    | pi expr pd    { $$ = $2; }  
    | '-' expr %prec UMINUS { $$ = -$2; }  
    | NUM               { $$ = $1; }  
    | ID               { $$ = leer_valor($1); 
                        if(error==NO_VAR) {
                            sprintf(mensaje, "%s no es una variable declarada por el usuario", $1);
                            yyerror(mensaje);
                            YYERROR;
                        }
                        }            
    ;
asig: ID '=' expr {asignar_valor($1, $3); 
                    if(error==NO_VAR) {
                        sprintf(mensaje, "%s es una función o palabra reservada", $1);
                        yyerror(mensaje);
                        YYERROR;
                    }
                    $$=$3;}
    ;
func:  ID pi expr pd   {  Funcion f=leer_funcion($1); 
                            if(!f){ sprintf(mensaje, "%s no se reconoce como funcion", $1);
                                    yyerror(mensaje);
                                    YYERROR;
                            } 
                            $$=f((void *)&$3);}
    ;
func_ind: ID pi STRING pd {if(ejecutarFuncion($1, (void *)$3)){YYERROR;}}
    | ID pi pd {void *s = NULL; if(ejecutarFuncion($1, s)){YYERROR;}}
    ;
pi: '(' {++contador_parentesis;};
pd: ')' {--contador_parentesis;};

%%

int ejecutarFuncion(char *nombre, void *arg) {
    Funcion f = leer_funcion(nombre);
    if(f == NULL) {
        sprintf(mensaje, "no se reconoce como funcion");
        yyerror(mensaje);
        return 1;
    }
    f(arg);
    if(arg!=NULL) {
        free(arg);
    }
    return 0;
}

void yyerror(const char *s) {
    if(contador_parentesis>0){
        fprintf(stderr, "Error: Parentesis sin cerrar\n");
        contador_parentesis = 0;
        return;
    }
    fprintf(stderr, "Error: %s\n", s);
}

double evaluar_exp(double izquierda, char operador, double derecha) {
    switch(operador) {
        case '+': return izquierda + derecha;
        case '-': return izquierda - derecha;
        case '*': return izquierda * derecha;
        case '/': return izquierda / derecha; 
        default: return 0;  
    }
}

