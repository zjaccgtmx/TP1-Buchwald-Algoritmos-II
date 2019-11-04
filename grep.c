#define _POSIX_C_SOURCE 200809L 
#include "cola.h"
#include "lista.h"
#include "rabinkarp.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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
    
    char linea[90];
    while (fgets(linea,90,archivo) != NULL){
        if (lista_largo(lista) >= n_contexto) lista_borrar_primero(lista);
        lista_insertar_ultimo(lista,strdup(linea));

        if (search_rabinkarp(buscado,linea,37)>=0){
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
    int lineas_previas = atoi(argv[2]);
    if (lineas_previas < 0){
        fprintf(stderr, "ERROR: '%d'. El valor debe ser mayor o igual a 0.\n",lineas_previas);
        return -1;
    }
    return lineas_previas;
}

bool chequeo_parametros(int argc){
    if (argc < 3){
        fprintf(stderr,"ERROR: La cantidad de parámetros es insuficiente.\n");
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
            fprintf(stderr,"ERROR: Archivo '%s' no pudo abrirse.\n",nombre);
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