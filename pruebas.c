#define _POSIX_C_SOURCE 200809L 
#include "strutil.h"
#include "testing.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#define CONST0 0
#define CONST1 1
#define CONST2 5
#define CONST_GRANDE 100
#define FINAL_DE_CADENA '\0'
#define SEP1 ','
#define SEP2 ';'

//  gcc -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion -o pruebas *.c
// valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

size_t _contar_subcadenas(const char* str, char sep){
    
    size_t contador = 0;
    for (size_t i = 0; (char) str[i] != FINAL_DE_CADENA; i++){
        if ( str[i] == sep){
            contador++;
        }
    }
    return contador+1;
}

void pruebas_substr(char* cadena){

    char *subcadena = substr((const char*)cadena, CONST2);
    char *subcadena2 = malloc((CONST2+1) * sizeof(char));
    if (!subcadena2){
        printf("Error creando subcadena2 para pruebas\n");
        return;
    }
    subcadena2 = strncpy(subcadena2,(const char*)cadena,CONST2);
    subcadena2[CONST2] = FINAL_DE_CADENA;
    print_test("Substr de cadena no nula",strcmp((const char*)subcadena,(const char*)subcadena2) == 0);
    free((void*)subcadena);
    free((void*)subcadena2);

    // cambio de subcadena
    char *subcadena_b = substr((const char*)cadena+2,CONST2); // "la Mu"
    char *subcadena3 = malloc((CONST2+1) * sizeof(char));
    if (!subcadena3){
        printf("Error creando subcadena2 para pruebas\n");
        return;
    }

    subcadena3 = strncpy(subcadena3,cadena+2,CONST2);
    subcadena3[CONST2] = FINAL_DE_CADENA;
    print_test("Substr de cadena no nula corrido +2 posiciones",strcmp((const char*)subcadena_b,(const char*)subcadena3) == 0);
    free((void*)subcadena_b);
    free((void*)subcadena3);

    // cambio de subcadena
    char *subcadena_c = substr((const char*)cadena,CONST_GRANDE); // "Hola Mundo"
    char *subcadena4 = malloc((strlen((const char*)cadena)+1) * sizeof(char));
    if (!subcadena4){
        printf("Error creando subcadena2 para pruebas\n");
        return;
    }

    subcadena4 = strcpy(subcadena4,(const char*)cadena);
    subcadena4[strlen((const char*)cadena)] = FINAL_DE_CADENA;
    print_test("Substr de cadena no nula con n > len(cadena)",strcmp((const char*)subcadena_c,(const char*)subcadena4) == 0);
    free((void*)subcadena_c);
    free((void*)subcadena4);
}

void pruebas_split_cadena(char* cadena, char sep){

    char** arreglo_a = split((const char*)cadena,sep);
    print_test("Crear arreglo con split",arreglo_a);
    size_t tam_arreglo = _contar_subcadenas((const char*)cadena,sep);
    
    if (sep == FINAL_DE_CADENA){ // sep = '\0'
        print_test("Sep = '/0' devuelve cadena entera en arreglo",strcmp(arreglo_a[0],cadena) == 0);
        free_strv(arreglo_a);
        return;
    }

    if(!sep){
        bool split_ok = true;
        for (size_t i = 0; i<tam_arreglo && split_ok;i++){
            split_ok = (char)arreglo_a[i][0] == (char)cadena[i];
        }
        print_test("sep vacío fragmenta la cadena letra por letra en el arreglo", split_ok);
        free_strv(arreglo_a);
        return;
    }

    size_t inicio = 0;
    size_t final = 0;  
    size_t indice_arreglo = 0;
    char *cadena_comparar = malloc( strlen((const char*)cadena) * sizeof(char));

    for (size_t i = 0; i < strlen((const char*)cadena) ; i++){
        char caracter = cadena[i];
        if (caracter != sep){           // letras
            final++;
            continue;
        }
    
        // caracter = separador
        final = i;
        
        if (final-inicio < 1){   // dos separadores seguidos
            char cad_vacia = FINAL_DE_CADENA;
            cadena_comparar = strcpy(cadena_comparar,(const char*)&cad_vacia);  // ingresar una cadena vacía
            inicio++;

        } else {    // hay texto entre separador y separador :: se agrega al arreglo            
            const char* caracter_a_copiar = cadena + inicio;
            size_t cant_a_copiar = final - inicio;
            cadena_comparar = strncpy(cadena_comparar,(const char*)caracter_a_copiar,cant_a_copiar);
            cadena_comparar[cant_a_copiar] = FINAL_DE_CADENA;
            final++;
            inicio = final;
        }

        const char* cadena_arreglo = (const char*) arreglo_a[indice_arreglo];
        print_test("Split correcto en subcadena de arreglo ", strcmp((const char*)cadena_comparar,cadena_arreglo) == 0 );
        indice_arreglo++;

    }

    final++;
    const char* caracter_a_copiar = cadena + inicio;
    size_t cant_a_copiar = final - inicio;
    cadena_comparar = strncpy(cadena_comparar,(const char*)caracter_a_copiar,cant_a_copiar);
    cadena_comparar[cant_a_copiar] = FINAL_DE_CADENA;
    
    const char* cadena_arreglo = (const char*) arreglo_a[indice_arreglo];
    print_test("Split correcto en subcadena de arreglo ", strcmp((const char*)cadena_comparar,cadena_arreglo) == 0 );
    print_test("Último elemento de la cadena es NULL",arreglo_a[tam_arreglo] == NULL);

    free((void*) cadena_comparar);
    free_strv(arreglo_a);
}

