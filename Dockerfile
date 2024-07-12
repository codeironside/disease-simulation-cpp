FROM gcc:13.3

WORKDIR /usr/src/simulation

# Copy all files and directories to the working directory
COPY . .
COPY simulation/disease_in.ini /usr/src/simulation/disease_in.ini

# Compile the program, specifying the include directory
RUN g++ -o Main simulation/simulation.cpp simulation/main.cpp

WORKDIR /scratch
COPY . /scratch
COPY include /scratch/include
RUN ls -l /scratch/include

# Set the command to run the executable
CMD ["./Main"]
