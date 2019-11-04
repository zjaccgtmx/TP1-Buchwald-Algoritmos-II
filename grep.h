#ifndef GREP_H
#define GREP_H

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
void grep(FILE* archivo, const char* buscado,int n_contexto);

int chequeo_contexto(char* argv[]);

bool chequeo_parametros(int argc);

FILE* chequeo_archivo(int argc,char* argv[]);

int main(int argc, char* argv[]);

#endif