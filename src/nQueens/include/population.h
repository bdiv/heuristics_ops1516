#ifndef POPULATION_H
#define POPULATION_H
#include "individual.h"

class population
{
    public:
        population();               // constructor
        init();                     // initialize population with random individuals
        virtual ~population();      // destructor
        individual * getRandomIndividual(); // return a random individual which is not in use (indicated by flag)
    protected:
    private:
        vector<individual*> individuals; // dadastructure for the individuals
};

#endif // POPULATION_H
