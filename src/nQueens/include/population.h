#ifndef POPULATION_H
#define POPULATION_H
#include "individual.h"

namespace nQueens {

class population
{
    public:

        //konstruktor
        population(unsigned int n, unsigned int x, double pMutation);

        //destruktor
        virtual ~population();

        //methoden
        void init();        // initialize population with random individuals
        individual& getRandomIndividual();                      // return a random individual which is not in use (indicated by flag)
        std::pair<individual&,individual&> chooseTwoRandom();   // returns a pair of random parent individuals, first will have the lower score
        void scoreAll();    //Alle Individuen bekommen einen score;
        void printAll();    //gibt alle individuen aus (Test methode)

    private:

        //Members
        unsigned int n;                         // number of genes an individual has
        double pMutation;                       // probability for mutation after cross

        std::vector<individual> individuals;    // datastructure for the individuals
};
}
#endif // POPULATION_H
