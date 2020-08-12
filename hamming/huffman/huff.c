#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include<time.h>
#define TRUE 1
#define FALSE 0
#define MAX 200

/*------------------------------------STRUCT ARBOL VIEJO---------------------------------------------------------------
typedef struct{
    char nombre[51], telefono[13],  categoria[13];
    float deuda;
    int anio;
}Y;

typedef struct{
    int dni;
    Y datos;
}Socio;


Socio *arrSocLI[MAX];
int ultElemLI = -1;

typedef struct nodo {
    Socio Soc;
    struct nodo *der;
    struct nodo *izq;
} nodo;

typedef nodo *pNodo;
nodo *raiz = NULL;


/*-----------------------------------------------------------------STRUCT POSTA------------------------------------*/
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


int longitudDeArchivo(FILE *fp)
{
	int longitud = 0;
	if(fp != NULL)
	{
		while(fgetc(fp) != EOF)
		{
			longitud++;
		}
	}
	rewind(fp);
	return longitud;
}



void vaciarFrecuencia(int frecuencias[])
{
	int i;
	for(i = 0; i < 256; i++)
	{
		frecuencias[i] = 0;
	}
}


char *extraccionChars (int *cantCaracteres, char *nombreArchivo)
{
	FILE *archivo;
    char *caracteres;
    int i;
    int longitud;
    char *start;

    archivo = fopen(nombreArchivo,"r+");

    longitud = longitudDeArchivo(archivo);
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
			}
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

