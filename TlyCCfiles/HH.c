#include <jni.h>
#include <stdio.h>
#include "Choose.h"
#include "ProteccionEncriptacion.h"
#include <stdlib.h>
#include <math.h>
#include<time.h>
#define TRUE 1
#define FALSE 0





int longitudDeArchivo(FILE *fp)
{
	int sz = 0;
	if(fp != NULL)
	{
	    fseek(fp, 0L, SEEK_END);
        sz = ftell(fp);

	}
	rewind(fp);
	return sz;
}



int pertenece (int valor, int arreglo[], int tamano)
{
	int i, exito = FALSE;
	for (i=0; i<tamano; i++)
	{
		if (valor == arreglo[i])
		{
			exito = TRUE;
		}
	}
	return exito;
}


void vaciar(int sobrante[], int tamano){
	int i;
	for(i = 0; i < tamano; i++, sobrante++)
	{
		(*sobrante) = 2;
	}
}



void hammining (char *caracteres, int final, int tamano, int cantControl)
 {
	int control[tamano-cantControl][cantControl];
	int sobrante[8], indiceSobrante = 0;
	int posControl[16] = {0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095,8191,16383,32767};
	int j, h, i;
	int *modulo, *startModulo;
	char ascii = 0;
	FILE *ham, *hem;
	char *startCaracteres;

	vaciar(sobrante, 8);

	modulo = malloc(tamano*sizeof(int));

	int c, f;
	int indice = 0, avance = 1, siONo = FALSE;
	int inicio = TRUE;


	for (c = 0; c < cantControl; c++) //Con esto obtenemos la matriz para hamminizar
 	{
		if(c == 0) siONo = TRUE;
		else siONo = FALSE;

		for (f = 0; f < tamano;)
		{
			if(inicio == TRUE && c != 0)
			{
				j = 1;
			}
			else
			{
				j = 0;
			}
			inicio = FALSE;

			for(; j < avance && f < tamano; j++, f++)
			{
				if(siONo == TRUE)
				{
					if(!pertenece(f, posControl, 16))
					{
						control[indice][c] = 1;
						indice++;
					}
				}
				else
				{
					if(!pertenece(f, posControl, 16))
					{
						control[indice][c] = 0;
						indice++;
					}

				}
			}
			if(siONo == FALSE)
			{
				siONo = TRUE;
			}
			else
			{
				siONo = FALSE;
			}
		}
		avance += pow(2, c);
		inicio = TRUE;
		indice = 0;
	}

	ham = fopen("HA1.txt","w+");
	hem = fopen("HE1.txt","w+");

	startCaracteres = caracteres;



	for(h = 0; h < final || sobrante[indiceSobrante] != 2;)
	{
		startModulo = modulo;

		i = 0;

		if(sobrante[indiceSobrante] != 2)
		{
			while(sobrante[indiceSobrante] != 2 && indiceSobrante < 8)
			{
				if(!pertenece(i, posControl, 16))
				{
					(*modulo) = sobrante[indiceSobrante];
					sobrante[indiceSobrante] = 2;
					indiceSobrante++;
				}
				else
				{
					(*modulo) = 2;
				}
				modulo++;
				i++;
			}
		}

		indiceSobrante = 0;

		while(i < tamano)
		{
			if (h < final)
			{
				for (j = 7; j >= 0;)
	    		{
	    			if(!pertenece(i, posControl, 16))
					{
						if(i < tamano)
						{
							*modulo = ((*caracteres) & (1 << j)) ? 1 : 0;
						}
						else
						{
							sobrante[indiceSobrante] = ((*caracteres) & (1 << j)) ? 1 : 0;
							indiceSobrante++;
						}
						j--;
	    			}
	    			else
	    			{
	    				*modulo = 2;
					}
	    			modulo++;
	    			i++;
				}
				caracteres++;
				h++;
			}
			else
			{
				if(!pertenece(i, posControl, 16))
				{
    				*modulo = 0;
    			}
	    		else
	    		{
	    			*modulo = 2;
				}
				modulo++;
				i++;
			}
		}

		int a,b=0, w;
		int suma=0;
        modulo = startModulo;
		for (a = 0; a < cantControl; a++)
		{
			modulo = startModulo;
			for (i = 0; i < tamano; i++, modulo++)
			{
				if(!pertenece(i, posControl, 16))
				{
					suma += (control[b][a] * (*modulo));
					b++;
				}
			}
			b = 0;
			modulo = startModulo;
			for(w = 1; w < pow(2, a); w++){
				modulo++;
			}
			*modulo = (suma % 2);
			suma = 0;
		}
		modulo = startModulo;

		ascii = 0;
		for(i = 0; i < tamano;)
		{
			for(j = 7; j >= 0; j--, i++, modulo++)
			{
				if((*modulo) == 1)
				{
					ascii += pow(2, j);
				}
			}
			fputc(ascii, ham);
			fputc(ascii, hem);
			ascii = 0;
		}

		modulo = startModulo;
		indiceSobrante = 0;
	}
	caracteres = startCaracteres;
	modulo = startModulo;
	free(caracteres);
	free(modulo);
	fclose(ham);
	fclose(hem);
	return;
 }



