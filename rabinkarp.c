/* Following program is a C implementation of Rabin Karp 
Algorithm given in the CLRS book */
#include<stdio.h> 
#include<string.h> 

// d is the number of characters in the input alphabet 
#define d 256 

/* pat -> pattern 
	txt -> text 
	q -> A prime number 
*/

/****************************************************************************
* Inicialmente, la función devolvía void e imprimía
* las posiciones en la línea en donde aparecía el str buscado.
* Como yo sólo quiero saber si está o no, indiferentemente de la cantidad
* de veces que aparece o la posición, modifiqué para que devuelva la posicion
* de la primer aparición o, en caso de no estar, que devuelva -1.
* De esta manera es la misma función pero puedo utilizarla con un bool por fuera
* para chequear una condición (está / no está).
* Asimismo modifiqué los parámetros de entrada de char var[] a const char* var
* para hacerlo coincidir con los parámetros que uso fuera de esta función.
*****************************************************************************/

/* Devuelve un número mayor o igual a 0 si la palabra buscada está en txt,
sino devuelve -1*/
int search_rabinkarp(const char* pat, const char* txt, int q){ 
    int M = (int) strlen(pat); 
	int N = (int) strlen(txt); 
	int i, j; 
	int p = 0; // hash value for pattern 
	int t = 0; // hash value for txt 
	int h = 1; 

	// The value of h would be "pow(d, M-1)%q" 
	for (i = 0; i < M-1; i++) 
		h = (h*d)%q; 

	// Calculate the hash value of pattern and first 
	// window of text 
	for (i = 0; i < M; i++) 
	{ 
		p = (d*p + pat[i])%q; 
		t = (d*t + txt[i])%q; 
	} 

	// Slide the pattern over text one by one 
	for (i = 0; i <= N - M; i++) 
	{ 

		// Check the hash values of current window of text 
		// and pattern. If the hash values match then only 
		// check for characters on by one 
		if ( p == t ) 
		{ 
			/* Check for characters one by one */
			for (j = 0; j < M; j++) 
			{ 
				if (txt[i+j] != pat[j]) 
					break; 
			} 

			// if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1] 
			if (j == M) 
				return i;
		} 

		// Calculate hash value for next window of text: Remove 
		// leading digit, add trailing digit 
		if ( i < N-M ) 
		{ 
			t = (d*(t - txt[i]*h) + txt[i+M])%q; 

			// We might get negative value of t, converting it 
			// to positive 
			if (t < 0) 
			t = (t + q); 
		} 
	}
	return -1; 
} 

/* Driver program to test above function */
/*int main() 
{ 
	char text[16] = "hola como estas"; 
	char pattern[25] = "com"; 

	const char* txt = (char*)&text;
	const char* pat = (char*)&pattern;
	int q = 101; // A prime number 
	printf("Palabra en oración: %d\n",search_rabinkarp(pat, txt, q) >= 0); 
	return 0; 
}*/
