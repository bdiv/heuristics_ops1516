#ifndef ITERATIVEMASTER_H
#define ITERATIVEMASTER_H
#include "variation.h"
#include "crossbreedFunctor.h"
#include <ctime>
namespace nQueens {

class iterativeMaster
{
    public:
        // constructor
        iterativeMaster( unsigned int n, unsigned int x, double pMutation );
        //destructor
        virtual ~iterativeMaster();
        // getter for the population
        population& getPopulation();
        // getter for current number of variations during runtime
        unsigned int getVariationsCounter();
        // getter for solutions
        std::vector<individual> & getSolutions();
        // solver parameterized by variations
        unsigned int solve(unsigned int maxVariations, crossbreedFunctor & breed);
        // solver that runs until it reached a specified point in time (timeout)
        unsigned int solve(std::time_t timeout, crossbreedFunctor & breed);
    protected:
    private:
        // containes the individuals
        population pop;
        // contains the number of variations during runtime/solving
        unsigned int variationCounter;
        // contains the solutions (score == 0)
        std::vector <individual> solutions;
};

}
#endif // ITERATIVEMASTER_H
