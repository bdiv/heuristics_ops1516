#ifndef VARIATION_H
#define VARIATION_H
#include "population.h"
#include "crossbreedFunctor.h"

namespace nQueens {

/*
    This class contains two other child classes of the crossbreedFunctor. They implement different ways to
    change the population, eg. generate children and kill the worst parent (score wise).
    The two functors are just clustered in variation for encapsulation reasons. Feel free to
    write other functor-modules, as long as they inherit from crossbreedFunctor and implement its ()Operator
    everything should be fine and dandy.

    This class also contains the static mutation function because they belong together concept wise
*/

class variation
{
    public:
        // static mutation function, bc mutation is easy and always the same
        static void mutation(individual & i, double pMutation);


        // Source: "Landscape analysis and efficient metaheuristics for solving the n-queens problem"
        // for the theory behind the implementation of:
        class matched_crossover : public crossbreedFunctor
        {
            public:
                individual operator() (population & pop) override;
        };
        // Source: "Landscape analysis and efficient metaheuristics for solving the n-queens problem"
        // for the theory behind the implementation of:
        class ordered_crossover : public crossbreedFunctor
        {
            public:
                individual operator() (population & pop) override;
        };

        virtual ~variation();
    protected:
    private:
        // private helper function for crossbreeding
        // chooses an area to be switched between the parents
        static std::pair<unsigned int, unsigned int> chooseRandomArea(unsigned int n); // returns a pair(start, end) that describes an area in a vector
        // private helper function for crossbreeding
        // basically copies the vectors of the parents so we can work on them savely
        static std::pair<individual, individual > generateChildren(individual & parent1, individual & parent2);
        variation();
};
}
#endif // VARIATION_H