char *extraccionChars(int *cantCaracteres, char *nombreArchivo){
	FILE *archivo;
    char *caracteres = "";
    int i;
    int longitud;
    char *start;

    archivo = fopen(nombreArchivo,"r+");

    longitud = longitudDeArchivo(archivo);
    *cantCaracteres = longitud;

	caracteres = malloc(longitud*sizeof(char));

	start = caracteres;
    int q= 0;
    //printf("Longitud del archivo %d\n",longitud);
    if (archivo != NULL)
    {
        while(q<longitud)
	    {
	    	*caracteres = fgetc(archivo);
	    	caracteres++;
	    	q++;
	    }
    }
    caracteres = start;

    fclose(archivo);

    return caracteres;
}




void desprotegerSinCorreccion(int tamano, int cantControl)
{
	char ascii = 0, *caracteres, *start;
	int final;
	int j, i, f, bit, entradas = 0, longitud, exponente = 7;
		int posControl[16] = {0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095,8191,16383,32767};

	FILE *ham, *sinCorreccion;

	ham = fopen("HE1.txt", "r+");

	longitud = longitudDeArchivo(ham);
	final = longitud;

	caracteres = (char*)malloc(longitud*sizeof(char));

	start = caracteres;
	int q = 0;
	//("Longitud del archivo %d\n",longitud);

	if (ham != NULL)
    {
        while(q<longitud)
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
			q++;
	    }
    }
    caracteres = start;

    fclose(ham);

	sinCorreccion = fopen("DEE1.txt", "w+");

	for(f = 0; f < final;)
	{
		for(i = 0; i < tamano; f++, caracteres++)
		{
		    for (j = 7; j>=0; j--, i++)
		  	{
		  		bit = (*caracteres & (1 << j)) ? 1 : 0;
				if(!pertenece(i, posControl, 16))
				{
					if(bit == 1)
					{
						ascii += pow(2, exponente);
					}
		     		exponente--;
		     		entradas++;
		     		if(entradas == 8)
					{
						exponente = 7;
						entradas = 0;
						fputc(ascii, sinCorreccion);
						ascii = 0;
					}
				}
			}

		}
	}
	fclose(sinCorreccion);
	caracteres = start;
	free(caracteres);
}



void agregarError(int tamano, int cantControl){
	int longitud;
	char *caracteres, *startCaracteres;
	int sobrante[8], indiceSobrante = 0;
	int final;
	int ascii = 0;
	int *modulo, *startModulo;
	int caracterDeError, bitDeError, i, j, h;
	FILE *ham, *archivoError;

	vaciar(sobrante, 8);

	ham = fopen("HA1.txt", "r+");

	longitud = longitudDeArchivo(ham);
	final = longitud;

	caracteres = malloc(longitud*sizeof(char));
	modulo = malloc(tamano*sizeof(int));

	startCaracteres = caracteres;
    int q = 0;
    //("Longitud del archivo %d\n",longitud);
	if (ham != NULL)
    {
        while(q<longitud)
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
			q++;
	    }
    }
    caracteres = startCaracteres;

    fclose(ham);

    archivoError = fopen("HE1.txt", "w+");

    srand(time(NULL));

    for(h = 0; h < final || sobrante[indiceSobrante] != 2;)
	{
		startModulo = modulo;										//Esta parte crea un modulo

		i = 0;

		if(sobrante[indiceSobrante] != 2)
		{
			while(sobrante[indiceSobrante] != 2 && indiceSobrante < 8)
			{
				(*modulo) = sobrante[indiceSobrante];
				sobrante[indiceSobrante] = 2;
				indiceSobrante++;
				modulo++;
				i++;
			}
		}

		indiceSobrante = 0;

		while (i < tamano){
			if (h < final)
			{
				for (j = 7; j>=0; j--, modulo++, i++)
	    		{
	    			if(i < tamano)
					{
						(*modulo) = ((*caracteres) & (1 << j)) ? 1 : 0;
					}
					else
					{
						sobrante[indiceSobrante] = ((*caracteres) & (1 << j)) ? 1 : 0;
						indiceSobrante++;
					}
				}
				caracteres++;
				h++;
			}
			else
			{
				*modulo = 0;
				modulo++;
				i++;
			}
		}

		modulo = startModulo;

		int cantCaracteres = tamano/8;
		if(cantCaracteres == 0){
            cantCaracteres = 1;
		}

		int caracter = rand() % cantCaracteres;
		caracterDeError = caracter * 8;
		bitDeError = rand() % 8;


		modulo+= (caracterDeError + bitDeError);
		int meter = rand() %2;
		//("Mete error? %d\n",meter );
		if(meter){
		if((*modulo) == 1)
		{
			(*modulo) = 0;
		}
		else
		{
			(*modulo) = 1;
		}
	}
		// agregar posibilidad de introducir o no el error en cada modulo
	/*	printf("\nModulo con Error:\n");
		modulo = startModulo;
		for(i = 0; i < tamano; i++, modulo++)
		{
			printf("%d", (*modulo));
		}
		printf("\n\n");
*/
		modulo = startModulo;

		for(i = 0; i < tamano;)
		{
			for(j = 7; j >= 0; j--, i++, modulo++)
			{
				if((*modulo) == 1)
				{
					ascii += pow(2, j);
				}
			}
			fputc(ascii, archivoError);
			ascii = 0;
		}

		modulo = startModulo;

	}
    fclose(archivoError);
    modulo = startModulo;
    caracteres = startCaracteres;
    free(modulo);
    free(caracteres);
}



