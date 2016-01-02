#include"../include/population.h"
#include<iostream>


namespace nQueens {


//Konstruktor
population::population(unsigned int n, unsigned int x, double pMutation) : n(n), pMutation(pMutation)
{
    // create n dudes for our population
    for(unsigned int i = 0; i < x; i++)
    {
        individual dude(n);
        this->individuals.push_back(dude);
    }
}

//Destruktor
population::~population()
{
    //dtor
}

//Methoden

// scores all individuals
// we have to do that if we create a new population
void population::scoreAll()
{
    // loop through all individuals
    for(unsigned int i=0;i<this->individuals.size();i++)
    {
        std::cout << "das da " << std::endl;
        // score individual
        // this will maybe become a static function in the future
        // for optimizing our vtables and use of memory
        individuals[i].score(individuals[i].getVector());
    }
}

void population::printAll()
{
    for(unsigned int i=0;i<this->individuals.size();i++)
    {
        individuals[i].printIndu();
    }
}

// grab a random dude out of the bag and throw it in the callers face
individual& population::getRandomIndividual()
{
    // choose a key at random
    unsigned int key = rand() % this->individuals.size()-1;
    // if the individual ist not locked, hence not engaged in intercourse
    if(!(this->individuals.at(key).isLocked() ))
    {
        // throw the dude
        return this->individuals.at(key);
    }
    // else, if we're not at the start or end
    else if(key > 0 && key < this->individuals.size()-1)
    {
        // check left and right neighbor of our dude
        if(!this->individuals.at(key+1).isLocked())
        {
            // not locked? nice! throw him!
            return this->individuals.at(key+1);
        }
        else if(!this->individuals.at(key-1).isLocked())
        {
            // not locked? nice! throw him!
            return this->individuals.at(key-1);
        }
    }
    // if all fails ... choose another dude
    // we really wanna throw one
    return this->getRandomIndividual();
}

// choose two dudes at random and throw them in somebodies face
std::pair<individual&,individual&> population::chooseTwoRandom()
{
    // we get one dude
    individual parent1 = this->getRandomIndividual();
    // we get another
    individual parent2 = this->getRandomIndividual();
    // they should not be the same dude
    while(parent1.getVector()==parent2.getVector())
    {
        // if they are, get another random dude
        parent2 = this->getRandomIndividual();
    }
    // order them according to their score
    // simplifies some things later
    if(parent1.getScore() < parent2.getScore())
    {
        // triangle swap
        individual temp = parent1;
        parent1 = parent2;
        parent2 = temp;
    }
    // tie up the dudes and throw them
    return std::pair<individual &, individual &>(parent1, parent2);
}
}
