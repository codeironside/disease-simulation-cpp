
#include <iostream>
#include "simulation.h"
#include <string>
#include <iostream>
#include <mpi.h>
#include "simulation.h"
#include <string>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cout << "Loading Disease Simulation on " << size << " processors..." << std::endl;
    }

    Simulation sim = Simulation();

    sim.start();

    MPI_Finalize();
    return 0;
}

// int main()
// {
//     std::cout << "Loading Disease Simulation..." << std::endl;
//     Simulation sim = Simulation();
//     sim.start();
//     return 0;
// }
