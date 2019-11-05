#define _POSIX_C_SOURCE 200809L 
#include "strutil.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#define UN_ELEMENTO 1
#define DOS_ELEMENTOS 2
#define FINAL_DE_CADENA '\0'
#define PRIMER_ELEMENTO 0
#define SEGUNDO_ELEMENTO 1
#define TERCER_ELEMENTO 2

/***********************
 * FUNCIONES AUXILIARES
 * *********************/

/* Crea una cadena a partir de un caracter pasado por parámetro*/
char* crear_cadena_vacia(){
    return strdup("");
}

/* Crea un char** arreglo de tamaño pasado por parámetro.
En caso de error, devuelve NULL.*/
char** crear_arreglo(size_t volumen){
    char** arreglo = malloc((volumen) * sizeof(char*));
    if (!arreglo){
        return NULL;
    }
    return arreglo;
}

/* Crea una cadena dinámica a partir del contenido de un char*.
Post: no se modifica la cadena original.
Se devuelve la nueva cadena y la liberación de la memoria dinámica devuelta queda a cargo de quien llame a
esta función. La función devuelve NULL en caso de error.*/
char* hacer_dinamica(char* cadena){
        size_t tam = strlen(cadena)+1;
        char *cadena_nueva = malloc((tam) * sizeof(char*));
        if (!cadena_nueva){
            return NULL;
        }
        memset(cadena_nueva, '\0', tam);
        strcpy(cadena_nueva,(const char*)cadena);
        return cadena_nueva;
}

/* devuelve true si la cadena está vacía, o de lo contrario, false */
bool cadena_esta_vacia(const char* str){
    const char final = FINAL_DE_CADENA;
    return strcmp(str,&final) == 0;
}

/* Cuenta la cantidad de subcadenas que se crean a
partir de un str y dividiendo por el separador pasado por parámetro*/
size_t contar_subcadenas(const char* str, char sep){
    size_t contador = 0;
    for (size_t i = 0; (char) str[i] != FINAL_DE_CADENA; i++){
        if ( str[i] == sep){
            contador++;
        }
    }
    return contador+1;
}

char* generar_cadena(const char* str,size_t inicio, size_t final){
    const char* char_inicial = str + inicio;
    size_t cant_copias = final - inicio;

    char *subcadena = malloc(sizeof(char) * (cant_copias +1));
    if (!subcadena) return NULL;
    memset(subcadena,FINAL_DE_CADENA,cant_copias +1);
    strncpy(subcadena,char_inicial,cant_copias);
    return subcadena;
}

bool guardar_cadena(char** arr, char* subcadena,size_t ind_arr){
    size_t vol_pos = cadena_esta_vacia(subcadena) ? 1 : strlen(subcadena) +1 ;
    arr[ind_arr] = malloc(sizeof(char) * vol_pos);
    if (!arr[ind_arr]) return false;

    strcpy(arr[ind_arr],(const char*)subcadena);
    return true;
}

/* Genera las subcadenas creadas a partir de un str y un separador pasado
por parámetro en las guarda en un arreglo también pasado por parámetro.
Post: el arreglo se encuentra lleno de subcadenas.*/
bool guardar_subcadenas(char** arr,const char* str,size_t cantidad_subcadenas,char sep){
    size_t inicio = 0;
    size_t final = 0;
    size_t ind_arr = 0;

    for (size_t i = 0; i < strlen(str); i++){
        if (str[i] != sep) continue;

        final = i;
        char *subcadena;

        if (final - inicio < 1){
            subcadena = crear_cadena_vacia();
        } else {
            subcadena = generar_cadena(str, inicio, final);
            inicio = final;
        }
        inicio++;

        if (!guardar_cadena(arr, subcadena, ind_arr)) return false;
        free(subcadena);
        ind_arr++;
    }

    final = strlen(str);
    char *subcadena = generar_cadena(str,inicio,final);
    if (!guardar_cadena(arr,subcadena,ind_arr)){
        free(subcadena);
        return false;
    }
    free(subcadena);
    ind_arr++;
    arr[ind_arr] = NULL;
    return true;
}

/* Cuenta la cantidad de elementos no nulos que 
contiene un arreglo*/
size_t contar_elem(char** strv){
    size_t i = 0;
    
    char* subcadena = strv[PRIMER_ELEMENTO];
    while (subcadena != NULL){
        i++;
        subcadena = strv[i];
    }
    return i;
}

/* Devuelve la cantidad total de caracteres de todas
las cadenas guardadas en el arreglo*/
size_t len_total(char** strv,size_t cant_subcad_arreglo){
    size_t len = 0;
    for (size_t i = 0; i < cant_subcad_arreglo; i++){
        const char* cadena = strv[i];
        len += strlen(cadena);
    }
    return len;
}

/***********************
 * FUNCIONES PEDIDAS
 * *********************/

char *substr(const char *str, size_t n){
    if (cadena_esta_vacia(str)){
        return strdup(str);
    }

    if (n > strlen(str)){
        return strdup(str);
    }

    char* nuevo_str = malloc((n + 1) * sizeof(char));
    if (!nuevo_str) return NULL;
    memset(nuevo_str,FINAL_DE_CADENA,n+1);

    return strncpy(nuevo_str,str,n);
}

char **split(const char *str, char sep){
    if (cadena_esta_vacia(str)){
        char **arreglo = crear_arreglo(2);
        if (!arreglo) return NULL;

        char* cadena = strdup(str);
        arreglo[0] = malloc(sizeof(char) * 2);
        if (!arreglo[0]) return NULL;
        strcpy(arreglo[0],(const char*) cadena);
        arreglo[1] = NULL;
        free(cadena);
        return arreglo;
    }

    size_t cant_subcad = contar_subcadenas(str,sep);
    size_t vol_arreglo = cant_subcad +1;

    char** arreglo = crear_arreglo(vol_arreglo);
    if (!arreglo) return NULL;

    return guardar_subcadenas(arreglo,str,cant_subcad,sep) ? arreglo : NULL;
}

char *join(char **strv, char sep){
    if (strv[0] == NULL) return crear_cadena_vacia();
    size_t cant_elem = contar_elem(strv);
    size_t lentotal_str = len_total(strv,cant_elem);
    size_t cant_sep = sep == FINAL_DE_CADENA ? 0 : cant_elem - 1;
    size_t tam_str = lentotal_str + cant_sep +1;

    char* cad_res = malloc(sizeof(char) * tam_str);
    if (!cad_res) return NULL;
    memset(cad_res,FINAL_DE_CADENA,tam_str);
    
    size_t ind_str = 0;
    for (size_t i = 0; i < cant_elem; i++){
        char* cad_act = strv[i];
        if (i > 0 && sep != FINAL_DE_CADENA){
            cad_res[ind_str] = sep;
            ind_str++;
        }
        if (cadena_esta_vacia(cad_act)) continue;
        strcpy(cad_res + ind_str, cad_act);
        ind_str += strlen(cad_act);
    }
    return cad_res;
}

void free_strv(char *strv[]){

    size_t i = 0;
    while (strv[i]){
        free(strv[i]);
        i++;
    }
    free(strv);
}
