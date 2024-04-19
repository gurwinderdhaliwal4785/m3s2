#include <mpi.h>
#include <stdio.h>
#include <string.h> // Fixed typo in the include statement
#define MASTER 0
#define MESSAGE_TAG 0 // Corrected the macro name

int main(int argc, char *argv[]) {
    int rank, size;
    char message[100]; // Buffer to hold the message
    MPI_Init(&argc, &argv); // Corrected typo in MPI_Init function call
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER) {
        // If the process is the master, send "Hello World!" message to all workers
        strcpy(message, "Hello World!"); // Fixed typo in strcpy function name
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, MESSAGE_TAG, MPI_COMM_WORLD); // Corrected MPI_COMM_WORLD spelling
        }
    } else {
        // If the process is a worker, receive the message from the master
        MPI_Recv(message, 100, MPI_CHAR, MASTER, MESSAGE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Corrected MPI_COMM_WORLD spelling
        printf("Worker %d received message: %s\n", rank, message);
    }

    // Broadcast a message from the master to all workers
    MPI_Bcast(message, 100, MPI_CHAR, MASTER, MPI_COMM_WORLD);
    printf("Process %d received broadcasted message: %s\n", rank, message);

    MPI_Finalize();
    return 0; // Corrected syntax error: replaced colon with semicolon
}
