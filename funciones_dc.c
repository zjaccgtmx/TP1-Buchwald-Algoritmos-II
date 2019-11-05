#define _POSIX_C_SOURCE 200809L 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "funciones_dc.h"
#include "pila.h"
#include "dc.c"

bool aritmetica(pila_t* pila, int operacion(int a, int b)){
    char* a = pila_desapilar(pila);
    char* b = pila_desapilar(pila);
    if (!a || !b){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (es_operador(a) || es_operador(b)){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (operacion == dividir && atoi(b) == 0){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    return pila_apilar(pila,operacion(atoi(a),atoi(b)));
}

int sumar(int a, int b){
    return a + b;
}

int restar(int a, int b){
    return a - b;
}

int multiplicar(int a, int b){
    return a * b;
}

int dividir(int a, int b){
    return a / b;
}

bool ternaria(pila_t* pila){
    char* a = pila_desapilar(pila);
    char* b = pila_desapilar(pila);
    char* c = pila_desapilar(pila);

    if (!a || !b || !c){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (es_operador(a), es_operador(b), es_operador(c)){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    return pila_apilar(pila, atoi(a) == 0 ? atoi(b) : atoi(c));
}

bool raiz(pila_t* pila){
    char* elem = pila_desapilar(pila);
    if (!elem){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (es_operador(elem)){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (atoi(elem) < 0){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    return pila_apilar(pila,_raiz(stoi(elem),1));
}

int _raiz(int elem, int i){
    if (i * i > elem)   return i - 1;
    return _raiz(elem,i + 1);
}

bool potencia(pila_t* pila){
    char* base = pila_desapilar(pila);
    char* expo = pila_desapilar(pila);
    if (!base || !expo){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (es_operador(base) || es_operador(expo)){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (atoi(expo) < 0){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    return pila_apilar(pila,_pot(atoi(base),atoi(expo)));
}

int _pot(int base, int expo){
    if (expo == 0)  return base;

    int x = base;
    while (expo > 1){
        x = x ** 2;
        expo / 2;
    }
    return x;
}

bool loga(pila_t* pila){
    char* a = pila_desapilar(pila);
    char* b = pila_desapilar(pila);
    if (!a || !b){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (es_operador(a) || es_operador(b)){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    if (atoi(b) <= 1){
        fprintf(stderr,"ERROR\n");
        return false;
    }
    return pila_apilar(pila,_log(atoi(a),atoi(b)));
}

int _log(int arg, int base){
    if (arg / base < 1) return arg;
    return _log(arg / base, base);
}

bool es_operador(char* str){

    bool suma = str[0] == SUMA;
    bool resta = str[0] == RESTA;
    bool multip = str[0] == MULT;
    bool division = str[0] == DIV;
    bool pot = str[0] == POT;
    bool tern = str[0] == TERN;

    char raiz_c[10] = 'sqrt';
    char logaritmo[10] = 'log';

    bool raiz = strcmp(str,(const char*)&raiz_c) == 0;
    bool logs = strcmp(str,(const char*)&logaritmo) == 0;

    return (suma || resta || multip || division || raiz || pot || logs || tern);
}

