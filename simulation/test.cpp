#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"
#include "../include/INIReader.h"

INIReader reader("disease_in.ini");

TEST_CASE("Testing Disease class")
{
    REQUIRE(reader.ParseError() == 0);

    std::string disease_name = reader.Get("disease", "name", "UNKNOWN");
    int duration = reader.GetInteger("disease", "duration", -1);
    double transmissibility = reader.GetReal("disease", "transmissability", -1.0);

    Disease disease(disease_name, duration, transmissibility);

    INFO("Disease name should be " << disease_name);
    CHECK(disease.name == disease_name);

    INFO("Disease duration should be " << duration);
    CHECK(disease.duration == duration);

    INFO("Disease transmission probability should be " << transmissibility);
    CHECK(disease.transmission_probability == transmissibility);
}

TEST_CASE("Testing Person class")
{
    REQUIRE(reader.ParseError() == 0);

    int duration = reader.GetInteger("disease", "duration", -1);
    double transmissibility = reader.GetReal("disease", "transmissability", -1.0);
    Disease disease("TestDisease", duration, transmissibility);
    Person person;

    SUBCASE("Initial state should be Susceptible")
    {
        INFO("Initial state should be Susceptible");
        CHECK(person.state == State::Susceptible);
    }

    SUBCASE("Infecting a person")
    {
        person.infect(duration);
        INFO("State should be Infectious after infection");
        CHECK(person.state == State::Infectious);

        INFO("Days sick should be " << duration << " after infection");
        CHECK(person.days_sick == duration);
    }

    SUBCASE("Vaccinating a person")
    {
        person.vaccinate();
        INFO("State should be Vaccinated after vaccination");
        CHECK(person.state == State::Vaccinated);
    }

    SUBCASE("Recovering from infection")
    {
        person.infect(1);
        person.one_more_day();
        INFO("State should be Recovered after recovering from infection");
        CHECK(person.state == State::Recovered);
    }

    SUBCASE("Infecting another person")
    {
        Person otherPerson;
        person.infect(duration);
        person.touch(otherPerson, disease);
        INFO("Other person's state should be Infectious after being touched by an infected person");
        CHECK(otherPerson.state == State::Infectious);
    }
}

TEST_CASE("Testing Population class")
{
    REQUIRE(reader.ParseError() == 0);

    int population_size = reader.GetInteger("population_1", "size", -1);
    double vaccination_rate = reader.GetReal("population_1", "vaccination_rate", -1.0);
    bool patient_0 = reader.GetBoolean("population_1", "patient_0", false);

    Population population;
    population.people.resize(population_size);

    SUBCASE("Initial counts should be zero")
    {
        INFO("Initial infected count should be 0");
        CHECK(population.count_infected() == 0);

        INFO("Initial vaccinated count should be 0");
        CHECK(population.count_vaccinated() == 0);

        INFO("Initial recovered count should be 0");
        CHECK(population.count_recovered() == 0);

        INFO("Initial susceptible count should be " << population_size);
        CHECK(population.count_susceptible() == population_size);
    }

    SUBCASE("Random infection")
    {
        Disease disease("TestDisease", 5, 0.3);
        population.random_infection(disease, 10);
        INFO("Infected count should be 10 after random infection");
        CHECK(population.count_infected() == 10);
    }

    SUBCASE("Random vaccination")
    {
        population.vaccination_rate = vaccination_rate;
        population.random_vaccination();
        INFO("Vaccinated count should be " << int(population_size * vaccination_rate) << " after random vaccination");
        CHECK(population.count_vaccinated() == int(population_size * vaccination_rate));
    }

    SUBCASE("One more day")
    {
        Disease disease("TestDisease", 5, 0.3);
        population.random_infection(disease, 10);
        population.one_more_day();
        INFO("Recovered count should be 0 after one more day");
        CHECK(population.count_recovered() == 0);
        population.one_more_day();
        INFO("Recovered count should be greater than 0 after two more days");
        CHECK(population.count_recovered() > 0);
    }
}

TEST_CASE("Testing World class")
{
    World world;
    Population population;
    population.people.resize(100);
    world.add_population(population);

    SUBCASE("Counting states")
    {
        INFO("Initial infected count in world should be 0");
        CHECK(world.count_infected() == 0);

        INFO("Initial vaccinated count in world should be 0");
        CHECK(world.count_vaccinated() == 0);

        INFO("Initial recovered count in world should be 0");
        CHECK(world.count_recovered() == 0);

        INFO("Initial susceptible count in world should be 100");
        CHECK(world.count_susceptible() == 100);
    }

    SUBCASE("Select random population")
    {
        Population &random_population = world.select_random_population();
        INFO("Randomly selected population should be the first population");
        CHECK(&random_population == &world.population[0]);
    }
}

TEST_CASE("Testing Simulation class")
{
    Simulation simulation("disease_in.ini");

    SUBCASE("Starting simulation")
    {
        simulation.start();
        INFO("Simulation should start without errors");
        CHECK(true);
    }
}
