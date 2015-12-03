#ifndef POPULATION_H
#define POPULATION_H
#include "individual.h"

class population
{
    public:
        population(unsigned int n, unsigned int x, double pMutation);               // constructor
        void init();                     // initialize population with random individuals
        virtual ~population();      // destructor
        individual * getRandomIndividual(); // return a random individual which is not in use (indicated by flag)
        std::pair<individual*,individual*> chooseTwoRandom(); // returns a pair of random parent individuals, first will have the lower score
    protected:
    private:
        double pMutation;      // probability for mutation after cross
        unsigned int n;                 // number of genes an individual has
        std::vector<individual*> individuals; // datastructure for the individuals
};

#endif // POPULATION_H
