//includes y defines
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NUMESTADOS 16
#define NUMCOLS 14
#define TAMLEX 999999
#define TAMNOM 32
//typesdefs
typedef enum{
    ALGORITMO, FIN_ALGORITMO, LEER, ESCRIBIR, PESOS, BARRA, CORCHETE_IZQ, CORCHETE_DER,
    FLECHA, IGUAL, SI, FIN_SI, CERO, VARIABLE, SUMA, RESTA, FDT,
    IDENTIFICADOR, CONST_NUMERICA, CONST_CADENA, ERRORLEXICO
} TOKEN;

typedef struct{
    char identifi[TAMLEX];
    TOKEN t;
} Tabla;

//tabla de simbolos
Tabla TablaDeSimbolos[100] = {{"algoritmo", ALGORITMO},
		 {"finAlgoritmo", FIN_ALGORITMO},
		 {"leer", LEER},
		 {"escribir", ESCRIBIR},
		 {"si", SI},
		 {"finSi", FIN_SI},
		 {"variable", VARIABLE},
		 {"Â¿", 999 }};

char buffer[TAMLEX];
TOKEN tokenActual;
int flagToken = 0;
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
void Condicion(); // no esta hecha
void Identificador();  // no esta hecha

FILE *archFuente;

int main(){
	archFuente = fopen("Ejemplo.txt", "r");
    char nomArchi[TAMNOM];
    int l;
    /**********Se abre el Archivo Fuente*********/
    if ( archFuente == NULL ){
        printf("No se pudo abrir archivo fuente\n");
        return -1;//no pudo abrir archivo
    }
    Objetivo();
    fclose(archFuente);
}
//funciones
void Match(TOKEN t){
	printf("\n\n Intento de Match: %d", t);
    if( t != ProximoToken() ){
    	ErrorSintactico();
	}
}
void ErrorLexico(){
    printf("Error Lexico\n");
}

void ErrorSintactico(){
    printf("\n Error Sintactico");
}


TOKEN ProximoToken(){
    if ( !flagToken ){
        tokenActual = Scanner();
        
        printf("\n Caracter leido: #%s# ", buffer);
        printf("\n Token actual: %d", tokenActual);
        
        if ( tokenActual == ERRORLEXICO){
			ErrorLexico();
			flagToken = 1;
		}
        
        if ( tokenActual == IDENTIFICADOR ){
        	
            BuscarEnTs(buffer, TablaDeSimbolos, &tokenActual);
        }
    }return tokenActual;
}

int BuscarEnTs(char *id, Tabla * TablaDeSimbolos, TOKEN * t){
	printf("\n Entra a Ts");
    /* Determina si un identificador esta en la TS */
    int i = 0;
    while ( strcmp("Â¿", TablaDeSimbolos[i].identifi) ){
		if ( !strcmp(id, TablaDeSimbolos[i].identifi) ){
			*t = TablaDeSimbolos[i].t;
			printf("\n  Sale de TS con: %d", *t);
            return 1;
        }
		i++;
     }
     
     
	 
	 return 0;
}
//Identificador
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
//	char in;
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
				if ( col != 12 ){ //si el carÃ¡cter espureo no es un espacio
 					//ungetc(car, archFuente); // lo retorna al flujo
 					//buffer[i-1] = '\0';
 					retrocederBuffer();
 				}
 				return IDENTIFICADOR;
 			case 4 :
			  	if( col != 11 ) {
 					//ungetc(car, archFuente);
 					//buffer[i-1] = '\0';
 					retrocederBuffer();
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

void retrocederBuffer(){
	printf("	- Retroceder Buffer -	");
	
	int i = sizeBuffer();
	while(i >= 0){
		ungetc(buffer[i], archFuente);
		i--;
	}
}

int estadoFinal(int e){									// No, por ahi con lo que hicimos en el tp anterior??????????????????????????????????????????
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
    if  (isspace(c)) return 12;
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
                Sentencia();
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
	retrocederBuffer();
    switch(t){
        case VARIABLE:
            Declaracion();
        case LEER:
            Lectura();
        case ESCRIBIR:
            Escritura();
        case SI:
            Condicional();
        case CORCHETE_IZQ:
            Operacion();

        default: break;
    }
}

void Declaracion(){
    Match(VARIABLE);
    Identificador();
    Match(IGUAL);
    Asignacion();
}

void Asignacion(){
    TOKEN t = ProximoToken();
    if(t == CONST_NUMERICA || t == CONST_CADENA){
        Match(t);
    }
    //Error Sintac tambien
}

void Lectura(){printf("asdasdasd");
    Match(LEER);
    Match(FLECHA);
    Match(IDENTIFICADOR);
    //Identificador();
}

void Escritura(){
    Resultado();
    Match(FLECHA);
    Match(ESCRIBIR);
}

void Resultado(){
    TOKEN t = ProximoToken();
    if(t == IDENTIFICADOR){
        Match(t);
    }else if(t == CORCHETE_IZQ){
        Operacion();
    }
    //En Micro tira lo de erro sintactico
}

void Condicional(){
    Match(SI);
    Match(CORCHETE_IZQ);
    Condicion();
    Match(CORCHETE_DER);
    Match(PESOS);
    Sentencias();
    Match(PESOS);
    Match(FIN_SI);
}

void Condicion(){

}
void Identificador(){

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
    }
    //En Micro aca hace un else con Error Sintactico
}
