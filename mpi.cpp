#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);





    std::cout << "Task 1: Hello World " << rank << std::endl;

    if (size >= 2) {
        if (rank < 2) {

            int received_number;
            srand(time(NULL) + rank);
            int local_number = rand() % 100;
            if (rank == 0) {
                MPI_Send(&local_number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&received_number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else if (rank == 1) {
                MPI_Recv(&received_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&local_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
            int sum = local_number + received_number;
            std::cout << "Task 2: Process " << rank << " has number " << local_number
                << ", received " << received_number << ", sum: " << sum << std::endl;
        }



        int total_sum = 0;
        srand(time(NULL) + rank);
        int local_number = rand() % 100;
        if (rank == 0) {
            total_sum += local_number;
            for (int i = 1; i < size; i++) {
                int received;
                MPI_Recv(&received, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                total_sum += received;
            }
            for (int i = 1; i < size; i++) {
                MPI_Send(&total_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            std::cout << "Task 3: Process " << rank << " had number " << local_number
                << ",calculate sum: " << total_sum << std::endl;
        }
        else {
            MPI_Send(&local_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            // ∆дут ответа
            MPI_Recv(&total_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task 3: Process " << rank << " had number " << local_number
                << ",sum: " << total_sum << std::endl;
        }
       
    }
    else { std::cout << "Need more processes"; }

    srand(time(NULL) + rank);
    int local_number = rand() % 100;
    int ring_sum = local_number;
    int next_rank = (rank + 1) % size;
    int prev_rank = (rank - 1 + size) % size;

    for (int step = 0; step < size - 1; step++) {
        int received;


        MPI_Send(&local_number, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        MPI_Recv(&received, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        ring_sum += received;
        local_number = received;

    }

    std::cout << "Task 4: Process " << rank << " had number "
        << local_number << ", sum: " << ring_sum << std::endl;

    MPI_Finalize();
    return 0;
}
