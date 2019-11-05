#define _POSIX_C_SOURCE 200809L 
#include "strutil.h"
#include "pila.h"
#include "dc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "funciones_dc.h"
#define TAM_MAX_LINEA 90
#define TAM_MAX_ELEM 10
#define SUMA '+'
#define RESTA '-'
#define MULT '*'
#define DIV '/'
#define SQRT 'sqrt'
#define POT '^'
#define LOG 'log'
#define TERN '?'

/*
gcc -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
*/




bool _dc(pila_t* pila){
    if (pila_esta_vacia(pila))  return true;
    char* desapilado = pila_desapilar(pila);
    
    if (!_dc(pila)) return false;
    if (!es_operador(desapilado))   pila_apilar(pila,atoi(desapilado));

    char op = desapilado[0];
    switch (op){
        case SUMA:
            return aritmetica(pila,sumar);
        case RESTA:
            return aritmetica(pila,restar);
        case MULT:
            return aritmetica(pila,multiplicar);
        case DIV:
            return aritmetica(pila,dividir);
        case SQRT:
            return raiz(pila);
        case POT:
            return potencia(pila);
        case LOG:
            return loga(pila);
        case TERN:
            return ternaria(pila);
    }
    return false;
}

void dc(pila_t* pila){
    if (pila_esta_vacia(pila))  return;
    if (!_dc(pila)) return;
    int* desap = pila_desapilar(pila);
    if (!pila_esta_vacia(pila)){
        fprintf(stderr,"ERROR\n");
    } else {
        fprintf(stdout,"%d\n",*(int*)desap);
    }
    return;
}

FILE* chequeo_archivo(int argc,char* argv[]){
    FILE* archivo;
    if (argc == 1){
        archivo = stdin;
    } else {
        const char* nombre = argv[1];
        if (!(archivo = fopen(nombre,"r"))){
            fprintf(stderr,"No se pudo leer el archivo indicado\n");
            return NULL;
        }
    }
    return archivo;
}

int main(int argc, char* argv[]){

    FILE* archivo;
    if (!(archivo = chequeo_archivo(argc, argv)))   return -1;
        
    char linea[TAM_MAX_LINEA];
    while(fgets(linea,TAM_MAX_LINEA,archivo) != NULL){
        pila_t* pila = pila_crear();
        char** elems = split(linea," ");
        int i = 0;

        while (elems[i] != NULL){
            pila_apilar(pila,elems[i]);
        }
        dc(pila);
        free(elems);
        pila_destruir(pila);
    }

    if (archivo != stdin)   fclose(archivo);
    return 0;
}