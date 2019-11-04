#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

/* Definición de la estructura de una cola */
struct cola{
    void* primero; // Apunta al primero [encolado] de la cola
    void* ultimo; // Apunta al último [encolado] de la cola
};

/* Definición de la estructura de un nodo */
typedef struct nodo{
    void* dato;
    struct nodo* proximo;
} nodo_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t* cola_crear(void){
    cola_t* cola = malloc(sizeof(cola_t));

    if (cola == NULL){
        return NULL;
    }

    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
    if (cola_esta_vacia(cola)){
        printf("hola\n");
        free(cola);
        return;
    } 
    nodo_t* nodo = cola->primero;
    while (nodo != NULL){
        nodo_t* proximo = nodo->proximo;
        if (destruir_dato != NULL){
            destruir_dato(nodo->dato);
        }
        if(nodo){
            free(nodo);
        }
        nodo = proximo;
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola->primero == NULL;
}

nodo_t* cola_crear_nodo(void* valor);
bool cola_encolar(cola_t *cola, void* valor){
    nodo_t* nodo = cola_crear_nodo(valor);
    if (nodo == NULL){
        return false;
    }

    if (cola_esta_vacia(cola)) {
        cola->primero = nodo;
        cola->ultimo = nodo;
    }
    
    nodo_t* nodo_anteultimo = cola->ultimo;
    nodo_anteultimo->proximo = nodo;
    nodo->proximo = NULL;
    cola->ultimo = nodo;
    return true;
}

void* cola_ver_primero(const cola_t *cola){
    if( cola_esta_vacia(cola)){
        return NULL;
    }
    nodo_t* nodo_primero = cola->primero;
    return nodo_primero->dato;
}

void* cola_desencolar(cola_t *cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }

    nodo_t* primer_nodo = cola->primero;
    nodo_t* nuevo_primero = primer_nodo->proximo;
    cola->primero = nuevo_primero;

    if (cola_esta_vacia(cola)){
        cola->ultimo = NULL;
    }
    void* valor = primer_nodo->dato;
    free(primer_nodo);
    return valor;
}

// Crea un nodo que guarda el dato pasado por parámetro y cuyo próximo es NULL
// Si no puede crearse, devuelve NULL
nodo_t* cola_crear_nodo(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    
    if (nodo != NULL){
        nodo->dato = valor;
        nodo->proximo = NULL;
    }

    return nodo;
}
