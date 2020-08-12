#include <stdio.h>
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
		/*while(fgetc(fp) != EOF)
		{
			sz++;
		}*/
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
		//printf("\nSobrante:\n");
		//for(i = 0; i < 8; i++)
		//{
		//	printf("%d", sobrante[i]);
		//}
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
        //printf("\n\n");
		//printf("\nModulo sin control:\n");
        modulo = startModulo;
		//for(i = 0; i < tamano; i++, modulo++)
		//{
		//	printf("%d", (*modulo));
		//}
		//printf("\n\n");

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

		/*printf("\n\n");
		printf("\nModulo hamminzado:\n");
		modulo = startModulo;
		for(i = 0; i < tamano; i++, modulo++)
		{
			printf("%d", (*modulo));
		}
		printf("\n\n");*/

		modulo = startModulo;

		//ten points
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



char *extraccionChars (int *cantCaracteres, char *nombreArchivo)
{
	FILE *archivo;
    char *caracteres = "";
    int i;
    int longitud;
    char *start;

    archivo = fopen("pruebita.txt","r+");

    longitud = longitudDeArchivo(archivo);
    *cantCaracteres = longitud;

	caracteres = malloc(longitud*sizeof(char));

	start = caracteres;
    int q= 0;
    printf("Longitud del archivo %d\n",longitud);
    if (archivo != NULL)
    {
        while(q<longitud)
	    {
	    	*caracteres = fgetc(archivo);
	    	caracteres++;
	    	q++;
	    }
        /*while(!feof(archivo))
	    {
	    	*caracteres = fgetc(archivo);
	    	caracteres++;
	    }*/
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
	printf("Longitud del archivo %d\n",longitud);

	if (ham != NULL)
    {
        while(q<longitud)
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
			q++;
	    }
        /*while(!feof(ham))
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
	    }*/
    }
    caracteres = start;

    fclose(ham);

	sinCorreccion = fopen("DE1.txt", "w+");

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
    printf("Longitud del archivo %d\n",longitud);
	if (ham != NULL)
    {
        while(q<longitud)
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
			q++;
	    }
        /*while(!feof(ham))
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
	    }*/
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

		//printf("\n%d %d\n", caracter, bitDeError);

		/*for(i = 0; i < caracterDeError; i++)
		{
			modulo++;
		}
		for(j = 0; j < bitDeError; j++)
		{
			modulo++;
		}*/

		modulo+= (caracterDeError + bitDeError);
		if((*modulo) == 1)
		{
			(*modulo) = 0;
		}
		else
		{
			(*modulo) = 1;
		}

		/*printf("\nModulo con Error:\n");
		modulo = startModulo;
		for(i = 0; i < tamano; i++, modulo++)
		{
			printf("%d", (*modulo));
		}
		printf("\n\n");*/

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



void desprotegerConCorreccion(int tamano, int cantControl)
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

	ham = fopen("HE1.txt", "r+");							//Esta parte se encarga de leer todos los caracteres del archivo a deshamminizar

	longitud = longitudDeArchivo(ham);
	finalCaracteres = longitud;

	caracteres = malloc(longitud*sizeof(char));

	startCaracteres = caracteres;
    int q = 0;
    printf("Longitud del archivo %d\n",longitud);
	if (ham != NULL)
    {

       while(q<longitud)
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
			q++;
	    }
        /*while(!feof(ham))
	    {
	    	*caracteres = fgetc(ham);
			caracteres++;
	    }*/
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
		//printf("\n");
		for (c = 0; c < cantControl; c++)									//Aplicamos la matriz de deshaminizacion para encontrar la posicion de error
		{
			modulo = startModulo;
			suma = 0;
			for (i = 0; i < tamano; i++, modulo++)
			{
				resultado = (matriz[i][c] * (*modulo));
				//printf("%d x %d = %d\n", matriz[i][c], (*modulo), resultado);
				if(resultado == 1)
				{
					if(suma == 0) suma = 1;
					else suma = 0;
				}
			}
			//printf("%d\n", suma);
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

		//printf("\n%d\n", posicionDeError);

		if((*modulo) == 0)
		{
			(*modulo) = 1;
		}
		else
		{
			(*modulo) = 0;
		}


		/*printf("\nModulo corregido:\n");
		modulo = startModulo;
		for(i = 0; i < tamano; i++, modulo++)
		{
			printf("%d", (*modulo));
		}
		*/
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



void main(int argc, char *argv[]) {
	int i, opcion, opcionSw1, opcionSw2, opcionSw3, opcionSw4;
	char *nombreArchivo = "Prueba.txt";
	char *caracteres;
	int cantCaracteres;

	caracteres = extraccionChars(&cantCaracteres, nombreArchivo);
	do{		
		printf("\n\n	1.Proteger archivo con bloque de 7 bits");
		printf("\n	2.Proteger archivo con bloque de 32 bits");
		printf("\n	3.Proteger archivo con bloque de 1024 bits");
		printf("\n	4.Proteger archivo con bloque de 32768 bits");
		printf("\n	5.Salir");
		printf("\n	Introducir opcion (1-5): ", 162);

		scanf("%d", &opcion);

		switch(opcion)
		{
			case 1:
				do
				{
					printf("\n\n	1. Proteger archivo sin errores");
					printf("\n	2. Proteger archivo con errores");
					printf("\n	3. Desproteger archivo sin corregir");
					printf("\n	4. Desproteger archivo corrigiendo");
					printf("\n	5.Salir");
					printf("\n	Introducir opcion (1-5): ", 162);

					scanf("%d", opcionSw1);

					switch(opcionSw1)
					{
						case 1:
							hammining(caracteres, cantCaracteres, 7, 3);
						break;
						case 2:
							agregarError(7, 3);
						break;
						case 3:
							desprotegerSinCorreccion(7, 3);
						break;
						case 4:
							desprotegerConCorreccion(7, 3);
						break;
					}
				}while(opcionSw1 != 5);
			break;
			case 2:
				do
				{
					printf("\n\n	1. Proteger archivo sin errores");
					printf("\n	2. Proteger archivo con errores");
					printf("\n	3. Desproteger archivo sin corregir");
					printf("\n	4. Desproteger archivo corrigiendo");
					printf("\n	5.Salir");
					printf("\n	Introducir opcion (1-5): ", 162);

					scanf("%d", opcionSw2);

					switch(opcionSw2)
					{
						case 1:
							hammining(caracteres, cantCaracteres, 32, 5);
						break;
						case 2:
							agregarError(32, 5);
						break;
						case 3:
							desprotegerSinCorreccion(32, 5);
						break;
						case 4:
							desprotegerConCorreccion(32, 5);
						break;
					}
				}while(opcionSw2 != 5);
			break;
			case 3:
				do
				{
					printf("\n\n	1. Proteger archivo sin errores");
					printf("\n	2. Proteger archivo con errores");
					printf("\n	3. Desproteger archivo sin corregir");
					printf("\n	4. Desproteger archivo corrigiendo");
					printf("\n	5.Salir");
					printf("\n	Introducir opcion (1-5): ", 162);

					scanf("%d", opcionSw3);

					switch(opcionSw3)
					{
						case 1:
							hammining(caracteres, cantCaracteres, 1024, 10);
						break;
						case 2:
							agregarError(1024, 10);
						break;
						case 3:
							desprotegerSinCorreccion(1024, 10);
						break;
						case 4:
							desprotegerConCorreccion(1024, 10);
						break;
					}
				}while(opcionSw3 != 5);
			break;
			case 4:
				do
				{
					printf("\n\n	1. Proteger archivo sin errores");
					printf("\n	2. Proteger archivo con errores");
					printf("\n	3. Desproteger archivo sin corregir");
					printf("\n	4. Desproteger archivo corrigiendo");
					printf("\n	5.Salir");
					printf("\n	Introducir opcion (1-5): ", 162);

					scanf("%d", opcionSw4);

					switch(opcionSw4)
					{
						case 1:
							hammining(caracteres, cantCaracteres, 32768, 15);
						break;
						case 2:
							agregarError(32768, 15);
						break;
						case 3:
							desprotegerSinCorreccion(32768, 15);
						break;
						case 4:
							desprotegerConCorreccion(32768, 15);
						break;
					}
				}while(opcionSw4 != 5);
			break;
		}
	}while(opcion != 5);

	
	/*hammining(caracteres, cantCaracteres, 7, 3);
  agregarError(7, 3);
	//desprotegerSinCorreccion(7, 3);

	desprotegerConCorreccion(7, 3);*/
}
