#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h> //Para lanzar exception

#define NUM_DOCUMENTOS 3

typedef struct {
	int idDocumento;
	char* nombreDocumento;
	char* textoDocumento;
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
	if(procesos < NUM_DOCUMENTOS) {
		MPI_Abort(MPI_COMM_WORLD, 10);
		return 0;
	}
	else {
		int i, j;
		Documento documentos[NUM_DOCUMENTOS];
		FILE *file;
		char* nombresArchivos[] = {"documento0.txt", "documento1.txt", "documento2.txt"};
		char* nombreArchivo;
		for(i = 0; i < NUM_DOCUMENTOS; i++) {
			documentos[i].idDocumento = i;
			documentos[i].totalPalabras = 0;
			nombreArchivo = (char*) malloc(strlen(nombresArchivos[i])*sizeof(char));
			nombreArchivo = nombresArchivos[i];
			file = fopen(nombreArchivo, "r");
			assert(file != NULL);
			fgets(documentos[i].textoDocumento, 100, file);
			fclose(file);
			fflush(stdout);
		}
		char textoDocumentos[NUM_DOCUMENTOS][100];
		char nombreDocumentoN[100];
		int totalPalabrasEnDocumento = 0;
		char* palabrasEnDocumento;
		int totalPalabrasUnicasEnDocumentos = 0;
		Palabra palabrasUnicasEnDocumentos[100];
		char textoDocumentoN[100];
		strcpy(textoDocumentoN, textoDocumentos[proceso]);
		for (i = 0; textoDocumentoN[i] != '\0'; i++) {
			textoDocumentoN[i] = toupper(textoDocumentoN[i]);
		}
		palabrasEnDocumento = strtok(textoDocumentoN, " ");
		while (palabrasEnDocumento != NULL) {
			totalPalabrasEnDocumento++;
			palabrasEnDocumento = strtok(NULL, " ");
		}
		if(proceso == 0) {
			documentos[0].totalPalabras = totalPalabrasEnDocumento;
			for(i = 1; i < NUM_DOCUMENTOS; i++) {
				MPI_Recv(&documentos[i].totalPalabras, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			int indicePalabraUnicaEnDocumentos;
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
						palabrasUnicasEnDocumentos[indicePalabraUnicaEnDocumentos].palabra =  strdup(palabrasEnDocumento);
						palabrasUnicasEnDocumentos[indicePalabraUnicaEnDocumentos].longitud =  strlen(palabrasEnDocumento);
						totalPalabrasUnicasEnDocumentos++;
					}
					palabrasEnDocumento = strtok(NULL, " ");
				}
			}
			for (i = 1; i < NUM_DOCUMENTOS; i++) {
				for (j = 0; j < totalPalabrasUnicasEnDocumentos; j++) {
					MPI_Send(palabrasUnicasEnDocumentos[j].palabra, 100, MPI_CHAR, i, j, MPI_COMM_WORLD);
				}
			}
		}
		else{
			MPI_Send(&totalPalabrasEnDocumento, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		MPI_Bcast(&totalPalabrasUnicasEnDocumentos, 1, MPI_INT, 0, MPI_COMM_WORLD);
		int conteoPorPalabraPorDocumentoOrigen[totalPalabrasUnicasEnDocumentos][NUM_DOCUMENTOS];
		int conteoPorPalabraPorDocumentoDestino[totalPalabrasUnicasEnDocumentos][NUM_DOCUMENTOS];
		char palabraContarAparicion[totalPalabrasUnicasEnDocumentos][100];
		for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
			for(j = 0; j < NUM_DOCUMENTOS; j++){
				conteoPorPalabraPorDocumentoOrigen[i][j] = 0;
				conteoPorPalabraPorDocumentoDestino[i][j] = 0;
			}
		}
		if(proceso != 0){
			for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
				MPI_Recv(&palabraContarAparicion[i], 100, MPI_CHAR, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		else{
			for(i = 0; i<totalPalabrasUnicasEnDocumentos; i++){
				strcpy(palabraContarAparicion[i], palabrasUnicasEnDocumentos[i].palabra);
			}
		}
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
		for(i = 0; i < totalPalabrasUnicasEnDocumentos; i++){
			MPI_Reduce(&conteoPorPalabraPorDocumentoOrigen[i], &conteoPorPalabraPorDocumentoDestino[i], NUM_DOCUMENTOS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);	
		}
		if(proceso == 0){
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
				printf("  Palabra %i:\n    Texto: %s\n    Longitud: %i\n    Numero de documentos que la contienen: %i\n    Conteo por documento:\n", i, palabrasUnicasEnDocumentos[i].palabra, palabrasUnicasEnDocumentos[i].longitud, palabrasUnicasEnDocumentos[i].conteoDocumentosConPalabra);
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					printf("      Conteo en documento %i: %i\n", j, palabrasUnicasEnDocumentos[i].conteoAparicionPalabraPorDocumento[j]);
				}
				printf("    TF:\n");
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					printf("      TF en documento %i: %i / %i = %.2f\n", j,  palabrasUnicasEnDocumentos[i].conteoAparicionPalabraPorDocumento[j], documentos[j].totalPalabras, palabrasUnicasEnDocumentos[i].tf[j]);
				}
				printf("    IDF: log(%i / %i) = %.2f\n", NUM_DOCUMENTOS, palabrasUnicasEnDocumentos[i].conteoDocumentosConPalabra, palabrasUnicasEnDocumentos[i].idf);
				printf("    TF-IDF:\n");
				for(j = 0; j < NUM_DOCUMENTOS; j++){
					printf("      TF-IDF en documento %i: %.2f * %.2f = %.2f\n", j, palabrasUnicasEnDocumentos[i].tf[j], palabrasUnicasEnDocumentos[i].idf, palabrasUnicasEnDocumentos[i].tfidf[j]);
				}
			}
		}
		MPI_Finalize();
	}
	return 0;
}
