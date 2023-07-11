#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int vector[20];

int main(int argc, char **argv){
	int respuesta = 0;
	int proceso, procesos;
	int suma = 0, minimo=1000000;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

	respuesta = (proceso+1)*(proceso+1);
	printf("Proceso %d respuesta %d\n", proceso, respuesta);

	MPI_Reduce(&respuesta, &suma, 1, MPI_INT, MPI_SUM, 1, MPI_COMM_WORLD);
	MPI_Reduce(&respuesta, &minimo, 1, MPI_INT, MPI_MIN, 1, MPI_COMM_WORLD);

	if(proceso == 1) {
		printf("Proceso %d suma %d\n", proceso, suma);
		printf("Proceso %d minimo %d\n", proceso, minimo);
	}

	MPI_Finalize();
}
