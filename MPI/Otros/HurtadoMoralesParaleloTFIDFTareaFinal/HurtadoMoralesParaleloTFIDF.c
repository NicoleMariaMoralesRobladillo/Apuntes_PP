/*
	COMANDOS:
	- Compilar: mpicc HurtadoMoralesParaleloTFIDF.c -o HurtadoMoralesParaleloTFIDF -lm
	- Ejecutar: mpirun -np 3 ./HurtadoMoralesParaleloTFIDF
	INTEGRANTES:
	Hurtado Santos, Estiven Salvador - 20200135
	Morales Robladillo, Nicole Maria - 20200136
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#define NUM_DOCUMENTOS 3

typedef struct {
	int idDocumento;
	int totalPalabras;
} Documento;

typedef struct {
	char* palabra;
	int longitud;
	int conteoAparicionPalabraPorDocumento[NUM_DOCUMENTOS];
	int conteoDocumentosConPalabra;
	double tf[NUM_DOCUMENTOS];
	double idf;
	double tfidf[NUM_DOCUMENTOS];
} Palabra;

void removeSpecialChars(char* word) {
	char* src = word;
	char* dst = word;
	while (*src) {
		if (isalpha(*src)) {
			*dst++ = *src;
		}
		src++;
	}
	*dst = '\0';
}

void removeNewLine(char* str) {
	size_t length = strcspn(str, "\n");
	str[length] = '\0';
}

double calcularTF(Palabra palabra, Documento documento) {
	int idDocumento = documento.idDocumento;
	double tf = (double) palabra.conteoAparicionPalabraPorDocumento[idDocumento] / (double) documento.totalPalabras;
	return tf;
}

double calcularIDF(Palabra palabra) {
	double div = (double) NUM_DOCUMENTOS / (double) palabra.conteoDocumentosConPalabra;
	double idf = log10(div);
	return idf;
}

double calcularTFIDF(Palabra palabra, int idDocumento) {
	double tfidf = palabra.tf[idDocumento] * palabra.idf;
	return tfidf;
}

int main(int argc, char** argv) {
	int proceso = 0, procesos = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
	
	// Se aborta si el numero de ranks es menor al numero de documentos
	if(procesos < NUM_DOCUMENTOS) {
		MPI_Abort(MPI_COMM_WORLD, 10);
		return 0;
	}
	else {
		int i, j;
		char textoDocumentos[NUM_DOCUMENTOS][10000];
		char nombreDocumentoN[100];
		char* nombresArchivos[] = {"sample1.txt", "sample2.txt", "sample3.txt"};
		FILE* file;
		
		// Lectura de documentos
		for(i = 0; i < NUM_DOCUMENTOS; i++){
			strcpy(nombreDocumentoN, nombresArchivos[i]);
			file = fopen(nombreDocumentoN, "r");
			assert(file != NULL);
			char textoCompleto[10000] = "";
			char line[1000];
			while(fgets(line, sizeof(line), file)){
				removeNewLine(line);
				strcat(textoCompleto, line);
			}
			int length = strlen(textoCompleto);
			for(j = 0; j < length; j++){
				if(!isalpha(textoCompleto[j])){
					textoCompleto[j] = ' ';
				}
			}
			strcpy(textoDocumentos[i], textoCompleto);
			fclose(file);
			fflush(stdout);
		}
		
		int totalPalabrasEnDocumento = 0;
		char* palabrasEnDocumento;
		int totalPalabrasUnicasEnDocumentos = 0;
		
		// Guarda informacion de las palabras del corpus de documentos
		Palabra palabrasUnicasEnDocumentos[10000];
		
		char textoDocumentoN[10000];
		
		// Se copia el texto del documento i en textoDocumentoN, donde i = rank
		strcpy(textoDocumentoN, textoDocumentos[proceso]);
		
		// Se transforma textoDocumentoN a todo en mayuscula
		for (i = 0; textoDocumentoN[i] != '\0'; i++) {
			textoDocumentoN[i] = toupper(textoDocumentoN[i]);
		}
		
		// Se detectan los tokens (palabras) en textoDocumentoN
		palabrasEnDocumento = strtok(textoDocumentoN, " ");
		
		// Se cuenta el numero de palabras en textoDocumentoN
		while (palabrasEnDocumento != NULL) {
			totalPalabrasEnDocumento++;
			palabrasEnDocumento = strtok(NULL, " ");
		}
		
		// Guarda informacion de los documentos
		Documento documentos[NUM_DOCUMENTOS];
		
		if(proceso == 0) {
			
			// Se inicializan las estructuras en donde guardaremos informacion de los documentos
			for(i = 0; i < NUM_DOCUMENTOS; i++) {
				documentos[i].idDocumento = i;
				documentos[i].totalPalabras = 0;
			}
			
			// Se guarda el numero de palabras en documento 0 (estamos en rank = 0)
			documentos[0].totalPalabras = totalPalabrasEnDocumento;
			
			// Rank 0 recibe el numero de palabras en los demas documentos asignados a los demas ranks (ej. rank = 1 -> documento 1)
			for(i = 1; i < NUM_DOCUMENTOS; i++) {
				MPI_Recv(&documentos[i].totalPalabras, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			
			int indicePalabraUnicaEnDocumentos;
			
			// Se obtiene todas las palabras del corpus de documentos (sin repetir)
			for (i = 0; i < NUM_DOCUMENTOS; i++) {
				strcpy(textoDocumentoN, textoDocumentos[i]);
				for (j = 0; textoDocumentoN[j] != '\0'; j++) {
					textoDocumentoN[j] = toupper(textoDocumentoN[j]);
				}
				palabrasEnDocumento = strtok(textoDocumentoN, " ");
				while (palabrasEnDocumento != NULL) {
					indicePalabraUnicaEnDocumentos = -1;
					for (j = 0; j < totalPalabrasUnicasEnDocumentos; j++) {
						if (strcmp(palabrasUnicasEnDocumentos[j].palabra, palabrasEnDocumento) == 0) {
							indicePalabraUnicaEnDocumentos = j;
							break;
						}
					}
					if (indicePalabraUnicaEnDocumentos == -1) {
						indicePalabraUnicaEnDocumentos = totalPalabrasUnicasEnDocumentos;
						
						// Se guarda la palabra
						palabrasUnicasEnDocumentos[indicePalabraUnicaEnDocumentos].palabra =  strdup(palabrasEnDocumento);
						
						// Se guarda la longitud de la palabra
						palabrasUnicasEnDocumentos[indicePalabraUnicaEnDocumentos].longitud =  strlen(palabrasEnDocumento);
						
						// Se guarda el total de palabras del corpus de documentos (sin repetir)
						totalPalabrasUnicasEnDocumentos++;
					}
					palabrasEnDocumento = strtok(NULL, " ");
				}
			}
			
			// Se manda desde rank = 0 a los demas ranks cada una de las palabras del corpus de documentos
			for (i = 1; i < NUM_DOCUMENTOS; i++) {
				for (j = 0; j < totalPalabrasUnicasEnDocumentos; j++) {
					MPI_Send(palabrasUnicasEnDocumentos[j].palabra, 100, MPI_CHAR, i, j, MPI_COMM_WORLD);
				}
			}
		}
		else{
			// Se envia al rank 0 el total de palabras en los documentos 1 y 2 (estamos en rank = 1 o rank = 2)
			MPI_Send(&totalPalabrasEnDocumento, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		
		// Se envia a todos los ranks desde rank 0 el total de palabras del corpus de documentos (sin repetir)
		MPI_Bcast(&totalPalabrasUnicasEnDocumentos, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		int conteoPorPalabraPorDocumentoOrigen[totalPalabrasUnicasEnDocumentos][NUM_DOCUMENTOS];
		int conteoPorPalabraPorDocumentoDestino[totalPalabrasUnicasEnDocumentos][NUM_DOCUMENTOS];
		char palabraContarAparicion[totalPalabrasUnicasEnDocumentos][100];
		
		// Se inicializa la matriz de origen y destino para almacenar el conteo de cada palabra en cada documento
		for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
			for(j = 0; j < NUM_DOCUMENTOS; j++){
				conteoPorPalabraPorDocumentoOrigen[i][j] = 0;
				conteoPorPalabraPorDocumentoDestino[i][j] = 0;
			}
		}
		
		// Los ranks reciben del rank maestro = 0 cada una de las palabras del corpus de documentos
		if(proceso != 0){
			for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
				MPI_Recv(&palabraContarAparicion[i], 100, MPI_CHAR, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		else{
			// Rank 0 copia las palabras del corpus de documentos en palabraContarAparicion para trabajar
			for(i = 0; i<totalPalabrasUnicasEnDocumentos; i++){
				strcpy(palabraContarAparicion[i], palabrasUnicasEnDocumentos[i].palabra);
			}
		}
		
		// Se cuenta el numero de veces que aparece cada palabra del corpus de documentos en cada documento
		for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
			strcpy(textoDocumentoN, textoDocumentos[proceso]);
			for (j = 0; textoDocumentoN[j] != '\0'; j++) {
				textoDocumentoN[j] = toupper(textoDocumentoN[j]);
			}
			palabrasEnDocumento = strtok(textoDocumentoN, " ");
			while (palabrasEnDocumento != NULL){
				if (strcmp(palabraContarAparicion[i], palabrasEnDocumento) == 0) {
					conteoPorPalabraPorDocumentoOrigen[i][proceso]++;
				}
				palabrasEnDocumento = strtok(NULL, " ");
			}
		}
		
		// Se suman las matrices de origen (1 por rank) para almacenar el resultado en la matriz de destino (recibe rank 0)
		for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
			MPI_Reduce(&conteoPorPalabraPorDocumentoOrigen[i], &conteoPorPalabraPorDocumentoDestino[i], NUM_DOCUMENTOS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);	
		}
		bool palabraEnDocumento;
		
		if(proceso == 0){
			
			// Rank 0 almacena el conteo por documento, los calculos de tf, idf, tfidf, en su arreglo de estructuras (Palabra) palabrasUnicasEnDocumentos
			for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
				palabrasUnicasEnDocumentos[i].conteoDocumentosConPalabra = 0;
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					palabrasUnicasEnDocumentos[i].conteoAparicionPalabraPorDocumento[j] = conteoPorPalabraPorDocumentoDestino[i][j];
					if(palabrasUnicasEnDocumentos[i].conteoAparicionPalabraPorDocumento[j] >= 1){
						palabrasUnicasEnDocumentos[i].conteoDocumentosConPalabra++;
					}
					palabrasUnicasEnDocumentos[i].tf[j] = calcularTF(palabrasUnicasEnDocumentos[i], documentos[j]);
				}
				palabrasUnicasEnDocumentos[i].idf = calcularIDF(palabrasUnicasEnDocumentos[i]);
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					palabrasUnicasEnDocumentos[i].tfidf[j] = calcularTFIDF(palabrasUnicasEnDocumentos[i], j);
				}
			}
			
			// Se imprime informacion de los documentos y las palabras del corpus de documentos
			printf("MIS DOCUMENTOS:\n");
			for(i = 0; i < NUM_DOCUMENTOS; i++){
				strcpy(textoDocumentoN, textoDocumentos[i]);
				for (j = 0; textoDocumentoN[j] != '\0'; j++) {
					textoDocumentoN[j] = toupper(textoDocumentoN[j]);
				}
				printf("  Documento %i:\n    ID: %i\n    Texto: %s\n    Total de palabras: %i\n", i, documentos[i].idDocumento, textoDocumentoN, documentos[i].totalPalabras);
			}
			printf("PALABRAS IDENTIFICADAS:\n");
			for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
				printf("  Palabra %i:\n    Texto: %s\n    Longitud: %i\n    Numero de documentos que la contienen: %i\n    Conteo en documento", i, palabrasUnicasEnDocumentos[i].palabra, palabrasUnicasEnDocumentos[i].longitud, palabrasUnicasEnDocumentos[i].conteoDocumentosConPalabra);
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					printf(" - Documento %i: %i", j, palabrasUnicasEnDocumentos[i].conteoAparicionPalabraPorDocumento[j]);
				}
				printf("\n    TF:\n");
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					printf("      TF en documento %i: %i / %i = %.4f\n", j,  palabrasUnicasEnDocumentos[i].conteoAparicionPalabraPorDocumento[j], documentos[j].totalPalabras, palabrasUnicasEnDocumentos[i].tf[j]);
				}
				printf("    IDF: log(%i / %i) = %.4f\n    TF-IDF:\n", NUM_DOCUMENTOS, palabrasUnicasEnDocumentos[i].conteoDocumentosConPalabra, palabrasUnicasEnDocumentos[i].idf);
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					printf("      TF-IDF en documento %i: %.4f * %.4f = %.4f\n", j, palabrasUnicasEnDocumentos[i].tf[j], palabrasUnicasEnDocumentos[i].idf, palabrasUnicasEnDocumentos[i].tfidf[j]);
				}
			}
		}
		MPI_Finalize();
	}
	return 0;
}
