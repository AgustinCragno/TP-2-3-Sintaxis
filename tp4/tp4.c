//includes y defines
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUMESTADOS 16
#define NUMCOLS 14
#define TAMLEX 999999

//typesdefs
typedef enum{
    ALGORITMO, FIN_ALGORITMO, LEER, ESCRIBIR, PESOS, BARRA, CORCHETE_IZQ, CORCHETE_DER,
    FLECHA, IGUAL, SI, FIN_SI, VARIABLE, SUMA, CERO, RESTA, FDT,
    IDENTIFICADOR, CONST_NUMERICA, ERRORLEXICO
} TOKEN;

typedef struct{
    char identifi[TAMLEX];
    TOKEN t;
} Tabla;

//tabla de simbolos
Tabla TablaDeSimbolos[100] =
         {{"algoritmo", ALGORITMO},
		 {"finAlgoritmo", FIN_ALGORITMO},
		 {"leer", LEER},
		 {"escribir", ESCRIBIR},
		 {"si", SI},
		 {"finSi", FIN_SI},
		 {"variable", VARIABLE},
		 {"¿", 999 }}; //centinela

char buffer[TAMLEX];
TOKEN tokenActual;
int flagToken = 0; //esta variable la usamos para cuando usamos ProximoToken() fuera de Match() ya que necesitamos el token dos veces y el scanner quedaria un token adelantado
FILE *archFuente;
//Prototipos

void Match(TOKEN t);
TOKEN ProximoToken();
int BuscarEnTs(char * id, Tabla * TablaDeSimbolos, TOKEN * t);
TOKEN Scanner();
int estadoFinal(int e);
int columna(int c);
void ErrorSintactico();
void Objetivo();
void Programa();
void Sentencia();
void Sentencias();
void Oracion();
void Declaracion();
void Asignacion();
void Lectura();
void Escritura();
void Resultado();
void Condicional();
void Operacion();
void Operador();
void ErrorLexico();

int main(){
	archFuente = fopen("Ejemplo.txt", "r");
    if ( archFuente == NULL ){
        printf("No se pudo abrir archivo fuente\n");
        return -1;//no pudo abrir archivo
    }
    Objetivo();
    fclose(archFuente);
    printf("\n Compilacion exitosa, vuelva prontos");
}
//funciones
void Match(TOKEN t){
	int prox = ProximoToken();
    if( !(t == prox) )ErrorSintactico();
    	flagToken = 0;
}

void ErrorLexico(){
    printf("\n Error Lexico \n");
    exit(-1);
}

void ErrorSintactico(){
    printf("\n Error Sintactico \n");
    exit(-1);
}

TOKEN ProximoToken(){
    if ( !flagToken ){
        tokenActual = Scanner();
        if ( tokenActual == ERRORLEXICO)ErrorLexico();
        flagToken = 1;

        if ( tokenActual == IDENTIFICADOR ){
            BuscarEnTs(buffer, TablaDeSimbolos, &tokenActual);
        }
    }
    return tokenActual;
}

int BuscarEnTs(char *id, Tabla * TablaDeSimbolos, TOKEN * t){
    // Determina si un identificador esta en la Tabla de simbolos
    int i = 0;
    while ( strcmp("¿", TablaDeSimbolos[i].identifi) ){
		if ( !strcmp(id, TablaDeSimbolos[i].identifi) ){
			*t = TablaDeSimbolos[i].t;
            return 1;
        }
		i++;
     }
	 return 0;
}

