
FROM ubuntu:20.04


ENV DEBIAN_FRONTEND=noninteractive


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



ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1


WORKDIR /app


COPY . .
COPY disease_in.ini /app

RUN mpic++ -o Main main.cpp simulation.cpp

ENTRYPOINT ["mpirun", "-np", "1", "./Main"]

