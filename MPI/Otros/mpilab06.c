#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MASTER 0

void getInput(int argc, char* argv[], int proceso, int* respuesta) {

    if (proceso == MASTER){  // master process
        if (argc == 2){
             *respuesta = atoi(argv[1]);
        }
    }
    MPI_Bcast(respuesta, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    int respuesta = 0, longitud = 0;
    int proceso = 0;

    char miHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Get_processor_name (miHostName, &longitud);

    printf("ANTES del broadcast, el proceso %d en el host '%s' tiene la respuesta = %d\n", proceso, miHostName, respuesta);

    getInput(argc, argv, proceso, &respuesta);

    printf("DESPUES del broadcast, el proceso %d en el host '%s' tiene la respuesta = %d\n", proceso, miHostName, respuesta);

    MPI_Finalize();

    return 0;
}
