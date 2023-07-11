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
        if(procesos<2){
		MPI_Abort(MPI_COMM_WORLD, 10);
		return 0;
	}
	int mensaje;
	if(proceso==0){
		mensaje = 1000;
		MPI_Send(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("Envie mensaje\n");
	} else if(proceso==1){
		int recibido = -1;
		MPI_Recv(&recibido, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Recibi %i del proceso 0. mensaje %i\n", recibido, mensaje);
	}
        MPI_Finalize();
}
