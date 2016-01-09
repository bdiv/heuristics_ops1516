#ifndef POPULATION_H
#define POPULATION_H
//#include "individual.h"
#include <random>

namespace nQueens {

class population
{
    public:
        static std::random_device rd;
        //konstruktor
        population(unsigned int n, unsigned int x, double pMutation);

        //destruktor
        virtual ~population();

        //methoden
        std::vector<unsigned int> & getRandomIndividual();                      // return a random individual which is not in use (indicated by flag)
        std::pair<std::vector<unsigned int> &,std::vector<unsigned int>&> chooseTwoRandom();   // returns a pair of random parent individuals, first will have the lower score
        void scoreAll();    //Alle Individuen bekommen einen score;
        void printAll();    //gibt alle individuen aus (Test methode)
        void generateIndividual(std::vector<unsigned int> & i);
        void generateRandomizedIndividual(std::vector<unsigned int> & i);
        unsigned int scoreIndividual(std::vector<unsigned int> & z);
        std::vector<std::vector<unsigned int>> & getIndividuals();
        std::uniform_int_distribution<> randomIndividual;
        std::uniform_int_distribution<> randomPosition;
        std::uniform_real_distribution<> randomMutation;
        std::mt19937 mt;
        double getPMutation();

    private:

        //Members
        unsigned int n;                         // number of genes an individual has
        double pMutation;                       // probability for mutation after cross




        void printIndu(std::vector<unsigned int> & i);
        std::vector<std::vector<unsigned int>> individuals;    // datastructure for the individuals
};
}
#endif // POPULATION_H
