
FROM debian:bookworm-slim as build
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean


ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

WORKDIR /app

COPY . .
COPY . /app
COPY simulation/disease_in.ini /app
RUN mpic++ -o Main simulation/main.cpp simulation/simulation.cpp

WORKDIR /scratch
COPY . .
COPY . /scratch
COPY simulation/disease_in.ini /scratch


ENTRYPOINT ["mpirun", "-np", "1", "/app/Main"]

