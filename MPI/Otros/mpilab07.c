#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void llenar(int* vector, int longitud) {
	int i;
	for (i = 0; i < longitud; i++) {
		vector[i] = i + 11;
	}
}

void print(char* str, int proceso, int* vector) {
	printf("%s del envio del array, el proceso %d tiene: {%d, %d, %d, %d, %d, %d, %d, %d}\n", str, proceso, vector[0], vector[1], vector[2], vector[3], vector[4], vector[5], vector[6], vector[7]);
}

#define MAX 8

int main(int argc, char** argv) {
	int proceso = -1, procesos = -1;
	int vector[MAX] = {0};

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    	MPI_Comm_size(MPI_COMM_WORLD, &procesos);

	if (proceso == 0) llenar(vector, MAX);

	print("ANTES", proceso, vector);

	// master process sends array to every process
	if (proceso == 0) {
		for (int i = 1; i < procesos; i++) {
			MPI_Send(&vector, MAX, MPI_INT, i, 1, MPI_COMM_WORLD);
                }
	}

	else {
                MPI_Recv(&vector, MAX, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

    	print("DESPUES", proceso, vector);
 	MPI_Finalize();
	return 0;
}
