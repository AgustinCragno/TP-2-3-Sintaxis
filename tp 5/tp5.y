%{
	#include<stdio.h>
	#include<conio.h>
	void yyerror(const char *);
%}

%token ALGORITMO
%token FINALGORITMO
%token VARIABLE
%token ESCRIBIR
%token LEER
%token SI
%token FINSI
%token ID
%token NUM
%token OPERADOR

%% 

programa : ALGORITMO '$' sentencias '$' FINALGORITMO
	 ;
sentencias :  sentencia sentencias
		| sentencia 
		;
sentencia : oracion  '/'
oracion : declaracion
	  | operacion
	  | lectura
	  | escritura
	  | condicional
	  ;
declaracion : VARIABLE ID '=' NUM
	  | VARIABLE ID
	;
operacion: '[' ID OPERADOR ID ']'
	;
lectura : LEER '>' ID
       ;
escritura : resultado '>' ESCRIBIR
       ;
resultado : ID
	  | operacion
       ;
condicional : SI '[' resultado ']' '$' sentencias '$' FINSI
       ;
%%

int main(void)
{
	
	if(yyparse()==0)
		printf("Compilacion exitosa\n");
	return 0;
}

void yyerror(const char * s)
{
	puts(s);
	return ;
}
