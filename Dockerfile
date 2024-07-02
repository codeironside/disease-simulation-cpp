#FROM gcc:13.3

#WORKDIR /usr/src/simulation

# Copy all files and directories to the working directory
#COPY . .
#COPY simulation/disease_in.ini /usr/src/simulation/disease_in.ini

# Compile the program, specifying the include directory
#RUN g++ -o Main simulation/simulation.cpp simulation/main.cpp

# Set the command to run the executable
#CMD ["./Main"]

#new
# Use the official gcc image as a base image
FROM gcc:13.3

# Install MPI
RUN apt-get update && apt-get install -y mpich

# Set the working directory
WORKDIR /usr/src/simulation

# Copy all files and directories to the working directory
COPY . .
COPY simulation/disease_in.ini /usr/src/simulation/disease_in.ini

# Compile the program with MPI support
RUN mpic++ -o Main simulation/simulation.cpp simulation/main.cpp

# Set the command to run the executable with MPI
CMD ["mpirun", "-np", "4", "./Main"]

