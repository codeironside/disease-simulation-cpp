
#include <iostream>
#include "simulation.h"
#include <string>
int main()
{
    std::cout << "Loading Disease Simulation..." << std::endl;
    Simulation sim = Simulation();
    sim.start();
    return 0;
}
