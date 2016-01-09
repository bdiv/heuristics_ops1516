#ifndef ITERATIVEMASTER_H
#define ITERATIVEMASTER_H
#include "variation.h"
#include "crossbreedFunctor.h"
#include <ctime>
#include <algorithm>
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
        std::vector<std::vector <unsigned int> > & getSolutions();
        // solver parameterized by variations
        unsigned int solve(unsigned int maxVariations, crossbreedFunctor & breed);
        // solver that runs until it reached a specified point in time (timeout)
        unsigned int solve(std::time_t timeout, crossbreedFunctor & breed);
        // solver that runs until we found another solution besides the ones we already have
        unsigned int solve(crossbreedFunctor & breed);
        // public helper function to generate rotated and mirrored versions of a solution. They are unique and have a certain fixed order.
        // this method will not keep the order you specified!!
        static void generateIsoForms(std::vector<std::vector<unsigned int>> & v);
    protected:
    private:
        void checkPopulation(population & pop);
        // containes the individuals
        population pop;
        // contains the number of variations during runtime/solving
        unsigned int variationCounter;
        // contains the solutions (score == 0)
        std::vector <std::vector<unsigned int>> solutions;
        // invert a vector
        static void invert(std::vector <unsigned int> & v);
        /* swap and reaorder coordinates of our vector
           (3,1,4,2,0) will become (4,1,3,0,2)

            0 3      3 0      0 4
            1 1      1 1      1 1
            2 4  ->  4 2  ->  2 3
            3 2      2 3      3 0
            4 0      0 4      4 2

            so in essence: values will become keys and vice verca, additionally the new keys are sorted again
        */
        static void swapReorder(std::vector <unsigned int> & v);
        // makes sure that there are no doubles in the solutions vector
        // this will reorder the elements to ensure it. beware of that!
        static void uniquifySolutions(std::vector< std::vector<unsigned int >> & v);
        static void uniquifyPopulation(population & pop);
        void printStatus();
};

}
#endif // ITERATIVEMASTER_H
