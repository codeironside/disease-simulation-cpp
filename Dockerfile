#FROM gcc:13.3

#WORKDIR /usr/src/simulation

# Copy all files and directories to the working directory
#COPY . .
#COPY simulation/disease_in.ini /usr/src/simulation/disease_in.ini

# Compile the program, specifying the include directory
#RUN g++ -o Main simulation/simulation.cpp simulation/main.cpp

# Set the command to run the executable
#CMD ["./Main"]


# Use an official MPI base image
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    openmpi-common \
    libopenmpi-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY . .

# Build the application
RUN mpic++ -o simulation main.cpp simulation.cpp

# Set the entrypoint to mpirun with default arguments
ENTRYPOINT ["mpirun", "-np", "4", "./simulation"]
