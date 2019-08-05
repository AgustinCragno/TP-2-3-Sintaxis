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
    int estados[7];
    char alfabeto[3];  // en el orden de las columnas
    int estadoInicial;
    int estadosFinales[2];
    int tablaDeTransicion[7][3]; //Filas estados, Columnas caracteres
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

    struct AFD automata = {
    .estados = {0,1,2,3,4,5,6},
    .estadoInicial = 0,
    .estadosFinales = {5},
    .alfabeto = {'1', '+', '='},
    .tablaDeTransicion = {  {1, 6, 6},
                            {1, 2, 6},
                            {3, 6, 6},
                            {3, 6, 4},
                            {5, 6, 6},
                            {6, 6, 6},
                            {6, 6, 6} }
    };

    char cadena[] = "1+1=1";

    int pertenece = 9; pertenece = pertenceAlLenguaje(automata, cadena);

    printf("\nPertenece: %d \n", pertenece );

    return 0;
}
