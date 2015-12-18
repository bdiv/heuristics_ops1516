#ifndef VARIATION_H
#define VARIATION_H
#include "population.h"
#include "crossbreedFunctor.h"

namespace nQueens {

class variation
{
    public:
        static void mutation(individual & i, double pMutation);

        class matched_crossover : public crossbreedFunctor
        {
            public:
                individual operator() (population & pop);
        };
       // static individual matched_crossover(population & pop); // Landscape analysis and efficient metaheuristics
                                                                // for solving the n-queens problem
        class ordered_crossover : public crossbreedFunctor
        {
            public:
                individual operator() (population & pop);
        };
        //static individual ordered_crossover(population & pop); // Landscape analysis and efficient metaheuristics
                                                         // for solving the n-queens problem
        virtual ~variation();
    protected:
    private:
        static std::pair<unsigned int, unsigned int> chooseRandomArea(unsigned int n); // returns a pair(start, end) that describes an area in a vector
        static std::pair<individual, individual > generateChildren(individual & parent1, individual & parent2);
        variation();
};
}
#endif // VARIATION_H
