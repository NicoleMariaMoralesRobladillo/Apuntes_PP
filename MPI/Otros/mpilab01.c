#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"
int main(int argc, char **argv){
	MPI_Init(NULL, NULL);
	int proceso, procesos, longitud;
	char equipo[MPI_MAX_PROCESSOR_NAME];
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
	MPI_Get_processor_name(equipo, &longitud);
	printf("Estoy en el equipo %s soy el proceso %i de %i procesos\n", equipo, proceso, procesos);
	MPI_Finalize();
}