void pruebas_cadenas_simples(){
    printf("\n**PRUEBAS CON CADENAS SIMPLES**\n"); // cadenas simples: 1 elemento o menos.
    
    char cadena_vacia = '\0';
    const char* subcadena = substr((const char*)&cadena_vacia,CONST1);

    print_test("Substr de cadena vacía es una cadena vacía", strcmp(subcadena,(const char*)&cadena_vacia) == 0);    
    free((void*)subcadena);

    char sep = SEP1;

    char** arreglo = split((const char*)&cadena_vacia,sep);
    print_test("Se crea arreglo haciendo SPLIT de cadena vacía",arreglo);
    char *cadena_inicial = (char*)arreglo[0];
    print_test("La cadena inicial del arreglo es una cadena vacía",(char)cadena_inicial[0] == cadena_vacia);

    char** arreglo_nulo = malloc(sizeof(char*));
    print_test("Crear arreglo nulo para pruebas",arreglo_nulo);
    arreglo_nulo[0] = NULL;
    char* cadena_arr_nulo = join(arreglo_nulo,sep);
    print_test("Join de arreglo nulo es una cadena vacía", strcmp((const char*) cadena_arr_nulo, (const char*)&cadena_vacia) == 0);
    free( (void*) cadena_arr_nulo);
    free(arreglo_nulo);

    char* cadena_unida = join(arreglo,sep);
    print_test("Join de arreglo de una (1) cadena vacía es una cadena vacía", strcmp((const char*) cadena_unida, (const char*)&cadena_vacia) == 0);    
    char cadena_sep[2] = ",\0";
    char** arreglo_doble = split((const char*)&cadena_sep,sep);
    char cadena_0 = (char)((const char*)arreglo_doble[0])[0];
    char cadena_1 = (char)((const char*)arreglo_doble[1])[0];
    bool cadena_0_vacia = cadena_0 == cadena_vacia;
    bool cadena_1_vacia = cadena_1 == cadena_vacia;
    print_test("Split de cadena = separador es una lista de dos cadenas vacías\n",cadena_0_vacia && cadena_1_vacia);
    free(cadena_unida);

    char* cadena_unida_arreglo_doble = join(arreglo_doble,sep);
    print_test("Join de un arreglo de dos (2) cadenas vacías es igual al separador", strlen(cadena_unida_arreglo_doble) == 1 && cadena_unida_arreglo_doble[0] == sep);
    
    free_strv(arreglo);
    free( (void*) cadena_unida_arreglo_doble);
    free_strv(arreglo_doble);
    
}

