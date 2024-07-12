Certainly! Here's the updated README content in a format that you can directly copy and place in a README file:

```markdown
# HPC Disease Simulation

This project is designed for the HPC-05 Programming Lab, focusing on simulating the spread of diseases.

## Prerequisites

Make sure you have a working Docker installation. You can download and install Docker from [here](https://www.docker.com/products/docker-desktop).

## Cloning the Repository

1. **Clone this repository**:
   ```bash
   git clone <https://mygit.th-deg.de/sh04625/hpc-disease-simulation.git>
   ```

2. **Navigate into the cloned repository**:
   ```bash
   cd hpc-disease-simulation
   ```

## Building and Running the Simulation

### Task 1

1. **Build the Docker image for Task 1**:
   ```bash
   docker build . -t simulation:1
   ```

2. **Run the simulation for Task 1**:
   ```bash
   docker run --rm -it simulation:1
   ```

3. **Build the test for Task 1**:
   ```bash
   g++ -o test_simulation test.cpp simulation.cpp
   ```

4. **Run the test for Task 1**:
   ```bash
   ./test_simulation
   ```

### Task 2 (Herd investigation)

1. **Navigate to the Task 2 folder**:
   ```bash
   cd tasktwo
   ```

2. **Build the Docker image for Task 2**:
   ```bash
   docker build . -t simulation:2
   ```

3. **Run the simulation for Task 2**:
   ```bash
   docker run --rm -it simulation:2
   ```

## Note on Docker Mounting Challenges

During the course of this project, I encountered significant challenges with mounting the `disease_in.ini` file correctly in the Docker container. It required considerable troubleshooting and adjustments to ensure the file was accessible and correctly used by the simulation. If you encounter similar issues, ensure that the path and file permissions are correctly set and that you are mounting the file in the expected location within the container.

# THANK YOU
