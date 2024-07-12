
FROM debian:bookworm-slim as build
#RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && #apt-get clean
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    openmpi-common \
    libopenmpi-dev \
    gdb && \
    apt-get clean


ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

WORKDIR /app

COPY . .
COPY . /app
COPY simulation/disease_in.ini /app
RUN mpic++ -g -o Main simulation/main.cpp simulation/simulation.cpp
RUN mpic++ -Wall -Wextra -o Main simulation/main.cpp simulation/simulation.cpp
COPY run_gdb.sh /app/run_gdb.sh
RUN chmod +x /app/run_gdb.sh

WORKDIR /scratch
COPY . /scratch
COPY include /scratch/include
COPY . /scratch


ENTRYPOINT ["/app/run_gdb.sh"]
#ENTRYPOINT ["mpirun", "-np", "1", "gdb", "--args", "/app/Main"]

#ENTRYPOINT ["mpirun", "-np", "1", "/app/Main"]

