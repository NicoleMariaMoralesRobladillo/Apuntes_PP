/* Single program multiple data (SPMD)
 * pattern using basic MPI commands.
 * Compile: mpicc SPMD_know_hostname.c -o SPMD_know_hostname
 * Usage: mpirun -np 3 ./SPMD_know_hostname
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
	int id = -1, numProcesses = -1, length = -1;
	char myHostName[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
	MPI_Get_processor_name (myHostName, &length);
	printf("Greetings from process %i of %i on %s\n", id, numProcesses, myHostName);
	MPI_Finalize();
	return 0;
}
