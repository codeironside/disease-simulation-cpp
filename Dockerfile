# Build Stage
FROM debian:bookworm-slim as build

WORKDIR /src

# Copy necessary files
COPY simulation /src/simulation
COPY include /src/include

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean

# Build the application
COPY include /src/include
COPY simulation/disease_in.ini /src/simulation
RUN mpic++ -o hpc_disease_simulation /src/simulation/main.cpp /src/simulation/simulation.cpp

# Run Stage
FROM debian:bookworm-slim as run

# Install runtime dependencies
RUN apt-get update && apt-get install -y openmpi-bin openmpi-common libopenmpi-dev && apt-get clean

# Set environment variables for MPI
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# Copy the built application from the build stage
WORKDIR /app
COPY --from=build /src/hpc_disease_simulation /app

# Copy the default ini file for internal tests
COPY include /app/include
COPY simulation/disease_in.ini /app/simulation

# Set the entry point to execute the simulation with MPI
ENTRYPOINT ["mpirun", "-np", "1", "/app/hpc_disease_simulation"]
