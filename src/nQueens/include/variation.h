#ifndef VARIATION_H
#define VARIATION_H
#include "population.h"

class variation
{
    public:
        static void mutation(individual & i, double pMutation);
        static void matched_crossover(population & pop); // Landscape analysis and efficient metaheuristics
                                                         // for solving the n-queens problem
        static void ordered_crossover(population & pop); // Landscape analysis and efficient metaheuristics
                                                         // for solving the n-queens problem
        virtual ~variation();
    protected:
    private:
        static std::pair<unsigned int, unsigned int> chooseRandomArea(unsigned int n); // returns a pair(start, end) that describes an area in a vector
        static std::pair<std::vector<unsigned int>, std::vector<unsigned int> > generateChildren(individual* parent1, individual* parent2);
        static std::vector<unsigned int> generateChild(); // simply copies the vector of a parent so that we can work on it
        variation();
};

#endif // VARIATION_H
