/* parallelLoopEqualChunks.c
 * ... illustrates the parallel for loop pattern in MPI
 *	in which processes perform the loop's iterations in equal-sized 'chunks'
 *	(preferable when loop iterations access memory/cache locations) ...
 * Compile: mpicc Descomposicion_Datos_Trozos_iguales_parallel_for.c -o Descomposicion_Datos_Trozos_iguales_parallel_for
 * Usage: mpirun -np 4 ./Descomposicion_Datos_Trozos_iguales_parallel_for
 */

#include <stdio.h> // printf()
#include <mpi.h>   // MPI

int main(int argc, char** argv) {
    const int REPS = 8;                      // repetitions in a loop
    int id = -1, numProcesses = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    // In this example, ensure that the REPS can ben evenly divided by the
    // number of processors and that the number of processes doesn't exceed REPS.
    // If either is the case, stop.
    if ((REPS % numProcesses) == 0 && numProcesses <= REPS) {

      int chunkSize = REPS / numProcesses;      // find chunk size
      int start = id * chunkSize;               // find starting index
      int stop = start + chunkSize;             // find stopping index

      for (int i = start; i < stop; i++) {      // iterate through our range
          printf("Process %d is performing iteration %d\n", id, i);
      }

    } else {
      if (id == 0) {
          printf("Please run with -np divisible by and less than or equal to %d\n.", REPS);
      }
    }

    MPI_Finalize();
    return 0;
}

