/* parallelLoopChunksOf1.c
 * ... illustrates the parallel for loop pattern in MPI
 *	in which processes perform the loop's iterations in 'chunks'
 *      of size 1 (simple, and useful when loop iterations
 *      do not access memory/cache locations) ...
 * Note this is much simpler than the 'equal chunks' loop.
 * Compile: mpicc Descomposicion_Datos_Trozos_1_parallel_for.c -o Descomposicion_Datos_Trozos_1_parallel_for
 * Usage: mpirun -np 4 ./Descomposicion_Datos_Trozos_1_parallel_for
 */

#include <stdio.h>  // printf()
#include <mpi.h>    // MPI

int main(int argc, char** argv) {
	const int REPS = 8;
	int id = -1, numProcesses = -1, i = -1;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

	if (numProcesses > REPS) {
		if (id == 0) {
			printf("Please run with -np less than or equal to %d\n.", REPS);
		}
	}
	else {
		for (i = id; i < REPS; i += numProcesses) {
			printf("Process %d is performing iteration %d\n", id, i);
		}
	}

	MPI_Finalize();
	return 0;
}