void desprotegerConCorreccion(int tamano, int cantControl,char *nombreArchivo)
{
	int matriz[tamano][cantControl];
	int c, f, j;
	int indice = 0, avance = 1, siONo = FALSE;
	int inicio = TRUE;
	int *modulo, *startModulo, *finalModulo;
	char ascii = 0, *caracteres, *startCaracteres;
	int finalCaracteres;
	int i, bit, entradas = 0, longitud, exponente = 7;
	int h;
	int suma = 0;
	int posicionDeError = 0, posicionActual = 1;
  int posControl[16] = {0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095,8191,16383,32767};


	modulo = malloc(tamano*sizeof(int));

	FILE *ham, *conCorreccion;

	ham = fopen(nombreArchivo, "r+");							//Esta parte se encarga de leer todos los caracteres del archivo a deshamminizar

	longitud = longitudDeArchivo(ham);
	finalCaracteres = longitud;

	caracteres = malloc(longitud*sizeof(char));

	startCaracteres = caracteres;
    int q = 0;
    //("Longitud del archivo %d\n",longitud);
	if (ham != NULL)
    {

       while(q<longitud)
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
			q++;
	    }
    }

    caracteres = startCaracteres;

    fclose(ham);

    for (c = 0; c <= cantControl-1; c++) //Con esto obtenemos la matriz para deshamminizar
	{
		if(c == 0)
		{
			siONo = TRUE;
		}
		else
		{
			siONo = FALSE;
		}
		for (f = 0; f < tamano;)
		{
			if(inicio == TRUE && c != 0)
			{
				j = 1;
			}
			else
			{
				j = 0;
			}
			inicio = FALSE;
			for(; j < avance && f < tamano; j++, f++)
			{
				if(siONo == TRUE)
				{
					matriz[indice][c] = 1;
					indice++;
				}
				else
				{
					matriz[indice][c] = 0;
					indice++;
				}
			}
			if(siONo == FALSE)
			{
				siONo = TRUE;
			}
			else
			{
				siONo = FALSE;
			}
		}
		avance += pow(2, c);
		inicio = TRUE;
		indice = 0;
	}



    conCorreccion = fopen("DE1.txt", "w+");

    startModulo = modulo;

    for(h = 0; h < finalCaracteres;)
	{
		modulo = startModulo;

		for(i = 0; i < tamano;)
		{
			if (h < finalCaracteres)
			{
				for (j = 7; j >= 0;)
	    		{
					(*modulo) = ((*caracteres) & (1 << j)) ? 1 : 0;
					j--;
	    			modulo++;
	    			i++;
				}
				caracteres++;
				h++;
			}
			else
			{
				while(i < tamano)
				{
					*modulo = 0;
			    	modulo++;
				    i++;
				}
			}
		}


		posicionDeError = 0;
		int resultado;
		for (c = 0; c < cantControl; c++)									//Aplicamos la matriz de deshaminizacion para encontrar la posicion de error
		{
			modulo = startModulo;
			suma = 0;
			for (i = 0; i < tamano; i++, modulo++)
			{
				resultado = (matriz[i][c] * (*modulo));
				if(resultado == 1)
				{
					if(suma == 0) suma = 1;
					else suma = 0;
				}
			}
			if(suma == 1)
			{
				posicionDeError += pow(2, c);
			}
		}

		modulo = startModulo;
		for(i = 1; i < posicionDeError; i++)
		{
			modulo++;
		}

		if((*modulo) == 0)
		{
			(*modulo) = 1;
		}
		else
		{
			(*modulo) = 0;
		}


		modulo = startModulo;

		for(i = 0; i < tamano;)
		{
		    for (j = 7; j >= 0; j--, i++, modulo++)
		  	{
		  		bit = (*modulo);
				if(!pertenece(i, posControl, 16))
				{
		     		if(bit == 1)
					{
						ascii += pow(2, exponente);
					}
					exponente--;
		     		entradas++;
		     		if(entradas == 8)
					{
						entradas = 0;
						exponente = 7;
						if(ascii != 0)
						{
							fputc(ascii, conCorreccion);
						}
						ascii = 0;
					}
				}
			}
		}
	}
	fclose(conCorreccion);
}


