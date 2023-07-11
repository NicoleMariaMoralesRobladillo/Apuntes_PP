#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <assert.h>
#include "mpi.h"

#define MAX_PALABRAS_DOCUMENTO 10
#define MAX_WORD_LENGTH 20
#define NUMERO_DOCUMENTOS 3
typedef struct
{
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

typedef struct
{
    char word[MAX_WORD_LENGTH];
    double tf[NUMERO_DOCUMENTOS];
    double idf;
    double matriz[NUMERO_DOCUMENTOS];
} WordCountPerDocument;
int compareWordCount(const void *a, const void *b)
{
    return strcmp(((WordCount *)a)->word, ((WordCount *)b)->word);
}
void reduce(int n, WordCount *wc)
{
    for (int i = 1; i < n; i++)
    {
        if (strcmp(wc[i].word, wc[i - 1].word) == 0)
        {
            wc[i].count += wc[i - 1].count;
            wc[i - 1].count = 0;
        }
    }
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char document[20];

    WordCount *wc1[MAX_PALABRAS_DOCUMENTO];
    WordCount *wc2[MAX_PALABRAS_DOCUMENTO];
    WordCount *wc3[MAX_PALABRAS_DOCUMENTO];

    int numPalabrasDoc1;
    int numPalabrasDoc2;
    int numPalabrasDoc3;

    char texto[MAX_PALABRAS_DOCUMENTO * MAX_WORD_LENGTH];
    char palabras[MAX_PALABRAS_DOCUMENTO][MAX_WORD_LENGTH];
    if (rank != 0)
    {
        // En esta parte se hace la lectura de los docs
        sprintf(document, "doc%d.txt", rank);
        WordCount *wc[MAX_PALABRAS_DOCUMENTO];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            wc[i] = NULL;
        }

        FILE *file = fopen(document, "r");
        fgets(texto, MAX_PALABRAS_DOCUMENTO * MAX_WORD_LENGTH, file);
        texto[strcspn(texto, "\n")] = 0;
        int n = 0;
        char *word = strtok(texto, " ");
        while (word != NULL)
        {
            n++;

            *wc = realloc(*wc, (n) * sizeof(WordCount));
            strcpy((*wc)[n - 1].word, word);
            (*wc)[n - 1].count = 1;
            // printf("N: %d palabra, %s, count %d \n", rank,(*wc)[n - 1].word, (*wc)[n - 1].count );
            word = strtok(NULL, " ");
        }

        qsort(*wc, n, sizeof(WordCount), compareWordCount);

        MPI_Send(*wc, n * sizeof(WordCount), MPI_BYTE, 0, 0, MPI_COMM_WORLD);

        MPI_Send(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    if (rank == 0)
    {
        MPI_Recv(&numPalabrasDoc1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&numPalabrasDoc2, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&numPalabrasDoc3, 1, MPI_INT, 3, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("N: %d %d %d \n", numPalabrasDoc1, numPalabrasDoc2, numPalabrasDoc3);

        *wc1 = malloc(numPalabrasDoc1 * sizeof(WordCount));
        *wc2 = malloc(numPalabrasDoc2 * sizeof(WordCount));
        *wc3 = malloc(numPalabrasDoc3 * sizeof(WordCount));

        MPI_Recv(*wc1, numPalabrasDoc1 * sizeof(WordCount), MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(*wc2, numPalabrasDoc2 * sizeof(WordCount), MPI_BYTE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(*wc3, numPalabrasDoc3 * sizeof(WordCount), MPI_BYTE, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        qsort(*wc1, numPalabrasDoc1, sizeof(WordCount), compareWordCount);
        qsort(*wc2, numPalabrasDoc2, sizeof(WordCount), compareWordCount);
        qsort(*wc3, numPalabrasDoc3, sizeof(WordCount), compareWordCount);

        reduce(numPalabrasDoc1, *wc1);
        reduce(numPalabrasDoc2, *wc2);
        reduce(numPalabrasDoc3, *wc3);

        WordCount *wc1SinDuplicados[3 * MAX_PALABRAS_DOCUMENTO];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            wc1SinDuplicados[i] = NULL;
        }
        int wc1SinDuplicadosLength = 0;
        for (int j = 0; j < numPalabrasDoc1; j++)
        {

            if ((*wc1)[j].count > 0)
            {
                wc1SinDuplicadosLength++;
                *wc1SinDuplicados = realloc(*wc1SinDuplicados, (wc1SinDuplicadosLength) * sizeof(WordCount));
                strcpy((*wc1SinDuplicados)[wc1SinDuplicadosLength - 1].word, (*wc1)[j].word);
                (*wc1SinDuplicados)[wc1SinDuplicadosLength - 1].count = (*wc1)[j].count;
            }
        }

        WordCount *wc2SinDuplicados[3 * MAX_PALABRAS_DOCUMENTO];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            wc2SinDuplicados[i] = NULL;
        }
        int wc2SinDuplicadosLength = 0;
        for (int j = 0; j < numPalabrasDoc2; j++)
        {

            if ((*wc2)[j].count > 0)
            {
                wc2SinDuplicadosLength++;
                *wc2SinDuplicados = realloc(*wc2SinDuplicados, (wc2SinDuplicadosLength) * sizeof(WordCount));
                strcpy((*wc2SinDuplicados)[wc2SinDuplicadosLength - 1].word, (*wc2)[j].word);
                (*wc2SinDuplicados)[wc2SinDuplicadosLength - 1].count = (*wc2)[j].count;
            }
        }

        WordCount *wc3SinDuplicados[3 * MAX_PALABRAS_DOCUMENTO];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            wc3SinDuplicados[i] = NULL;
        }
        int wc3SinDuplicadosLength = 0;
        for (int j = 0; j < numPalabrasDoc3; j++)
        {

            if ((*wc3)[j].count > 0)
            {
                wc3SinDuplicadosLength++;
                *wc3SinDuplicados = realloc(*wc3SinDuplicados, (wc3SinDuplicadosLength) * sizeof(WordCount));
                strcpy((*wc3SinDuplicados)[wc3SinDuplicadosLength - 1].word, (*wc3)[j].word);
                (*wc3SinDuplicados)[wc3SinDuplicadosLength - 1].count = (*wc3)[j].count;
            }
        }

        WordCount *allWC[3 * MAX_PALABRAS_DOCUMENTO];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            allWC[i] = NULL;
        }

        int k = 0;

        for (int j = 0; j < numPalabrasDoc1; j++)
        {

            if ((*wc1)[j].count > 0)
            {
                k++;
                *allWC = realloc(*allWC, (k) * sizeof(WordCount));
                strcpy((*allWC)[k - 1].word, (*wc1)[j].word);
                (*allWC)[k - 1].count = (*wc1)[j].count;
            }
        }
        for (int j = 0; j < numPalabrasDoc2; j++)
        {
            if ((*wc2)[j].count > 0)
            {
                k++;

                *allWC = realloc(*allWC, (k) * sizeof(WordCount));
                strcpy((*allWC)[k - 1].word, (*wc2)[j].word);
                (*allWC)[k - 1].count = (*wc2)[j].count;
            }
        }
        for (int j = 0; j < numPalabrasDoc3; j++)
        {
            if ((*wc3)[j].count > 0)
            {
                k++;
                *allWC = realloc(*allWC, (k) * sizeof(WordCount));
                strcpy((*allWC)[k - 1].word, (*wc3)[j].word);
                (*allWC)[k - 1].count = (*wc3)[j].count;
            }
        }

        /*
        for (int j = 0; j < numPalabrasDoc1; j++){
                    allWC[k++] = wc1[j];

                    printf("WC1   N: %d palabra, %s, count %d \n", rank, (*wc1)[j].word, (*wc1)[j].count);
                    printf("allWC N: %d palabra, %s, count %d \n", k, (*allWC)[k-1].word, (*allWC)[k-1].count);


                }

                for (int j = 0; j < numPalabrasDoc3; j++){
                    allWC[k++] = wc3[j];

                    printf("WC3   N: %d palabra, %s, count %d \n", rank, (*wc3)[j].word, (*wc3)[j].count);
                    printf("allWC N: %d palabra, %s, count %d \n", k, (*allWC)[k-1].word, (*allWC)[k-1].count);

                }
        */
        qsort(*allWC, k, sizeof(WordCount), compareWordCount);
        reduce(k, *allWC);

        WordCount *allWCSinDuplicados[3 * MAX_PALABRAS_DOCUMENTO];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            allWCSinDuplicados[i] = NULL;
        }
        int numeroSinDuplicados = 0;
        for (int j = 0; j < k; j++)
        {
            if ((*allWC)[j].count > 0)
            {
                numeroSinDuplicados++;

                *allWCSinDuplicados = realloc(*allWCSinDuplicados, (numeroSinDuplicados) * sizeof(WordCount));
                strcpy((*allWCSinDuplicados)[numeroSinDuplicados - 1].word, (*allWC)[j].word);
                (*allWCSinDuplicados)[numeroSinDuplicados - 1].count = (*allWC)[j].count;
            }
        }
        // WordCountPerDocument *palabraDocumento[3 * MAX_PALABRAS_DOCUMENTO];

        WordCountPerDocument *countPerDocument[MAX_PALABRAS_DOCUMENTO * 3];
        for (int i = 0; i < MAX_PALABRAS_DOCUMENTO; i++)
        {
            countPerDocument[i] = NULL;
        }
        *countPerDocument = realloc(*countPerDocument, (numeroSinDuplicados) * sizeof(WordCount));

        for (int j = 0; j < numeroSinDuplicados; j++)
        {
            strcpy((*countPerDocument)[j].word, (*allWCSinDuplicados)[j].word);
            int numeroDocumentos;
            (*countPerDocument)[j].tf = {0.0d, 0.0d, 0.0d};
            for (int k = 0; k < wc1SinDuplicadosLength; k++){
                if(strcmp())
            }
        }

        for (int i = 0; i < wc1SinDuplicadosLength; i++)
        {
            printf("N: %d palabra, %s, count %d \n", rank, (*wc1SinDuplicados)[i].word, (*wc1SinDuplicados)[i].count);
        }
        for (int i = 0; i < wc2SinDuplicadosLength; i++)
        {
            printf("N: %d palabra, %s, count %d \n", rank, (*wc2SinDuplicados)[i].word, (*wc2SinDuplicados)[i].count);
        }
        for (int i = 0; i < wc3SinDuplicadosLength; i++)
        {
            printf("N: %d palabra, %s, count %d \n", rank, (*wc3SinDuplicados)[i].word, (*wc3SinDuplicados)[i].count);
        }
        printf("==================\n");
        for (int i = 0; i < numeroSinDuplicados; i++)
        {
            printf("N: %d palabra, %s, count %d \n", rank, (*allWCSinDuplicados)[i].word, (*allWCSinDuplicados)[i].count);
        }
    }
    MPI_Finalize();
}
