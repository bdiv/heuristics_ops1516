#include "../include/iterativeMaster.h"


namespace nQueens {

iterativeMaster::iterativeMaster(unsigned int n, unsigned int x, double pMutation): pop(population(n,x,pMutation))
{

}

iterativeMaster::~iterativeMaster()
{
    //dtor
}

population& iterativeMaster::getPopulation()
{
    return this->pop;
}

unsigned int iterativeMaster::getVariationsCounter()
{
    return this->variationCounter;
}

std::vector <individual> & iterativeMaster::getSolutions()
{
    return this->solutions;
}

unsigned int iterativeMaster::solve(unsigned int maxVariations, crossbreedFunctor& breed)
{
    for(unsigned int i = 0; i < maxVariations; i++)
    {
        individual dude = breed(this->pop);
        if(dude.getScore() == 0)
        {
            this->solutions.push_back(dude);
        }
    }
    return this->solutions.size();
}

unsigned int iterativeMaster::solve(std::time_t timeout, crossbreedFunctor & breed)
{
    std::time_t now;
    std::time(&now);
    while(now < timeout)
    {
        individual dude = breed(this->pop);
        if(dude.getScore() == 0)
        {
            this->solutions.push_back(dude);
        }
    }
    return this->solutions.size();
}

}
