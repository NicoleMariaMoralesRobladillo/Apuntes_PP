#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void inicializar_vector(int *v, int n);
void imprimir_vector(int *v, int ini, int fin);
int compara (const void *a, const void *b){
	return(*(int*)a - *(int*)b);
}

#define N 20000000
int a[N], c[N];
int *b;

int main(int argc, char **argv){
        MPI_Init(NULL, NULL);
        int proceso, procesos, longitud;
        char equipo[MPI_MAX_PROCESSOR_NAME];
        MPI_Comm_size(MPI_COMM_WORLD, &procesos);
        MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
        MPI_Get_processor_name(equipo, &longitud);
	printf("Proceso %i de %i\n", proceso, procesos);
        if(procesos!=2){
		MPI_Abort(MPI_COMM_WORLD, 10);
		return 0;
	}
	double inicio, duracion;
	b=a+N/2;
	if(proceso==0){
		inicializar_vector(a, N);
		memcpy(c,a,sizeof(int)*N);

		inicio = MPI_Wtime();
		qsort(c,N,sizeof(int),compara);
		duracion = MPI_Wtime() - inicio;
		printf("Secuencial, tardo %lf segundos en ordenar\n", duracion);

		inicio = MPI_Wtime();
		MPI_Send(b, N/2, MPI_INT, 1, 0, MPI_COMM_WORLD);
		qsort(a,N/2,sizeof(int),compara);
		MPI_Recv(b,N/2,MPI_INT,1,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		duracion = MPI_Wtime() - inicio;
		printf("Paralelo, tardo %lf segundos en ordenar\n", duracion);
	} else if(proceso==1){
		MPI_Recv(b, N/2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		qsort(b,N/2,sizeof(int),compara);
		MPI_Send(b,N/2,MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
        MPI_Finalize();
}

void inicializar_vector(int *v, int n){
	for(int i=0;i<n;i++){
		v[i] = (int)(10000.0 * (rand()*1.0/RAND_MAX));
	}
}

void imprimir_vector(int *v, int ini, int fin){
	for(int i=ini;i<fin;i++){
		printf("%i ", v[i]);
	}
	printf("\n");
}
