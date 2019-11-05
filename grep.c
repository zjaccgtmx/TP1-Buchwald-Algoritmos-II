#define _POSIX_C_SOURCE 200809L 
#include "cola.h"
#include "lista.h"
#include "grep.h"
#include "rabinkarp.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define TAM_MAX 90
#define NUM_PRIMO 37

/*
gcc -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
*/

void grep(FILE* archivo, const char* buscado,int n_contexto){
    lista_t* lista = lista_crear();
    cola_t* cola = cola_crear();
    if (!lista || !cola){
        fprintf(stderr,"ERROR en grep.\n");
        return;
    }
    char linea[TAM_MAX];
    while (fgets(linea,TAM_MAX,archivo) != NULL){
        if (lista_largo(lista) > n_contexto){      // Si supera el número pedido de lineas de contexto,
            lista_borrar_primero(lista); 
        }
        char* guardar = strdup((const char*)&linea);
        lista_insertar_ultimo(lista,guardar);

        if (search_rabinkarp(buscado,linea,NUM_PRIMO)>=0){
            while (!lista_esta_vacia(lista)){
                
                cola_encolar(cola,lista_borrar_primero(lista));
            }
        }
    }
    while (!cola_esta_vacia(cola)){
        char* linea_cola = cola_desencolar(cola);
        fputs(linea_cola,stdout);
        free(linea_cola);
    }
}

int chequeo_contexto(char* argv[]){
    if (*argv[2] == '0'){ 
        return atoi(argv[2]);
    }
    char* str;
    int lineas_previas = (int) strtol(argv[2],&str,10); // 10: Base Decimal
    if (lineas_previas == 0){
        fprintf(stderr, "Tipo de parametro incorrecto\n");
        return -1;
    }
    return lineas_previas;
}

bool chequeo_parametros(int argc){
    if (argc < 3){
        fprintf(stderr,"Cantidad de parametros erronea\n");
        return false;
    }
    return true;
}

FILE* chequeo_archivo(int argc,char* argv[]){
    FILE* archivo;
    if (argc == 3){
        archivo = stdin;
    } else {
        const char* nombre = argv[3];
        if (!(archivo = fopen(nombre,"r"))){
            fprintf(stderr,"No se pudo leer el archivo indicado\n");
            return NULL;
        }
    }
    return archivo;
}

int main(int argc, char* argv[]){

    if (!chequeo_parametros(argc))  return -1;
    FILE* archivo;
    if (!(archivo = chequeo_archivo(argc, argv)))   return -1;
    
    const char* buscado = argv[1];
    int lineas_previas;
    if ((lineas_previas = chequeo_contexto(argv)) < 0)    return -1;
    
    grep(archivo,buscado,lineas_previas);

    if (archivo != stdin)   fclose(archivo);
    return 0;
}