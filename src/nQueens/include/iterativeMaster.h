#ifndef ITERATIVEMASTER_H
#define ITERATIVEMASTER_H
#include "variation.h"
#include "crossbreedFunctor.h"
#include <ctime>
namespace nQueens {

class iterativeMaster
{
    public:
        iterativeMaster( unsigned int n, unsigned int x, double pMutation );
        virtual ~iterativeMaster();
        population& getPopulation();
        unsigned int getVariationsCounter();
        std::vector<individual> & getSolutions();
        unsigned int solve(unsigned int maxVariations, crossbreedFunctor & breed);
        unsigned int solve(std::time_t timeout, crossbreedFunctor & breed);
    protected:
    private:
        population pop;
        unsigned int variationCounter;
        std::vector <individual> solutions;
};

}
#endif // ITERATIVEMASTER_H
