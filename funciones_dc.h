#ifndef FUNC_H
#define FUNC_

#include <stdbool.h>
#include "pila.h"

bool aritmetica(pila_t* pila, int operacion(int a, int b));

int sumar(int a, int b);

int restar(int a, int b);

int multiplicar(int a, int b);

int dividir(int a, int b);

bool ternaria(pila_t* pila);

bool raiz(pila_t* pila);
int _raiz(int elem, int i);

bool potencia(pila_t* pila);
int _pot(int base, int expo);

bool loga(pila_t* pila);
int _log(int arg, int base);

bool es_operador(char* str);

#endif