#include "population.h"
#include "individual.h"

population::population(unsigned int n, unsigned int x, double pMutation)
{
    this->n = n;
    this->pMutation = pMutation;
    for(int i = 0; i < x; i++) this->individuals.push_back(new individual(n));
}

population::~population()
{
    //dtor
}

individual * population::getRandomIndividual()
{
    int key = rand() % this->individuals.size();
    if(!(this->individuals.at(key)->isLocked() ))
    {
        return this->individuals.at(key);
    }
    else if(key > 0 && key < this->individuals.size()-1)
    {
        if(!this->individuals.at(key+1)->isLocked())
        {
            return this->individuals.at(key+1);
        }
        else if(!this->individuals.at(key-1)->isLocked())
        {
            return this->individuals.at(key-1);
        }
    }
    return this->getRandomIndividual();
}

std::pair<individual*,individual*> population::chooseTwoRandom()
{
    individual * parent1 = this->getRandomIndividual();
    individual * parent2 = this->getRandomIndividual();
    if(parent1->getScore() > parent2->getScore())
    {
        individual * temp = parent1;
        parent1 = parent2;
        parent2 = temp;
    }
    return std::pair<individual *, individual *>(parent1, parent2);
}
