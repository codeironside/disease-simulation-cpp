#include "simulation.h"
#include "./include/INIReader.h"
#include <iostream>
#include <vector>

#include <fstream>
#include <string>

Simulation::Simulation(std::string in_file) {
    input_file = in_file;
    INIReader reader(input_file);
    if (reader.ParseError() < 0) {
        std::cout << "Can't load " << input_file << "\n";
        return;
    }
    int num_populations = reader.GetInteger("global", "num_populations", 0);
    simulation_run = reader.GetInteger("global", "simulation_runs", 0);
    simulation_name = reader.Get("global", "simulation_name", "");

    std::string name = reader.Get("disease", "name", "");
    int duration = reader.GetInteger("disease", "duration", 0);
    double transmission_probability = reader.GetReal("disease","transmissability", 0.0);
    disease = Disease(name, duration, transmission_probability);
    std::string section_name = "population_2";
    int population_size = reader.GetInteger(section_name, "size", 100);
    bool patient_0 = reader.GetBoolean(section_name,"patient_0", false);
    std::string population_name = reader.Get(section_name, "name", "");
    double population_vaccination_rate = reader.GetReal(section_name, "vaccination_rate", 0.0);
    Population population;
    population.name = population_name;
    population.vaccination_rate = population_vaccination_rate;
    population.people.resize(population_size);
    int initial_infections= patient_0? 1: 1;

    population.random_infection(disease, initial_infections);
    world.add_population(population);

 
}

void Simulation::reset_world() {
    world.population.clear();
    INIReader reader(input_file);
    if (reader.ParseError() < 0) {
        std::cout << "Can't load " << input_file << "\n";
        return;
    }
    int num_populations = reader.GetInteger("global", "num_populations", 0);

    std::string name = reader.Get("disease", "name", "");
    int duration = reader.GetInteger("disease", "duration", 0);
    double transmission_probability = reader.GetReal("disease","transmissability", 0.0);
    disease = Disease(name, duration, transmission_probability);
    std::string section_name = "population_2";
    int population_size = reader.GetInteger(section_name, "size", 100);
    std::string population_name = reader.Get(section_name, "name", "");
    double population_vaccination_rate = reader.GetReal(section_name, "vaccination_rate", 0.0);
    Population population;
    population.name = population_name;
    population.vaccination_rate = population_vaccination_rate;
    population.people.resize(population_size);
    bool patient_0 = reader.GetBoolean(section_name,"patient_0", false);
    int initial_infections= patient_0? 1: 1;
    population.random_infection(disease, initial_infections);
    world.add_population(population);

}


void Simulation::start() {
    std::vector<int> timesteps;
    std::vector<int> num_vaccinated;
    std::vector<int> num_recovered;
    std::vector<int> num_susceptible;
    std::ofstream vaccination_vs_recovered_file("vaccination_vs_recovered.csv");
    vaccination_vs_recovered_file << "Vaccination_Rate"<<","<<"Number_of_Recovered\n";
    for (double vaccination_rate = 0.0; vaccination_rate <= 1.0; vaccination_rate += 0.1) {
    
        reset_world();
        for (Population& pop : world.population) {
            pop.vaccination_rate = vaccination_rate;
            pop.random_vaccination();
        }
        int timestep;
        for (timestep = 0; world.count_infected() > 0; ++timestep) {
        std::ofstream details_file("disease_details.csv");
        details_file << "Timestep,Name, Infected, Recovered, Susceptible, Vaccinated\n";
        for (int run = 0; run < simulation_run; ++run) {
            reset_world(); 
            int timestep;
            for (timestep = 0; world.count_infected() > 0; ++timestep) {
                        for (Population& population : world.population) {
                for (Person& p : population.people) {
                    if (p.state == State::Infectious) {
                        for (int i = 0; i < 6; ++i) {
                            int index = rand() % population.people.size();
                            p.touch(population.people[index], disease);
                        }
                    }
                }
                population.one_more_day();
            };
                Population& pop1 = world.select_random_population();
                Population& pop2 = world.select_random_population();
                for (int i = 0; i < 5; ++i) {
                    Person& person1 = pop1.people[rand() % pop1.people.size()];
                    Person& person2 = pop2.people[rand() % pop2.people.size()];
                    person1.touch(person2, disease);
                    person2.touch(person1, disease);
                }

                    for (const Population& population : world.population) {
                        // population.random_vaccination();
                        details_file << timestep << ", " << population.name << ", "
                                    << population.count_infected() << ", "
                                    << population.count_recovered() << ", "
                                    << population.count_susceptible() << ", "
                                    << population.count_vaccinated() << std::endl;
                    }
            }
            timesteps.push_back(timestep);
            num_vaccinated.push_back(world.count_vaccinated());
            num_recovered.push_back(world.count_recovered());
            num_susceptible.push_back(world.count_susceptible());
        }
        details_file.close();

        double avg_vaccinated = calculate_mean(num_vaccinated);
        double stddev_vaccinated = calculate_stddev(num_vaccinated, avg_vaccinated);
        double avg_recovered = calculate_mean(num_recovered);
        double stddev_recovered = calculate_stddev(num_recovered, avg_recovered);
        double avg_susceptible = calculate_mean(num_susceptible);
        double stddev_susceptible = calculate_stddev(num_susceptible, avg_susceptible);
        std::ofstream stats_file("disease_stats.csv");
        // stats_file << "Average Vaccinated, Standard Deviation Vaccinated, Average Recovered, Standard Deviation Recovered, Average Susceptible, Standard Deviation Susceptible\n";
        stats_file <<"key"<<","<<"value"<<"\n"
                <<"timesteps"<<","<<timesteps.back()<<"\n"
                << "suceptiible_persons"<< ", " <<num_susceptible.back()<<"\n"
                << "recovered_persons" <<", "<<num_recovered.back() << "\n "
                << "vaccinated_persons" <<", "<< num_vaccinated.back()<<"\n"<< std::endl;
        stats_file.close();
        }
        vaccination_vs_recovered_file << vaccination_rate << ", " << world.count_recovered() << "\n";
    }
        std::cout << "Simulation finished after " << timesteps.back() << " days\n";
        std::cout << "Number of vaccinated persons: " << num_vaccinated.back() << "\n";
        std::cout << "Number of recovered persons: " << num_recovered.back() << "\n";
        std::cout << "Number of susceptible persons: " << num_susceptible.back() << "\n";    
    vaccination_vs_recovered_file.close();
}
