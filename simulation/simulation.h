#ifndef HPC_DISEASE_SIMULATION_H_
#define HPC_DISEASE_SIMULATION_H_

#endif // HPC_DISEASE_SIMULATION_H_

#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <numeric>
#include <valarray>
#include "../include/INIReader.h"
//class Person {};
enum class State {
  Susceptible,
  Infectious,
  Vaccinated,
  Recovered,
};
class Disease {
public:
  int duration;
  double transmission_probability;
  double mutation_probability;
  std::string name;
  cout << name <<endl;
  void mutate() {
    double duration_change = (rand() % 21 - 10) / 100.0;  
    double transmission_change = (rand() % 21 - 10) / 100.0;
    
    duration = static_cast<int>(duration * (1 + duration_change));
    transmission_probability *= (1 + transmission_change);

    if (transmission_probability < 0) {
        transmission_probability = 0;
    } else if (transmission_probability > 1) {
        transmission_probability = 1;
    }
}

  Disease(std::string name="", int duration = 0, double transmission_probability = 0.0, double mutation_probability = 0.0)
        : name(name), duration(duration), transmission_probability(transmission_probability), mutation_probability(mutation_probability) {}
};

class Person {
public:
  State state;
  int days_sick;
  int simulation_runs;
  int size;
  //days_sick(0)
  Person() : state(State::Susceptible),simulation_runs()  {}

  void infect(int n) {
    if (state == State::Susceptible) {
      state = State::Infectious;
      days_sick = n;
    }
  }

  void vaccinate() {
    if (state == State::Susceptible) {
      state = State::Vaccinated;
    }
  }

  void one_more_day() {
    if (state == State::Infectious && --days_sick == 0) {
      state = State::Recovered;
    }
  }

  bool is_recovered() {
    return state == State::Recovered;
  }

 
  void touch(Person& otherPerson, Disease& disease) {
      if (state == State::Infectious && otherPerson.state == State::Susceptible) {
          double infection_chance = rand() / static_cast<double>(RAND_MAX);
          if (infection_chance < disease.transmission_probability) {
              otherPerson.state = State::Infectious;
              otherPerson.days_sick = disease.duration;
              double mutation_chance = rand() / static_cast<double>(RAND_MAX);
              if (mutation_chance < disease.mutation_probability) {
                  disease.mutate();
              }
          }
      }
  }

};

//class Disease {};






class Population {
public:
  std::vector<Person> people;
  double vaccination_rate;
  std::string name;

  int count_infected() const {
    return std::count_if(people.begin(), people.end(),
                         [](const Person& p) { return p.state == State::Infectious; });
  }

  int count_vaccinated() const {
    return std::count_if(people.begin(), people.end(),
                         [](const Person& p) { return p.state == State::Vaccinated; });
  }

  int count_recovered() const {
    return std::count_if(people.begin(), people.end(),
                         [](const Person& p) { return p.state == State::Recovered; });
  }

  int count_susceptible() const {
    return std::count_if(people.begin(), people.end(),
                         [](const Person& p) { return p.state == State::Susceptible; });
  }

  void random_infection(const Disease& disease, int n) {
    for (int i = 0; i < n; ++i) {
      people[rand() % people.size()].infect(disease.duration);
    }
  }

  void random_vaccination() {
    int num_to_vaccinate = static_cast<int>(people.size() * vaccination_rate);
    for (int i = 0; i < num_to_vaccinate; ++i) {
      people[rand() % people.size()].vaccinate();
    }
  }


  void one_more_day() {
    for (Person& p : people) {
      p.one_more_day();
    }
  }
};

class World {
public:
  std::vector<Population> population;
  int count_infected() const {
    int total_infected = 0;
    for (const Population& pop : population) {
      total_infected += pop.count_infected();
    }
    return total_infected;
  }

  int count_recovered() const {
    int total_recovered = 0;
    for (const Population& pop : population) {
      total_recovered += pop.count_recovered();
    }
    return total_recovered;
  }

  int count_susceptible() const {
    int total_susceptible = 0;
    for (const Population& pop : population) {
      total_susceptible += pop.count_susceptible();
    }
    return total_susceptible;
  }

  int count_vaccinated() const {
    int total_vaccinated = 0;
    for (const Population& pop : population) {
      total_vaccinated += pop.count_vaccinated();
    }
    return total_vaccinated;
  }
  void add_population(const Population& pop) {
  population.push_back(pop);
  }   


  Population& select_random_population() {
    int random_index = rand() % population.size();
    return population[random_index];
  }
};

class Simulation {
public:
    Simulation(std::string in_file = "disease_in.ini");
    void start();
    void reset_world();
    double calculate_mean(const std::vector<int>& data) {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        return sum / data.size();
    }

    double calculate_stddev(const std::vector<int>& data, double mean) {
        double sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);
        double variance = sq_sum / data.size() - mean * mean;
        return std::sqrt(variance);
    }

private:
    std::string input_file;
    Population population;
    World world;
    Disease disease;
    int simulation_run;
    std::string simulation_name;
    
};