TOKEN Scanner(){
	int tabla[NUMESTADOS][NUMCOLS] = {
//   0   1  2  3  4  5  6   7  8  9  10 11 12 13
//   L   D  +  -  [  ]  /   $  >  =  0 fdt sp otro
 	{ 1, 3, 5, 6, 7, 8, 9, 10,11,12,13,14,0,15},
	{ 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{ 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	{15,15,15,15,15,15,15,15,15,15,15,15,15,15},
	};
	int car;
	int col;
	int estado = 0;
	int i = 0;
	do {
 		car = fgetc(archFuente);
 		col = columna(car);
 		estado = tabla[estado][col];
 		if ( col != 12 ) { //si es espacio no lo agrega al buffer
 			buffer[i] = car;
 			i++;
 		}
	}while ( !estadoFinal(estado) && !(estado == 15) );
		buffer[i] = '\0'; //completa la cadena
		switch ( estado ){
 			case 2 : // si es ID
				if ( col != 12 ){ //si el caraceter centinela no es un espacio
 					ungetc(car, archFuente); // lo retorna al flujo
 					buffer[i-1] = '\0';
 				}
 				return IDENTIFICADOR;
 			case 4 :
			  	if( col != 11 ) {
 					ungetc(car, archFuente);
 					buffer[i-1] = '\0';
 				}
 				return CONST_NUMERICA;
 			case 5 : return SUMA;
 			case 6 : return RESTA;
 			case 7 : return CORCHETE_IZQ;
 			case 8 : return CORCHETE_DER;
 			case 9 : return BARRA;
  			case 10 : return PESOS;
  			case 11 : return FLECHA;
  			case 12 : return IGUAL;
  			case 13 : return CERO;
  			case 14 : return FDT;
  			case 15 : return ERRORLEXICO;
 		}
  return 0;
}

int sizeBuffer(){
	int i = 0;
	while(buffer[i] != '\0'){
		i++;
	}
	return i;
}

int estadoFinal(int e){
    if ( e == 0 || e == 1 || e == 3 || e == 15 )
        return 0;
    return 1;
}

int columna(int c){
    if (isalpha(c))return 0;
    if (isdigit(c))return 1;
    if (c == '+') return 2;
    if (c == '-') return 3;
    if (c == '[') return 4;
    if (c == ']') return 5;
    if (c == '/') return 6;
    if (c == '$') return 7;
    if (c == '>') return 8;
    if (c == '=') return 9;
    if  ( c == '0') return 10;
    if  ( c == EOF) return 11;
    if  (isspace(c) || c == '\n' || c == '\t') return 12;
    return 13;
}

void Objetivo(){
    Programa();
    Match(FDT);
}

void Programa(){
    Match(ALGORITMO);
    Match(PESOS);
    Sentencias();
    Match(PESOS);
    Match(FIN_ALGORITMO);
}

void Sentencias(){
    Sentencia();
    while(1){
        switch(ProximoToken()){
            case VARIABLE:
            case LEER:
            case ESCRIBIR:
            case SI:
            case IDENTIFICADOR:
            case CORCHETE_IZQ:
                Sentencia();
                break;
            default:
                return;
        }
    }
}
void Sentencia(){
    Oracion();
    Match(BARRA);
}

void Oracion(){
	TOKEN t = ProximoToken();
    switch(t){
        case VARIABLE:
            Declaracion();
            break;
        case LEER:
            Lectura();
            break;
        case ESCRIBIR:
            Escritura();
            break;
        case SI:
            Condicional();
            break;
        case CORCHETE_IZQ:
            Operacion();
            break;
        default:
            break;
    }
}

void Declaracion(){
    Match(VARIABLE);
    Match(IDENTIFICADOR);
    Match(IGUAL);
    Asignacion();
}

void Asignacion(){
    TOKEN t = ProximoToken();
    if(t == CONST_NUMERICA){
        Match(t);
    }
}

void Lectura(){
    Match(LEER);
    Match(FLECHA);
    Resultado();
}

void Escritura(){
    Match(ESCRIBIR);
    Match(FLECHA);
    Resultado();
}

void Resultado(){
    TOKEN t = ProximoToken();
    if(t == IDENTIFICADOR){
        Match(t);
    }else if(t == CORCHETE_IZQ){
        Operacion();
    }
}

void Condicional(){
    Match(SI);
    Match(CORCHETE_IZQ);
    Resultado();
    Match(CORCHETE_DER);
    Match(PESOS);
    Sentencias();
    Match(PESOS);
    Match(FIN_SI);
}

void Operacion(){
    Match(CORCHETE_IZQ);
    Match(IDENTIFICADOR);
    Operador();
    Match(IDENTIFICADOR);
    Match(CORCHETE_DER);
}

void Operador(){
    TOKEN t = ProximoToken();
    if(t == SUMA || t == RESTA){
        Match(t);
    } else {
      ErrorSintactico(); //revisar
    }
}
