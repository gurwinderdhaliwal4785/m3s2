#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void generateRandomVector(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    unsigned long vector_size = 100000000;
    int *vec1, *vec2, *vec3;

    if (rank == 0) {
        srand(time(NULL));
        vec1 = (int *)malloc(vector_size * sizeof(int));
        vec2 = (int *)malloc(vector_size * sizeof(int));
        vec3 = (int *)malloc(vector_size * sizeof(int));
        generateRandomVector(vec1, vector_size);
        generateRandomVector(vec2, vector_size);
    }

    MPI_Bcast(&vector_size, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    unsigned long local_vector_size = vector_size / num_procs;
    int *local_vec1 = (int *)malloc(local_vector_size * sizeof(int));
    int *local_vec2 = (int *)malloc(local_vector_size * sizeof(int));
    int *local_vec3 = (int *)malloc(local_vector_size * sizeof(int));

    MPI_Scatter(vec1, local_vector_size, MPI_INT, local_vec1, local_vector_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(vec2, local_vector_size, MPI_INT, local_vec2, local_vector_size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int i = 0; i < local_vector_size; i++) {
        local_vec3[i] = local_vec1[i] + local_vec2[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    MPI_Gather(local_vec3, local_vector_size, MPI_INT, vec3, local_vector_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Time taken by function: %f microseconds\n", (end_time - start_time) * 1e6);
    }

    MPI_Finalize();
    return 0;
}
