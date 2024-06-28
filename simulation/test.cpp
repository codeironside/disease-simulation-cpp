#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"

TEST_CASE("Testing Disease class")
{
    Disease disease("TestDisease", 5, 0.3);

    INFO("Disease name should be TestDisease");
    CHECK(disease.name == "TestDisease");

    INFO("Disease duration should be 5");
    CHECK(disease.duration == 5);

    INFO("Disease transmission probability should be 0.3");
    CHECK(disease.transmission_probability == 0.3);
}

TEST_CASE("Testing Person class")
{
    Disease disease("TestDisease", 5, 0.3);
    Person person;

    SUBCASE("Initial state should be Susceptible")
    {
        INFO("Initial state should be Susceptible");
        CHECK(person.state == State::Susceptible);
    }

    SUBCASE("Infecting a person")
    {
        person.infect(5);
        INFO("State should be Infectious after infection");
        CHECK(person.state == State::Infectious);

        INFO("Days sick should be 5 after infection");
        CHECK(person.days_sick == 5);
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
        person.infect(5);
        person.touch(otherPerson, disease);
        INFO("Other person's state should be Infectious after being touched by an infected person");
        CHECK(otherPerson.state == State::Infectious);
    }
}

TEST_CASE("Testing Population class")
{
    Population population;
    population.people.resize(100);

    SUBCASE("Initial counts should be zero")
    {
        INFO("Initial infected count should be 0");
        CHECK(population.count_infected() == 0);

        INFO("Initial vaccinated count should be 0");
        CHECK(population.count_vaccinated() == 0);

        INFO("Initial recovered count should be 0");
        CHECK(population.count_recovered() == 0);

        INFO("Initial susceptible count should be 100");
        CHECK(population.count_susceptible() == 100);
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
        population.vaccination_rate = 0.2;
        population.random_vaccination();
        INFO("Vaccinated count should be 20 after random vaccination");
        CHECK(population.count_vaccinated() == 20);
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

    // SUBCASE("Initial simulation setup") {
    //     INFO("Simulation name should be Test Simulation");
    //     CHECK(simulation.simulation_name == "Test Simulation");
    // }

    SUBCASE("Starting simulation")
    {
        simulation.start();
        INFO("Simulation should start without errors");
        CHECK(true);
    }
}
