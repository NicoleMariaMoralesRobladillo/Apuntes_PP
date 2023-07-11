#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> //Para lanzar exception

int vector[20];

int main(int argc, char** argv) {
        int respuesta = 0;
        int procesos = 0, proceso = 0;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &procesos);
        MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

	if (proceso == 0) {
		for (int i = 20; i > 0; i--) {
			vector[20 - i] = i;
		}
	}

	MPI_Bcast(vector, 20, MPI_INT, 0, MPI_COMM_WORLD);

	int bloque = 20/procesos;

	int suma = 0;

	for (int i = bloque*proceso; i < bloque*(proceso+1); i++) {
		suma = suma + vector[i];
	}

	printf("La suma para el proceso %d es %d\n", proceso, suma);

        MPI_Finalize();

        return 0;
}
