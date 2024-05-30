FROM gcc:13.3

WORKDIR /usr/src/simulation/tasktwo

COPY . .
COPY disease_in.ini /usr/src/simulation/tasktwo/disease_in.ini

RUN g++ -o Main simulation.cpp main.cpp


CMD ["./Main"]
