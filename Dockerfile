# Build Stage
FROM debian:bookworm-slim as build

WORKDIR /src

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean

# Set environment variables for MPI
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# Copy necessary files
COPY . /src
COPY simulation /src/simulation
COPY include /src/include
COPY simulation/disease_in.ini /src/simulation
COPY simulation/disease_in.ini /src

RUN ls -l /src/simulation
RUN chmod 644 /src/simulation/disease_in.ini




# Build the application
RUN mpic++ -o Main /src/simulation/main.cpp /src/simulation/simulation.cpp

# Run Stage
FROM debian:bookworm-slim as run

RUN apt-get update && apt-get install -y openmpi-bin openmpi-common && apt-get clean
# Set environment variables for MPI
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
# Copy the built application from the build stage
WORKDIR /app
COPY --from=build /src/Main /app
COPY --from=build /src/ /app
COPY --from=build /src/simulation/disease_in.ini /app

RUN ls -l /app/simulation
RUN ls -l /app
RUN ls -l /app/simulation

WORKDIR /scratch
COPY --from=build /src/Main /scratch
COPY --from=build /src/include /scratch/include


ENTRYPOINT ["mpirun", "-np", "1", "/app/Main"]


