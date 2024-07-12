FROM gcc:13.3

WORKDIR /app

# Copy all files and directories to the working directory
COPY . .
COPY simulation/disease_in.ini /app
COPY simulation/disease_in.ini /app/simulation/disease_in.ini

# Compile the program, specifying the include directory
RUN g++ -o Main simulation/simulation.cpp simulation/main.cpp

WORKDIR /scratch
COPY . .
COPY . /scratch
COPY include /scratch/include
COPY simulation/disease_in.ini /scratch
RUN ls -l /scratch/include

# Set the command to run the executablee


ENTRYPOINT ["/app/Main"]
