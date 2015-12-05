#include "variation.h"
#include <algorithm>

namespace nQueens {
variation::variation()
{
    //ctor
}

variation::~variation()
{
    //dtor
}

/*
*   chooses two random individuals of a population
*   chooses a random range using the vector size
*   elements in the range get swapped
*
*/
void variation::matched_crossover(population & pop)
{
    // initialization block...
    std::pair<individual*, individual*> parents = pop.chooseTwoRandom();
    individual * parent1 = parents.first;
    individual * parent2 = parents.second;
    unsigned int n = parent1->getVector().size();
    std::pair<unsigned int, unsigned int> area = variation::chooseRandomArea(n);
    unsigned int start = area.first;
    unsigned int stop = area.second;
    std::pair<std::vector<unsigned int>, std::vector<unsigned int> > children = generateChildren(parent1,parent2);
    std::vector<unsigned int> child1 = children.first;
    std::vector<unsigned int> child2 = children.second;
    //....

    // optimization for certain conditions (start = 0, stop = n;)
    if(start != 0 && stop != n-1)
    {
        for(unsigned int i = start; i <= stop; i++)
        {
            if(child1.at(i) != child2.at(i))
            {
                std::vector<unsigned int>::iterator it = std::find(child2.begin(), child2.end(), child1.at(i));
                unsigned int temp = child1.at(i);
                child1.at(i) = child2.at(i);
                child2.at(i) = temp;
            }
        }
    }

    // parent1 always has the worst score of the two parents
    if(individual::score(child1) <= individual::score(child2))
    {
        parent1 = new individual(n,child1);
    }
    else
    {
        parent1 = new individual(n,child2);
    }
}

/*  This crossover produces only one child by selecting an area in the first parent, erasing all elements of said area in parent two
*   then insert that area from parent1 into parent2 moving back all elements that are in the way
*   see: "Landscape analysis and efficient metaheuristics for solving the n-queens problem"
*/
void variation::ordered_crossover(population & pop)
{
    // initialization block
    std::pair<individual*, individual*> parents = pop.chooseTwoRandom();
    individual * parent1 = parents.first;
    individual * parent2 = parents.second;
    unsigned int n = parent1->getVector().size();
    std::pair<unsigned int, unsigned int> area = variation::chooseRandomArea(n);
    unsigned int start = area.first;
    unsigned int stop = area.second;
    std::pair<std::vector<unsigned int>, std::vector<unsigned int> > children = variation::generateChildren(parent1, parent2);
    std::vector<unsigned int> child1 = children.first;
    std::vector<unsigned int> child2 = children.second;

    /*  if our range equals vector child1 we dont have to do anything at all. child1 equals parent1. We would completely overwrite
    *   overwrite child2 so that child1 == child2 and after that set parent1 = child2. Which is pointless.
    */
    if(start > 0 && stop < n-1)
    {

        for(unsigned int i = start; i <= stop; i++)
        {
            child2.erase(std::find(child2.begin(), child2.end(), (*(child1.begin()+i))));
        }
        child2.insert(child2.begin()+start, child1.begin()+start, child1.begin()+stop);
        // parent1 always has the worst score
        parent1 = new individual(n, child2);
    }
}

void variation::mutation(individual & i, double pMutation)
{
    std::vector<unsigned int> & v = i.getVector();
    unsigned int key1 = rand() % v.size()-1;
    unsigned int key2 = rand() % v.size()-1;
    if(key1 == key2 && key2 == 0) key2++;
    if(key1 == key2 && key2 == v.size()-1) key2--;
    unsigned int x = v[key1];
    v[key1] = v[key2];
    v[key2] = x;
}

std::pair<unsigned int, unsigned int> variation::chooseRandomArea(unsigned int n)
{
    unsigned int start = rand() % n-2;
    unsigned int stop = start;
    while(stop == start)
    {
        stop = rand() % n-1;
    }
    if(start < stop)
    {
        unsigned int temp = start;
        start = stop;
        stop = temp;
    }
    return std::pair<unsigned int, unsigned int>(start, stop);
}

std::pair<std::vector<unsigned int>, std::vector<unsigned int> > variation::generateChildren(individual * parent1, individual * parent2)
{
     // copy vector of parent1
    std::vector<unsigned int> child1 = std::vector<unsigned int>();
    child1.resize(std::distance(parent1->getVector().begin(), parent1->getVector().end()));
    std::copy(parent1->getVector().begin(), parent1->getVector().end(), child1.begin());
    // copy vector of parent2
    std::vector <unsigned int> child2 = std::vector<unsigned int>();
    child2.resize(std::distance(parent2->getVector().begin(), parent2->getVector().end()));
    std::copy(parent2->getVector().begin(), parent2->getVector().end(), child2.begin());

    return std::pair<std::vector<unsigned int>,std::vector<unsigned int> >(child1, child2);
}
}