typedef struct{
	char simbolo;
	int peso;
}huffman;

typedef struct{
	int codigo[256];
	char simbolo;
	int peso;
}tabla;

typedef struct nodo{
	struct nodo *hDer;
	struct nodo *hIzq;
	struct nodo *siguiente;
	huffman dato;
}nodo;

typedef struct{
	nodo *raiz;
	nodo *actual;
	nodo *anterior;
}arbol;

typedef struct{
	nodo *inicio;
	nodo *actual;
	nodo *anterior;
}lista;
lista principal;
typedef struct nodotabla{
	struct nodotabla *hDer;
	struct nodotabla *hIzq;
	struct nodotabla *siguiente;
	tabla dato;
}nodotabla;
typedef struct{
	nodotabla *raiz;
	nodotabla *actual;
	nodotabla *anterior;
}arbolhuff;

void inicializarArbol(arbol *a, nodo *padre)
{
	(*a).raiz = padre;
	(*a).actual = padre;
	(*a).anterior = padre;
}


void inicializarLista(lista *li)
{
	(*li).inicio = NULL;
	(*li).actual = NULL;
	(*li).anterior = NULL;
}


void inicializarNodo(nodo *n)
{
	(*n).siguiente = NULL;
	(*n).hDer = NULL;
	(*n).hIzq = NULL;
}
void inicializarNodoHuff(nodotabla *n)
{
	(*n).siguiente = NULL;
	(*n).hDer = NULL;
	(*n).hIzq = NULL;
}


void inicioLista(lista *li)
{
	(*li).anterior = (*li).inicio;
	(*li).actual = (*li).inicio;
}

void inicioArbol(arbol *a)
{
	(*a).actual = (*a).raiz;
	(*a).anterior = (*a).raiz;
}

int avanzoLista(lista *li)
{
	int resultado = FALSE;
	if((*(*li).actual).siguiente != NULL)
	{
		if((*li).actual != (*li).anterior)
		{
			(*li).anterior = (*li).actual;
		}
		(*li).actual = (*(*li).anterior).siguiente;
		resultado = TRUE;
	}
	return resultado;
}

void avanzarArbol(arbol *a, int bit)
{
	(*a).anterior = (*a).actual;

	if(bit) (*a).actual = (*(*a).actual).hDer;
	else (*a).actual = (*(*a).actual).hIzq;
}

void ordenar(huffman *array, int inicio, int final) {
  int i = inicio, f = final;
  huffman tmp;
  int x = array[(inicio + final) / 2].peso;
  do {
    while(array[i].peso < x && f <= final) {
      i++;
    }
    while(x < array[f].peso && f > inicio) {
      f--;
    }
    if(i <= f) {
      tmp = array[i];
      array[i] = array[f];
      array[f] = tmp;
      i++; f--;
    }
  } while(i <= f);

  if(inicio < f) {
    ordenar(array,inicio,f);
  }

  if(i < final){
    ordenar(array,i,final);
  }

}


void ordenarTabla(tabla *array, int inicio, int final) {
  int i = inicio, f = final;
  tabla tmp;
  int x = array[(inicio + final) / 2].peso;
  do {
    while(array[i].peso > x && f <= final) {
      i++;
    }
    while(x > array[f].peso && f > inicio) {
      f--;
    }
    if(i <= f) {
      tmp = array[i];
      array[i] = array[f];
      array[f] = tmp;
      i++; f--;
    }
  } while(i <= f);

  if(inicio < f) {
    ordenarTabla(array,inicio,f);
  }

  if(i < final){
    ordenarTabla(array,i,final);
  }
}


