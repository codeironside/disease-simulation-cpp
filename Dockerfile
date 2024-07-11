# Build Stage
FROM debian:bookworm-slim as build

WORKDIR /src

# Copy necessary files
COPY simulation /src/simulation
COPY include /src/include
COPY . .
COPY simulation/disease_in.ini /src/simulation

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean

# Build the application
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

# Copy the default ini file
COPY include /app/include
COPY . .
COPY simulation/disease_in.ini /app/simulation

# Change to the scratch directory for runtime operations
WORKDIR /scratch


# Set the entry point to execute the simulation with MPI
ENTRYPOINT ["mpirun", "-np", "1", "/app/hpc_disease_simulation"]
