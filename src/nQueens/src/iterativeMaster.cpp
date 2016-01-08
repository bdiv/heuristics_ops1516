#include "../include/iterativeMaster.h"
#include<iostream>


namespace nQueens {

/*
    This class is a universal iterative solver for the nQueens problem.
    It wraps around the population and uses defined functors (crossbreedFunctor) to change this
    population in a way specified by genomic algorithms
*/

// simple constructor
iterativeMaster::iterativeMaster(unsigned int n, unsigned int x, double pMutation): pop(population(n,x,pMutation))
{
    this->variationCounter= 0;
    this->checkPopulation(this->pop);
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
std::vector<std::vector <unsigned int> > & iterativeMaster::getSolutions()
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
        std::vector<unsigned int> dude = breed(this->pop);
        this->variationCounter++;
        //std::cout << "Variation: " << this->variationCounter << std::endl;
        if(this->variationCounter % 20 == 0) this->uniquifyPopulation(pop);
        // if score is 0 we have found a solution, scores > 0 are suboptimal
        if(dude[dude.size()-1] == 0)
        {
            // grab and sack the dude
            //std::vector<std::vector<unsigned int>> isoForms = iterativeMaster::generateIsoForms(dude);
            //this->solutions.insert(this->solutions.end(), isoForms.begin(), isoForms.end());
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
        // he'll matically be in the population and replace the parent with
        // the lowest score
        std::vector<unsigned int> dude = breed(this->pop);
        this->variationCounter++;
        //std::cout << "Variation: " << this->variationCounter << std::endl;
        if(this->variationCounter % 20 == 0) this->uniquifyPopulation(pop);
        // check the dude
        // scores equal 0 are solutions, scores > 0 are suboptimal

        if(dude[dude.size()-1] == 0)
        {
            // grab'n'sack the dude
            //std::vector<std::vector<unsigned int>> isoForms = iterativeMaster::generateIsoForms(dude);
            //this->solutions.insert(this->solutions.end(), isoForms.begin(), isoForms.end());
            this->solutions.push_back(dude);
        }
    }
    // return number of solutions found
    return this->solutions.size();
}

unsigned int iterativeMaster::solve(crossbreedFunctor & breed)
{

    // save number of current solutions
    unsigned int n = this->solutions.size();
    // while nothing changes
    while(n == this->solutions.size())
    {
        // breed a dude
        // he'll automatically be in the population and replace the parent with
        // the lowest score
        std::vector<unsigned int> dude = breed(this->pop);
        this->variationCounter++;
        //std::cout << "Variation: " << this->variationCounter << std::endl;
        if(this->variationCounter % 20 == 0) this->uniquifyPopulation(pop);
        //pop.printAll();
        // check the dude
        // scores equal 0 are solutions, scores > 0 are suboptimal
        if(dude[dude.size()-1] == 0)
        {
            // grab'n'sack the dude and it's isoForms
            //std::vector<std::vector<unsigned int>> isoForms = iterativeMaster::generateIsoForms(dude);
            //this->solutions.insert(this->solutions.end(), isoForms.begin(), isoForms.end());
            this->solutions.push_back(dude);
        }
    }
}

void iterativeMaster::generateIsoForms(std::vector<std::vector<unsigned int>> & v)
{
    std::vector<std::vector<unsigned int>> isoForms;
    for(int i = 0; i < v.size(); i++)
    {
        std::vector<unsigned int> solution = v[i];
        iterativeMaster::invert(solution);
        isoForms.push_back(solution);
        // 90°
        iterativeMaster::swapReorder(solution);
        isoForms.push_back(solution);
        iterativeMaster::invert(solution);
        isoForms.push_back(solution);
        // 180°
        iterativeMaster::swapReorder(solution);
        isoForms.push_back(solution);
        iterativeMaster::invert(solution);
        isoForms.push_back(solution);
        // 270°
        iterativeMaster::swapReorder(solution);
        isoForms.push_back(solution);
        iterativeMaster::invert(solution);
        isoForms.push_back(solution);
    }
    v.insert(v.end(),isoForms.begin(),isoForms.end());
    iterativeMaster::uniquifySolutions(v);
}

void iterativeMaster::invert(std::vector<unsigned int> & v)
{
  unsigned int temp = 0;
  unsigned int n = v.size();
  // we only have to got until we reach the middle
  // center element of uneven vectors remains in place
  for(unsigned int i = 0; i <= n/2; i++)
  {
    // triangle swap
    temp = v.at(i);
    v.at(i) = v.at(n-1-i);
    v.at(n-1-i) = temp;
  }
}

/* swap and reaorder coordinates of our vector
           (3,1,4,2,0) will become (4,1,3,0,2)

            0 3      3 0      0 4
            1 1      1 1      1 1
            2 4  ->  4 2  ->  2 3
            3 2      2 3      3 0
            4 0      0 4      4 2

            so in essence: values will become keys and vice verca, additionally the new keys are sorted again
*/
void iterativeMaster::swapReorder(std::vector<unsigned int> & v)
{
  std::vector<unsigned int> temp;
  temp.assign(v.begin(), v.end());
  for(auto it = v.begin(); it != v.end(); it++)
  {
    (*it) = std::distance(temp.begin(), std::find(temp.begin(), temp.end(), std::distance(v.begin(), it)));
  }

}

void iterativeMaster::uniquifySolutions(std::vector< std::vector<unsigned int >> & v)
{
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
}

void iterativeMaster::checkPopulation(population & pop)
{
  for(int i = 0; i < pop.getIndividuals().size(); i++)
  {
    if(pop.getIndividuals()[i][pop.getIndividuals()[0].size()-1] == 0) this->solutions.push_back(pop.getIndividuals()[i]);
  }
}

void iterativeMaster::uniquifyPopulation(population & pop)
{
  std::vector<std::vector<unsigned int>>& individuals = pop.getIndividuals();
  for(int i = 1; i < individuals.size()-1; i++)
  {
    if(std::equal(individuals[i].begin(), individuals[i].end(), individuals[i+1].begin()) ||
       std::equal(individuals[i].begin(), individuals[i].end(), individuals[i-1].begin()))
    {
        //std::cout << "uniquify individual " << i << std::endl;
        pop.generateRandomizedIndividual(individuals[i]);
        pop.scoreIndividual(individuals[i]);
    }
  }
}

}