int longitudDeArchivohf(FILE *fp)
{       
	int longitud = 0;
	if(fp != NULL)
	{
		while(fgetc(fp) != EOF)
		{
			longitud++;
		}
	rewind(fp);
	return longitud;
        }
        else{
            return 0;
        }
	
}



void vaciarFrecuencia(int frecuencias[])
{
	int i;
	for(i = 0; i < 256; i++)
	{
		frecuencias[i] = 0;
	}
}


char *extraccionCharshf (int *cantCaracteres, char *nombreArchivo)
{
	FILE *archivo;
    char *caracteres;
    int i;
    int longitud;
    char *start;

    archivo = fopen(nombreArchivo,"r+");

    longitud = longitudDeArchivohf(archivo);
    *cantCaracteres = longitud;

	caracteres = (char*)malloc(longitud*sizeof(char));

	start = caracteres;

    if (archivo != NULL)
    {
        while(!feof(archivo))
	    {
	    	*caracteres = fgetc(archivo);
	    	caracteres++;
	    }
    }
    caracteres = start;

    fclose(archivo);

    return caracteres;
}


void verArbolBB(nodo *Padre, nodo *Hijo)
{
	if(Hijo != NULL)
	{
		printf("EL nodo es: \n");
		printf("El peso del nodo es: %d \n", (*Hijo).dato.peso);
		printf("El simbolo del nodo es: %c \n", (*Hijo).dato.simbolo);
		if (Padre != Hijo)
		{
			printf("El peso del padre es: %d \n", (*Padre).dato.peso);
			printf("El simbolo del padre es: %c \n", (*Padre).dato.simbolo);
		}
		else printf("El nodo no tiene padre por lo tanto es raiz.\n");
		if((*Hijo).hIzq != NULL)
		{
			printf("El peso del hijo izquierdo es: %d \n", (*(*Hijo).hIzq).dato.peso);
			printf("El simbolo del hijo izquierdo es: %c \n", (*(*Hijo).hIzq).dato.simbolo);
		}
		else printf("El nodo no tiene hijo izquierdo\n");
		if((*Hijo).hDer != NULL)
		{
			printf("El peso del hijo derecho es: %d \n", (*(*Hijo).hDer).dato.peso);
			printf("El simbolo del hijo derecho es: %c \n", (*(*Hijo).hDer).dato.simbolo);
		}
		else printf("El nodo no tiene hijo derecho\n");
		printf ("\n ----------------------------------------------------------------------- \n \n");
		verArbolBB(Hijo, (*Hijo).hIzq);
		verArbolBB(Hijo, (*Hijo).hDer);
	}
}

int esHoja(nodo *p)
{
	int resultado = FALSE;
	if((*p).hDer == NULL && (*p).hIzq == NULL)
	{
		resultado = TRUE;
	}
	return resultado;
}

void armarTabla (nodo *padre, nodo *analizar, tabla arreglo[], int codigo[], int *j, int *indice)
{
	int contador;

	if(esHoja(analizar))
	{
		if(analizar == padre)
		{
			arreglo[(*indice)].codigo[0] = 1;
			arreglo[(*indice)].codigo[1] = 2;
		}
		else
		{
			for(contador = 0; contador < (*j); contador++)
			{
				arreglo[(*indice)].codigo[contador] = codigo[contador];
				//printf("Codigo : %d\n", codigo[contador]);
			}
			//printf("termino 1\n");
			arreglo[(*indice)].codigo[contador] = 2;
		}
		arreglo[(*indice)].peso = (*analizar).dato.peso;
		arreglo[(*indice)].simbolo = (*analizar).dato.simbolo;

		(*indice)++;
	}
	else
	{
		codigo[(*j)] = 0;
		(*j)++;
		armarTabla(padre, (*analizar).hIzq, arreglo, codigo, j, indice);

		codigo[(*j)] = 1;
		(*j)++;
		armarTabla(padre, (*analizar).hDer, arreglo, codigo, j, indice);

	}
	(*j)--;
}

void armarTablaAux (nodo *padre, nodo *actual, tabla arreglo[])
{
	int codigo[256];
	int j = 0, indice = 0;
	tabla *inicio;
	inicio = arreglo;

	armarTabla(padre, actual, arreglo, codigo, &j, &indice);
}


