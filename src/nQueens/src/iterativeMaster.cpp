#include "../include/iterativeMaster.h"


namespace nQueens {

/*
    This class is a universal iterative solver for the nQueens problem.
    It wraps around the population and uses defined functors (crossbreedFunctor) to change this
    population in a way specified by genomic algorithms
*/

// simple constructor
iterativeMaster::iterativeMaster(unsigned int n, unsigned int x, double pMutation): pop(population(n,x,pMutation))
{

}

iterativeMaster::~iterativeMaster()
{
    //dtor
}

// return the population as a reference
population& iterativeMaster::getPopulation()
{
    return this->pop;
}

// return the number of variations that occured during runtime
unsigned int iterativeMaster::getVariationsCounter()
{
    return this->variationCounter;
}

// return vector containing the solutions
std::vector <individual> & iterativeMaster::getSolutions()
{
    return this->solutions;
}

// solve the optimization problem using a special functor
// the functor needs to implement the crossbreedFunctor class
// and overload the ()operator!
// this runs until we've reached a specified number of variations
// returns the numbe rof solutions found
// those can be recovered by calling getSolutions()
unsigned int iterativeMaster::solve(unsigned int maxVariations, crossbreedFunctor& breed)
{
    // loop until we reach maximum variation count
    for(unsigned int i = 0; i < maxVariations; i++)
    {
        // use the crossbreedFunctor to create a new individual
        // he'll automatically be in the population and replace the parent with
        // the lowest score
        individual dude = breed(this->pop);
        // if score is 0 we have found a solution, scores > 0 are suboptimal
        if(dude.getScore() == 0)
        {
            // grab and sack the dude
            this->solutions.push_back(dude);
        }
    }
    // return number of solutions
    return this->solutions.size();
}

// solves our optimizationproblem using a special crossbreedFunctor
// this will run until we reach the specified timeout
// (note that timeout is not a range but rather a future point in time!!)
// and returns the number of solutions found
// those can later be recovered by calling getSolutions()
unsigned int iterativeMaster::solve(std::time_t timeout, crossbreedFunctor & breed)
{
    // save current time
    std::time_t now;
    std::time(&now);
    // check if we've run past the timeout
    while(now < timeout)
    {
        // breed a dude
        // he'll automatically be in the population and replace the parent with
        // the lowest score
        individual dude = breed(this->pop);
        // check the dude
        // scores equal 0 are solutions, scores > 0 are suboptimal
        if(dude.getScore() == 0)
        {
            // grab'n'sack the dude
            this->solutions.push_back(dude);
        }
    }
    // return number of solutions found
    return this->solutions.size();
}

}
