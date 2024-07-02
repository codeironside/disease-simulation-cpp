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

# Set environment variables to make the installation of tzdata non-interactive
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies and set the timezone
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    openmpi-common \
    libopenmpi-dev \
    tzdata \
    && ln -snf /usr/share/zoneinfo/Africa/Lagos /etc/localtime \
    && echo "Africa/Lagos" > /etc/timezone \
    && dpkg-reconfigure -f noninteractive tzdata \
    && rm -rf /var/lib/apt/lists/*


# Set environment variables to allow running as root
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY . .

# Build the application
RUN mpic++ -o Main simulation/main.cpp simulation/simulation.cpp

# Set the entrypoint to mpirun with default arguments
ENTRYPOINT ["mpirun", "-np", "4", "./Main"]