int huffmaning(tabla arreglo[], char *caracteres, int cantCaracteres)
{
	int *startModulo, *modulo;
	FILE *huf;
	int i=0, j, k;
	char ascii = 0;
	int tamanoModulo = 0;

	modulo = (int*)malloc(cantCaracteres*8*sizeof(int));

	startModulo = modulo;

	for (j=0; j<cantCaracteres; j++, caracteres++)
	{
		i=0;

		while ((int) arreglo[i].simbolo != (int) (*caracteres)) i++;

		k=0;
		while (arreglo[i].codigo[k] != 2)
		{
			(*modulo) = arreglo[i].codigo[k];
			modulo++;
			tamanoModulo++;
			k++;
		}
	}
	huf = fopen("HUF.txt","w+");

	modulo = startModulo;

	ascii = 0;
	i=0;
	while(i < tamanoModulo)
	{
		for(j = 7; j >= 0; j--, i++, modulo++)
		{
			if (i < tamanoModulo)
			{
				if((*modulo) == 1)
				{
					ascii += pow(2, j);
				}
			}
		}
		fputc(ascii, huf);
		ascii = 0;
	}
	modulo = startModulo;
	free(modulo);

	fclose(huf);
	return tamanoModulo;
}

void deshuffminizar(arbol a, char *caracteres, int cantCaracteres, int tamano)
{

	int j = 0, i = 0, *inicioModulo, tamModulo;
	int ab=0;
	FILE *fap;
	int entro = FALSE;
	int modulo[(cantCaracteres*8)];


	for (i = 0; i < cantCaracteres; i++, caracteres++)
	{
		for(j = 7;  j >= 0; j--, ab++)
		{
			modulo[ab] = ((*caracteres) & (1 << j)) ? 1 : 0;
		}
	}
	fap = fopen("DEHUF.txt", "w+");

	i = 0;
	ab=0;
	while(i < tamano)
	{
		while(!esHoja(a.actual) && i < tamano)
		{
			avanzarArbol(&a, modulo[ab]);
			ab++;
			i++;
			entro = TRUE;
		}
		if (entro == FALSE) i++;
		if (esHoja(a.actual)) fputc((*a.actual).dato.simbolo, fap);
		inicioArbol(&a);
	}

	fclose(fap);

}

int contarFrecu(int frecuencias[],char *nomArch){
		int cantCaracteres,caracteresTexto= 0;
	  char *caracteres= extraccionCharshf(&cantCaracteres,nomArch);
		for(int contador = 0; contador < cantCaracteres; contador++) //Contamos la frecuencia de los caracteres.
		{
			if(frecuencias[(int)*caracteres] == 0) caracteresTexto++;
			frecuencias[(int)*caracteres]++;
			caracteres++;
		}
		return caracteresTexto;
}
void armarPesos(huffman caracFrecu[],int frecuencias[]){
	int indice = 0;
	for(int contador = 0; contador < 256; contador++)
	{
		if(frecuencias[contador] != 0)
		{
			caracFrecu[indice].simbolo = contador;
			caracFrecu[indice].peso = frecuencias[contador];
			indice++;
		}
	}
}
void armarArbol(huffman caracFrecu[],int caracteresTexto){
	nodo *aAgregar;

	for(int contador = 0; contador < caracteresTexto; contador++)
	{
		aAgregar = (nodo*)malloc(sizeof(nodo));
		inicializarNodo(aAgregar);
		if (caracFrecu[contador].peso != 0)
		{
			(*aAgregar).dato.peso = caracFrecu[contador].peso;
			(*aAgregar).dato.simbolo = caracFrecu[contador].simbolo;
			if(principal.inicio == NULL)
			{
				principal.inicio = aAgregar;
				principal.actual = aAgregar;
				principal.anterior = aAgregar;
			}
			else
			{
				if(principal.anterior != principal.actual)
				{
					principal.anterior = principal.actual;
				}
				principal.actual = aAgregar;
				(*principal.anterior).siguiente = principal.actual;
			}
		}
	}




		inicioLista(&principal);
		avanzoLista(&principal);

	while(principal.actual != principal.anterior)
	{
		nodo *a, *b;

		b = principal.actual;
		a = principal.anterior;
		if((*principal.actual).siguiente != NULL)
		{
			principal.inicio = (*principal.actual).siguiente;
			principal.anterior = (*principal.actual).siguiente;
			principal.actual = (*principal.actual).siguiente;
		}
		else
		{
			principal.inicio = NULL;
			principal.actual = NULL;
			principal.anterior = NULL;
		}

		aAgregar = (nodo*)malloc(sizeof(nodo));
		inicializarNodo(aAgregar);
		(*aAgregar).dato.peso = (*a).dato.peso + (*b).dato.peso;
		(*aAgregar).dato.simbolo = 0;
		(*aAgregar).hIzq = a;
		(*aAgregar).hDer = b;
		if(principal.inicio != NULL)
		{
			while((*aAgregar).dato.peso >= (*principal.actual).dato.peso && avanzoLista(&principal));
			if ((*aAgregar).dato.peso < (*principal.actual).dato.peso)
			{
				if(principal.actual == principal.anterior)
				{
					principal.inicio = aAgregar;
					principal.anterior = aAgregar;
				}
				else
				{
					(*principal.anterior).siguiente = aAgregar;
				}
				(*aAgregar).siguiente = principal.actual;
				principal.actual = aAgregar;
			}
			else
			{
				(*principal.actual).siguiente = aAgregar;
				principal.anterior =principal.actual;
				principal.actual = aAgregar;
			}

			inicioLista(&principal);
			avanzoLista(&principal);
		}
		else
		{
			principal.inicio = aAgregar;
			principal.actual = aAgregar;
			principal.anterior = aAgregar;
		}

	}
}