int contarFrecu(int frecuencias[]){
		int cantCaracteres,caracteresTexto= 0, contador;
	  char *caracteres= extraccionChars(&cantCaracteres,"Prueba.txt");
		for(contador = 0; contador < cantCaracteres; contador++) //Contamos la frecuencia de los caracteres.
		{
			if(frecuencias[(int)*caracteres] == 0) caracteresTexto++;
			frecuencias[(int)*caracteres]++;
			caracteres++;
		}
		return caracteresTexto;
}
void armarPesos(huffman caracFrecu[],int frecuencias[]){
	int indice = 0, contador;
	for(contador = 0; contador < 256; contador++)
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
	int contador;
	for(contador = 0; contador < caracteresTexto; contador++)
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
		(*aAgregar).dato.simbolo = NULL;
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





/*-------------------------------------------FIN ARBOL VIEJO -------------------------------------------*/

void comprimirArchivo()
{
    //declaraciones
    char *nombreArchivo = "Prueba.txt", *caracteres, *inicioCaracteres;
    int cantCaracteres, contador, frecuencias[256], caracteresTexto = 0, indice = 0;


    vaciarFrecuencia(frecuencias);
    inicializarLista(&principal);

    //armar arbol
    caracteres = extraccionChars(&cantCaracteres, nombreArchivo);
    inicioCaracteres = caracteres;
    caracteresTexto = contarFrecu(frecuencias);
    huffman caracFrecu[caracteresTexto];//Separamos los caracteres de importancia.
    tabla arreglo[caracteresTexto];
    armarPesos(caracFrecu,frecuencias);
    ordenar(caracFrecu, 0, caracteresTexto-1);
    armarArbol(caracFrecu,caracteresTexto);
    inicioLista(&principal);
    armarTablaAux(principal.actual, principal.actual, arreglo);
    ordenarTabla(arreglo, 0, caracteresTexto-1);


    FILE *tabla;

    if ((tabla = fopen("Tabla.txt", "w+")) == NULL) {
        printf("Error, no se pudo abrir el archivo\n");
    }

    fprintf(tabla, "%d", caracteresTexto);
    fputc('\n',tabla);

    int tamModulo = huffmaning(arreglo, caracteres, cantCaracteres);
    fprintf(tabla,"%d",tamModulo);
    fputc('\n',tabla);
    for(contador = 0; contador < caracteresTexto; contador++)
    {
        printf("%c ", arreglo[contador].simbolo);
        fputc(caracFrecu[contador].simbolo, tabla);
        if(caracFrecu[contador].simbolo != '\n')
        fputc('\n', tabla);
        fprintf(tabla, "%d", caracFrecu[contador].peso);
        int contador2 = 0;
        while(arreglo[contador].codigo[contador2] != 2)
        {
            printf("%d ", arreglo[contador].codigo[contador2]);
            //fprintf(tabla, "%d", arreglo[contador].codigo[contador2]);
            contador2++;
        }
        //fputc(frecuencias, tabla);
        fputc('\n', tabla);
        printf("\n\n\n");
    }
    fclose(tabla);
    //int tamModulo = huffmaning(arreglo, caracteres, cantCaracteres);
    caracteres = inicioCaracteres;
    free(caracteres);

}

void descomprimirArchivo()
{
  FILE tablita,archivo;
  tablita = fopen("Tabla.txt","r+");
  int caracteresTexto;
  int cantCaracteres;
  int tamModulo;
  arbol aDeHuff;
  char *caracteres, *inicioCaracteres;
  fscanf(tablita,"%d",&caracteresTexto);
  fgetc(tablita);
  printf("caracteresTexto : %d\n",caracteresTexto );
  fscanf(tablita,"%d",&tamModulo);
  fgetc(tablita);
  printf("tamModulo : %d\n",tamModulo);

  huffman caracFrecu[caracteresTexto];
  tabla arreglo[caracteresTexto];
  int ind = 0;
  char carac;
  int frecu;
  while(fscanf(tablita,"%c",&carac) != EOF){
    caracFrecu[ind].simbolo = carac;
    printf("Caracter : %c \n", carac);
    fgetc(tablita);
    fscanf(tablita,"%d",&frecu);
    printf("Peso : %d \n",frecu );
    fgetc(tablita);
    ind ++;
  }
  ordenar(caracFrecu, 0, caracteresTexto-1);
  inicicioLista(&principal);
  armarArbol(caracFrecu,caracteresTexto);
  armarTablaAux(principal.actual, principal.actual, arreglo);
  printf("llega aca \n");
  ordenarTabla(arreglo, 0, caracteresTexto-1);


  caracteres = extraccionChars(&cantCaracteres, "HUF.txt");
  inicioCaracteres = caracteres;
  inicioLista(&principal);

  inicializarArbol(&aDeHuff, principal.actual);


  deshuffminizar(aDeHuff, caracteres, cantCaracteres, tamModulo);
  caracteres = inicioCaracteres;
  free(caracteres);
  printf("LLEGO ACA\n" );
}
/*
void leerTabla(huffman caracFrecu[])
{
	int caracteresTexto, contador = 0;
    FILE *fp;

	if ((fp = fopen("Tabla.txt", "r+")) == NULL) {
        printf("Error, no se pudo abrir el archivo\n");
    }
	rewind(fp);
	fscanf(fp,"%d", &caracteresTexto);
	char cfin=fgetc(archivo);
	while(!feof(fp) && (contador < caracteresTexto) && (cfin != EOF))
	{
		fscanf(fp,"%d", &caracFrecu[caracteresTexto].simbolo);
		cfin=fgetc(archivo);
		fscanf(fp,"%d", &caracFrecu[caracteresTexto].peso);
		cfin=fgetc(archivo);
		contador ++;
	}
	fclose(fp);
    return resultado[];
}
*/
int main( int argc, const char* argv[] )
{
    int opcion;

    do{
		printf("\n	Introducir opcion (1-3): ");
        printf("\n\n1.Comprimir archivo");
		printf("\n	2.Descomprimir archivo");
		printf("\n	3.Salir");

		scanf("%d", &opcion);
        switch (opcion)
        {
            case 1:
                comprimirArchivo();
                break;
        
            case 2:
                descomprimirArchivo();
                break;
        }

    }while(opcion != 3);

	return 0;
}