void pruebas_cadenas_mixtas(){
    printf("\n**PRUEBAS CON CADENAS MIXTAS**\n"); // cadenas mixtas: más de un elemento

    printf("*PRUEBAS SUBSTR*\n");
    char texto_substr[11] = "Hola Mundo";
    char* cadena_substr = (char*)&texto_substr;
    pruebas_substr(cadena_substr);
    
    printf("*PRUEBAS SPLIT*\n");
    char texto1_split[12] = "abc,def,ghi\0";
    char* cadena_split = (char*)&texto1_split;
    char sep1 = SEP1;
    pruebas_split_cadena(cadena_split,sep1);

    char texto2_split[9] = "abc,,def\0";
    cadena_split = (char*)&texto2_split;
    pruebas_split_cadena(cadena_split,sep1);
 
    char texto3_split[9] = "abc,def,\0";
    cadena_split = (char*)&texto3_split;
    pruebas_split_cadena(cadena_split,sep1);

    char texto4_split[9] = ",abc,def\0";
    cadena_split = (char*)&texto4_split;
    pruebas_split_cadena(cadena_split,sep1);

    char texto5_split[37] = "esto, en efecto, es una prueba, hola\0";
    cadena_split = (char*)&texto5_split;
    pruebas_split_cadena(cadena_split,sep1);

    char texto6_split[7] = "abcdef\0";
    cadena_split = (char*)&texto6_split;
    char sep2 = FINAL_DE_CADENA;
    pruebas_split_cadena(cadena_split,sep2);

    char texto7_split[16] = "separandoletras\0";
    cadena_split = (char*)&texto7_split;
    char sep3 = (char) 0;
    pruebas_split_cadena(cadena_split,sep3);


    printf("*PRUEBAS JOIN*\n");

    char texto1_join[12] = "abc,def,ghi\0";
    char *cadena_join = (char*)&texto1_join;
    char **strv = split(cadena_join, sep1);
    char union1 = SEP2;
    char *resultado = join(strv, union1);  // "abc;def;ghi"

    char texto1_join_b[12] = "abc;def;ghi\0";

    print_test("Join de cadena no nula",strcmp((const char*)&texto1_join_b,(const char*) resultado) == 0);

    free_strv(strv);
    free((void*)resultado);

    char texto2_join[13] = "abc,def,ghi,\0";
    cadena_join = (char*)&texto2_join;
    char **strv2 = split(cadena_join, sep1);
    char *resultado2 = join(strv2, union1);  // "abc;def;ghi"

    char texto2_join_b[13] = "abc;def;ghi;\0";
    print_test("Join de cadena no nula",strcmp((const char*)&texto2_join_b,(const char*) resultado2) == 0);

    free_strv(strv2);
    free((void*)resultado2);

    char texto3_join[4] = "abc\0";
    cadena_join = (char*)&texto3_join;
    char **strv3 = split(cadena_join, sep2);
    char *resultado3 = join(strv3, sep1);

    char texto3_join_b[4] = "abc\0";

    print_test("Join de cadena no nula",strcmp((const char*)&texto3_join_b,(const char*) resultado3) == 0);

    free_strv(strv3);
    free((void*)resultado3);

    char texto4_join[8] = "abc,def\0";
    cadena_join = (char*)&texto4_join;
    char **strv4 = split(cadena_join, sep1); // [abc, def]
    char *resultado4 = join(strv4, sep2); // "abcdef"

    char texto4_join_b[13] = "abcdef\0";

    print_test("Join de cadena no nula",strcmp((const char*)&texto4_join_b,(const char*) resultado4) == 0);

    free_strv(strv4);
    free((void*)resultado4);


    char texto5_join[5] = ",,,,\0";
    cadena_join = (char*)&texto5_join;
    char **strv5 = split(cadena_join, sep1); // ["","","","","", NULL]
    char sep4 = SEP2;
    char *resultado5 = join(strv5,sep4);  // " ;;;;\0 ""

    char texto5_join_b[13] = ";;;;\0";
    print_test("Join de cadena no nula",strcmp((const char*)&texto5_join_b,(const char*) resultado5) == 0);

    free_strv(strv5);
    free((void*)resultado5);
}


int main(){
    pruebas_cadenas_simples();
    pruebas_cadenas_mixtas();
    return 0;
}