void leerTabla(huffman caracFrecu[]){
	FILE *tabla;
	tabla = fopen("Tabla.txt","r+");
	int ind = 0;
	char carac;
	int frecu;
	while(carac =fgetc(tabla) !=EOF){
		caracFrecu[ind].simbolo = carac;
		fgetc(tabla);
		fscanf(tabla,"%d",&frecu);
		fgetc(tabla);
		ind ++;
	}

}




void comprimirArchivo(char *nombreArchi)
{
    //declaraciones
    char *caracteres, *inicioCaracteres;
    int cantCaracteres, contador, frecuencias[256], caracteresTexto = 0, indice = 0;


    vaciarFrecuencia(frecuencias);
    inicializarLista(&principal);

    //armar arbol
    caracteres = extraccionCharshf(&cantCaracteres, nombreArchi);
    inicioCaracteres = caracteres;
    caracteresTexto = contarFrecu(frecuencias,nombreArchi);
    huffman caracFrecu[caracteresTexto];//Separamos los caracteres de importancia.
    tabla arreglo[caracteresTexto];
    int pesotabla;
    char simbolotabla;

    armarPesos(caracFrecu,frecuencias);

    FILE *tabla;

    if ((tabla = fopen("Tabla.txt", "w+")) == NULL) {
       // printf("Error, no se pudo abrir el archivo\n");
    }
		for(contador = 0; contador < caracteresTexto; contador++)
		{
				//printf("Caracter:" );
				simbolotabla = caracFrecu[contador].simbolo;
				//printf("%c \n", simbolotabla);
				fprintf(tabla,"%c",simbolotabla);
				if(simbolotabla != '\n')
				fputc('\n', tabla);
				pesotabla = caracFrecu[contador].peso;
				fprintf(tabla, "%d", pesotabla);
				//printf("frecu:" );
				//printf("%d \n", pesotabla);
				int contador2 = 0;
                fputc('\n', tabla);
				//printf("\n\n\n");
		}
    ordenar(caracFrecu, 0, caracteresTexto-1);
    armarArbol(caracFrecu,caracteresTexto);
    inicioLista(&principal);
    armarTablaAux(principal.actual, principal.actual, arreglo);
    ordenarTabla(arreglo, 0, caracteresTexto-1);
    int tamModulo = huffmaning(arreglo, caracteres, cantCaracteres);

    //int tamModulo = huffmaning(arreglo, caracteres, cantCaracteres);

    FILE *tamanos;
    tamanos = fopen("Tamanos.txt","w+");
		fprintf(tamanos, "%d", caracteresTexto);
		fputc('\n',tamanos);
		fprintf(tamanos,"%d",tamModulo);
		fputc('\n',tamanos);
		fclose(tabla);
		fclose(tamanos);
    caracteres = inicioCaracteres;
    free(caracteres);

}
void descomprimirArchivo(char *nombreArchi)
{
  FILE *tablita,*archivo,*tamanos;
  tablita = fopen("Tabla.txt","r+");
  tamanos = fopen("Tamanos.txt","r+");
  int caracteresTexto;
  int cantCaracteres;
  int tamModulo;
  arbol aDeHuff;
  char *caracteres, *inicioCaracteres;
  fscanf(tamanos,"%d",&caracteresTexto);
  fgetc(tamanos);
  //printf("caracteresTexto : %d\n",caracteresTexto );
  fscanf(tamanos,"%d",&tamModulo);
  fgetc(tamanos);
  //printf("tamModulo : %d\n",tamModulo);
  fclose(tamanos);
  huffman caracFrecu[caracteresTexto];
  tabla arreglo[caracteresTexto];
  int ind = 0;
  char carac;
  int frecu;
  char cfin;
  while(fscanf(tablita,"%c",&carac) != EOF){
    caracFrecu[ind].simbolo = carac;
    //printf("Caracter : %c \n", carac);
    if(carac != '\n')
    cfin = fgetc(tablita);
    fscanf(tablita,"%d",&frecu);
		caracFrecu[ind].peso = frecu;
    //printf("Peso : %d \n",frecu );
    cfin =fgetc(tablita);
    ind ++;
  }
  fclose(tablita);
	for(int q = 0;q<caracteresTexto;q++){
		//printf("Caracter: %c \n",caracFrecu[q].simbolo );

			//printf("Peso: %d \n",caracFrecu[q].peso );
	}
  ordenar(caracFrecu, 0, caracteresTexto-1);
	inicializarLista(&principal);
	inicioLista(&principal);
  armarArbol(caracFrecu,caracteresTexto);
  armarTablaAux(principal.actual, principal.actual, arreglo);
  //printf("llega aca \n");
  ordenarTabla(arreglo, 0, caracteresTexto-1);
  for(int contador = 0; contador < caracteresTexto; contador++)
  {
    //printf("%c ", arreglo[contador].simbolo);
    int contador2 = 0;
    while(arreglo[contador].codigo[contador2] != 2)
    {
            //printf("%d ", arreglo[contador].codigo[contador2]);
            contador2++;
    }
    //printf("\n\n\n");

  }
  caracteres = extraccionCharshf(&cantCaracteres, nombreArchi);
  inicioCaracteres = caracteres;
  inicioLista(&principal);

  inicializarArbol(&aDeHuff, principal.actual);


  deshuffminizar(aDeHuff, caracteres, cantCaracteres, tamModulo);
  caracteres = inicioCaracteres;
  free(caracteres);
  //printf("LLEGO ACA\n" );
}
void proteger(int bits,int prote,char *rutac){
    char *caracteres;
    int cantCaracteres;

    caracteres = extraccionChars(&cantCaracteres, rutac);
    hammining(caracteres,cantCaracteres,bits,prote);
    agregarError(bits,prote);
}
void desproteger(int bits,int prote,char *rutac){
    desprotegerConCorreccion(bits,prote,rutac);
    
}
/*
 * Class:     tlyc_Choose1
 * Method:    desprotegera
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose1_desprotegera
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    desproteger(7,3,rutac);
}

/*
 * Class:     tlyc_Choose1
 * Method:    desprotegerb
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose1_desprotegerb
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    desproteger(32,5,rutac);
    
}

/*
 * Class:     tlyc_Choose1
 * Method:    desprotegerc
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose1_desprotegerc
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    desproteger(1024,10,rutac);
}

/*
 * Class:     tlyc_Choose1
 * Method:    desprotegerd
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose1_desprotegerd
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    desproteger(32768,15,rutac);
}
/*
 * Class:     tlyc_ProteccionEncriptacion
 * Method:    desproteger
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_ProteccionEncriptacion_desproteger
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    
    
}

/*
 * Class:     tlyc_ProteccionEncriptacion
 * Method:    comprimir
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_ProteccionEncriptacion_comprimir
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    //printf("%s",rutac);
    comprimirArchivo(rutac);
}

/*
 * Class:     tlyc_ProteccionEncriptacion
 * Method:    descomprimir
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_ProteccionEncriptacion_descomprimir
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    
    descomprimirArchivo(rutac);
}
/*
 * Class:     tlyc_Choose
 * Method:    hamminga
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose_hamminga
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    proteger(7,3,rutac);
}

/*
 * Class:     tlyc_Choose
 * Method:    hammingb
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose_hammingb
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    proteger(32,5,rutac);
}

/*
 * Class:     tlyc_Choose
 * Method:    hammingc
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose_hammingc
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    proteger(1024,10,rutac);
}

/*
 * Class:     tlyc_Choose
 * Method:    hammingd
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tlyc_Choose_hammingd
  (JNIEnv *env, jobject obj, jstring ruta){
    const char *rutac = (*env) -> GetStringUTFChars (env, ruta, NULL);
    proteger(32768,15,rutac);
}