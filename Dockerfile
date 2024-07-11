
FROM debian:bookworm-slim as build
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean


ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

WORKDIR /scratch

COPY . .
COPY simulation/disease_in.ini /scr]
RUN mpic++ -o Main simulation/main.cpp simulation/simulation.cpp

ENTRYPOINT ["mpirun", "-np", "1", "./Main"]

