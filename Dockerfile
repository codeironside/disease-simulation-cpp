
FROM ubuntu:20.04

#ENV DEBIAN_FRONTEND=noninteractive
FROM debian:bookworm-slim as build
#RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    openmpi-common \
    libopenmpi-dev \
    tzdata \
    && ln -snf /usr/share/zoneinfo/Africa/Lagos /etc/localtime \
    && echo "Africa/Lagos" > /etc/timezone \
    && dpkg-reconfigure -f noninteractive tzdata \
    && rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install -y build-essential openmpi-bin openmpi-common libopenmpi-dev && apt-get clean


ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

WORKDIR /app

COPY . .
COPY simulation/disease_in.ini /app
RUN mpic++ -o Main simulation/main.cpp simulation/simulation.cpp

ENTRYPOINT ["mpirun", "-np", "1", "./Main"]

