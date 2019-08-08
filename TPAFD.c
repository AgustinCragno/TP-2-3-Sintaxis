//AFD
// 1. Array de estados
// 2. Array para alfabeto
// 3. Estado inicial
// 4. Array de estados finales
// 5. Matriz para la TT


//Algoritmo 1
// Caracter a analizar (empieza siendo el primero)
// Estado actual (empieza siendo el inicial)
//
// Loop mientras haya caracteres
//      Meto caracter en tabla y actualizo el estado actual
//      Actualizo al proximo caracter
//
// Al terminar:
// Si estado actual es estado final entonces pertenece

#include <stdio.h>

struct AFD{
    int estados[10];
    char alfabeto[10];  // en el orden de las columnas
    int estadoInicial;
    int estadosFinales[1];
    int tablaDeTransicion[10][10]; //Filas estados, Columnas caracteres
};

int length(int array[]){
    return (int)( sizeof(array) / sizeof(int));
}

int lengthCadena(char array[]){
    int i = 0;
    while(array[i] != '\0'){
        i++;
    }
    return i;
}

int estadoEsFinal(struct AFD automata, int estado){

    for(int i = 0; i < length(automata.estadosFinales) ; i++){
        if( estado == automata.estadosFinales[i] ) return 1;
    }

    return 0;
}

int columnaDelCaracter(char caracter, struct AFD automata){
    int i = 0;
    while(automata.alfabeto[i] != '\0'){

        if( caracter == automata.alfabeto[i] ){
          return i;
        }
        i++;
    }
    return -1;
}

int pertenceAlLenguaje(struct AFD automata, char cadena[])
{
    int caracterActual;
    int estadoActual = automata.estadoInicial;

    for(int i = 0 ; i < lengthCadena(cadena) ; i++){

        caracterActual = columnaDelCaracter(cadena[i], automata);printf("Caracter actual: %d\n", caracterActual);
        if(caracterActual < 0) return 0;

        estadoActual = automata.tablaDeTransicion[estadoActual][caracterActual];
    }

    return estadoEsFinal(automata, estadoActual);
}



int main(){

    struct AFD token = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"palabraReservada", "identificador", "constante", "operador", "cadena"},
    .tablaDeTransicion = { {1, 1, 1, 1, 1},
                           {2, 2, 2, 2, 2},
                           {2, 2, 2, 2, 2} }
    };

    struct AFD palabraReservada = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"algoritmo","leer","escribir","variable","finAlgoritmo","si","finSi"},
    .tablaDeTransicion = { {1, 1, 1, 1, 1, 1, 1},
                           {2, 2, 2, 2, 2, 2, 2},
                           {2, 2, 2, 2, 2, 2, 2} }
    };

    struct AFD identificador = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"caracter","constante"},
    .tablaDeTransicion = { {1, 2},
                           {2, 1},
                           {2, 2} }
    };

    struct AFD constante = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"numerica","caracter"},
    .tablaDeTransicion = { {1, 1},
                           {2, 2},
                           {2, 2} }
    };

    struct AFD numerica = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"0","1","2","3"},
    .tablaDeTransicion = { {1, 1, 1, 1},
                           {2, 2, 2, 2},
                           {2, 2, 2, 2} }
    };

    struct AFD caracter = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"a","b","c","d"},
    .tablaDeTransicion = { {1, 1, 1, 1},
                           {2, 2, 2, 2},
                           {2, 2, 2, 2} }
    };

    struct AFD cadena = {
    .estados = {0, 1, 2, 3, 4},
    .estadoInicial = 0;
    .estadosFinales = {3},
    .alfabeto = {"\"","caracteres"},
    .tablaDeTransicion = { {1, 4},
                           {4, 2},
                           {3, 4},
                           {4, 4},
                           {4, 4} }
    };

    struct AFD caracteres = {
    .estados = {0, 1},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"constante"},
    .tablaDeTransicion = { {1},
                           {1} }
    };

    struct AFD numeros = {
    .estados = {0, 1},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"numerica"},
    .tablaDeTransicion = { {1},
                           {1} }
    };

    struct AFD operador = {
    .estados = {0, 1, 2},
    .estadoInicial = 0;
    .estadosFinales = {1},
    .alfabeto = {"+","-"},
    .tablaDeTransicion = { {1, 1},
                           {2, 2},
                           {2, 2} }
    };

    return 0;
}
