#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define N 5

void imprimir(int pro, char* tit, int* vec, int tam){
	printf("Proceso %i[%s] = [", pro, tit);
	for(int i=0;i<tam;i++)
		printf("%3d", vec[i]);
	printf("]\n");
	fflush(stdout);
}

int main(int argc, char **argv){
        int respuesta = 0;
        int proceso, procesos;
	int fuente[N]={0};
	int destino[N]={0};

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

	if(proceso == 0){
		printf("Antes de la reduccion\n");
		imprimir(proceso, "destino", destino, N);
	}

	for(int i=0;i<N;i++){
		fuente[i] = proceso * i;
	}

        MPI_Reduce (&fuente, &destino, N, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        if(proceso == 0) {
                printf("Despues de la reduccion\n");
                imprimir(proceso, "destino", destino, N);
        }
	else{
		imprimir(proceso, "fuente", fuente, N);
	}

        MPI_Finalize();
}
