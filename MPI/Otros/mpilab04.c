#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> //Para lanzar exception

int main(int argc, char** argv) {
	int respuesta = 0;
	int procesos = 0, proceso = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

	if (proceso == 0) {
		FILE *file = fopen("mpilab04.txt", "r");
		assert(file != NULL); //Si no se cumple se lanza la exception
		fscanf(file, "%d", &respuesta);
		fclose(file);
		fflush(stdout);
	}

	printf("ANTES del Broadcast, respuesta de proceso %d = %d\n", proceso, respuesta);
	
	fflush(stdout);

	MPI_Barrier(MPI_COMM_WORLD);
	
	MPI_Bcast(&respuesta, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	printf("DESPUES del Broadcast, respuesta de proceso %d = %d\n", proceso, respuesta);

	fflush(stdout);

	MPI_Finalize();

	return 0;
}
