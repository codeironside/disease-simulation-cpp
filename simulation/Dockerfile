FROM debian:bookworm-slim as build
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean
#RUN apt-get update && \
#    apt-get install -y g++


ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

WORKDIR /app

COPY . .
COPY . /app
COPY disease_in.ini /app
RUN mpic++ -g -o Main main.cpp simulation.cpp
#RUN mpic++ -Wall -Wextra -o Main simulation/main.cpp simulation/simulation.cpp


WORKDIR /scratch
COPY . /scratch
COPY scratch/disease_in.ini /scratch
RUN cat scratch/disease_in.ini


#ENTRYPOINT ["/app/run_gdb.sh"]
ENTRYPOINT ["mpirun", "-np", "1", "/app/Main"]

# ["/app/Main"]
