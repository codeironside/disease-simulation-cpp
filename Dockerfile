#FROM gcc:13.3

#WORKDIR /usr/src/simulation

# Copy all files and directories to the working directory
#COPY . .
#COPY simulation/disease_in.ini /usr/src/simulation/disease_in.ini

# Compile the program, specifying the include directory
#RUN g++ -o Main simulation/simulation.cpp simulation/main.cpp

# Set the command to run the executable
#CMD ["./Main"]

# Use the official gcc image as a base image
FROM gcc:13.3

# Install dependencies and MPI
RUN apt-get update && apt-get install -y \
    build-essential \
    mpich \
    gfortran \
    libmpich-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/simulation

# Copy all files and directories to the working directory
COPY . .
COPY simulation/disease_in.ini /usr/src/simulation/disease_in.ini

# Compile the program with MPI support
RUN mpic++ -o Main simulation/simulation.cpp simulation/main.cpp

# Set the command to run the executable with MPI
CMD ["mpirun", "-np", "4", "./Main"]

