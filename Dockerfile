# Build Stage
FROM debian:bookworm-slim as build

WORKDIR /src

# Copy necessary files
COPY CMakeLists.txt /src
COPY simulation /src/simulation
COPY include /src/include 

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential cmake openmpi-bin openmpi-common libopenmpi-dev && apt-get clean

# Build the application
WORKDIR /src/build
RUN cmake .. && make

# Run Stage
FROM debian:bookworm-slim as run

# Set environment variables for MPI
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# Copy the built application from the build stage
WORKDIR /app
COPY --from=build /src/build/hpc_disease_simulation /app

# Change to the scratch directory for runtime operations
WORKDIR /scratch

# Set the entry point to execute the simulation with MPI
ENTRYPOINT ["mpirun", "-np", "1", "/app/hpc_disease_simulation"]
