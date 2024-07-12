// #include <iostream>
// #include <mpi.h>
// #include "simulation.h"
// #include <string>

// int main(int argc, char *argv[]) {
//     MPI_Init(&argc, &argv);
//     int rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     if (rank == 0) {
//         std::cout << "Loading Disease Simulation on process " << rank << "..." << std::endl;
//     }
//     Simulation sim = Simulation();
//     sim.start();
//     MPI_Finalize();
//     return 0;
// }

#include <iostream>
#include <mpi.h>
#include "simulation.h"
#include <string>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string input_file = "disease_in.ini"; // Adjust as per your configuration
    Simulation sim(input_file);

    if (rank == 0) {
        std::cout << "Loading Disease Simulation on process " << rank << "..." << std::endl;
    }

    sim.start();

    MPI_Finalize();
    return 0;
}

