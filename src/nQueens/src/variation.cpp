#include "../include/variation.h"
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
individual variation::matched_crossover::operator()(population & pop)
{
    // initialization block...
    // choose two parents at random
    std::pair<individual&, individual&> parents = pop.chooseTwoRandom();
    individual & parent1 = parents.first;
    individual & parent2 = parents.second;

    // vectors all have the same size
    unsigned int n = parent1.getVector().size();
    // choose the area to work on -> this is a pair of vector entry keys: (startKey, endKey)
    std::pair<unsigned int, unsigned int> area = variation::chooseRandomArea(n);
    unsigned int start = area.first;
    unsigned int stop = area.second;
    // copy the vectors
    std::pair<individual, individual > children = generateChildren(parent1,parent2);
    // here we generate two children
    std::vector<unsigned int> child1 = children.first.getVector();
    std::vector<unsigned int> child2 = children.second.getVector();
    //....

    // optimization for certain conditions (start = 0, stop = n;)
    if(start != 0 && stop != n-1)
    {
        // step through our area
        for(unsigned int i = start; i <= stop; i++)
        {
            // if the vector entries are not equal
            if(child1.at(i) != child2.at(i))
            {
                // step through vector of child2 with an iterator
                // TODO: fix this, I think it'll work but i dont know the reaon for that iterator
                // right now
                std::vector<unsigned int>::iterator it = std::find(child2.begin(), child2.end(), child1.at(i));
                // note: I dont even use the iterator... wtf... seems that i just randomly stopped developing that
                unsigned int temp = child1.at(i);
                child1.at(i) = child2.at(i);
                child2.at(i) = temp;
            }
            // else nothing todo bc entries are equal, switching them makes no sense
        }
    }

    // parent1 always has the worst score of the two parents
    if(individual::score(child1) <= individual::score(child2))
    {
        // overwrite parent with best child
        parent1 = individual(n,child1);
    }
    else
    {
        // overwrite parent with best child
        parent1 = individual(n,child2);
    }
    return parent1;
}

/*  This crossover produces only one child by selecting an area in the first parent, erasing all elements of said area in parent two
*   then insert that area from parent1 into parent2 moving back all elements that are in the way
*   see: "Landscape analysis and efficient metaheuristics for solving the n-queens problem"
*/
individual variation::ordered_crossover::operator()(population & pop)
{
    // initialization block
    // choose two random parents
    std::pair<individual&, individual&> parents = pop.chooseTwoRandom();
    individual & parent1 = parents.first;
    individual & parent2 = parents.second;
    // save the length of the vector for convenience
    unsigned int n = parent1.getVector().size();
    // choose an area to operate in
    std::pair<unsigned int, unsigned int> area = variation::chooseRandomArea(n);
    unsigned int start = area.first;
    unsigned int stop = area.second;
    // generate children
    std::pair<individual, individual> children = variation::generateChildren(parent1, parent2);
    std::vector<unsigned int> child1 = children.first.getVector();
    std::vector<unsigned int> child2 = children.second.getVector();

    /*  if our range equals vector child1 we dont have to do anything at all. child1 equals parent1. We would completely overwrite
    *   overwrite child2 so that child1 == child2 and after that set parent1 = child2. Which is pointless.
    */
    if(start > 0 && stop < n-1)
    {
        // step through our working area
        for(unsigned int i = start; i <= stop; i++)
        {
            // well... this reads nice doesnt it?
            // here we search child2 for the element in child1 at position i
            // once found we erase it
            child2.erase(std::find(child2.begin(), child2.end(), (*(child1.begin()+i))));
        }
        // after all matching elements are erased in child2 we insert the working area from child1 into it
        // pushing back all elements that are in the way
        child2.insert(child2.begin()+start, child1.begin()+start, child1.begin()+stop);
        // parent1 always has the worst score
        parent1 = individual(n, child2);
        // we don't care for child1, he was just a stepping stone
    }
    // return new individual
    return parent1;
}

// mutate all things!!!
void variation::mutation(individual & i, double pMutation)
{
    // do a statistical test if we mutate or not using the
    // mutation probabilitie pMutation
    double test = (double)rand() / RAND_MAX;
    // if the test is a success
    if(test <= pMutation)
    {
        // save individuals vector for convenience
        std::vector<unsigned int> & v = i.getVector();
        // create two keys at random
        unsigned int key1 = rand() % v.size()-1;
        unsigned int key2 = rand() % v.size()-1;
        // add one to key2 if they are equal
        if(key1 == key2 && key2 == 0) key2++;
        // substract one from key2 if addition would leave us out of bounds
        if(key1 == key2 && key2 == v.size()-1) key2--;
        // triangle exchange of the values bc std::swap is bullshit...
        unsigned int x = v[key1];
        v[key1] = v[key2];
        v[key2] = x;
    }
}

// choose a random area to work on
std::pair<unsigned int, unsigned int> variation::chooseRandomArea(unsigned int n)
{
    // just another way of assigning two random values that should not be equal afterwards
    unsigned int start = rand() % n-1;
    unsigned int stop = start;
    // until they are not equal
    while(stop == start)
    {
        stop = rand() % n-1;
    }
    // triangle swap if start<stop, bc we cant have that right?
    if(start < stop)
    {
        unsigned int temp = start;
        start = stop;
        stop = temp;
    }
    // return our area
    return std::pair<unsigned int, unsigned int>(start, stop);
}

// generates children from the parents by copying their vectors
std::pair<individual, individual> variation::generateChildren(individual & parent1, individual & parent2)
{
     // copy vector of parent1
    std::vector<unsigned int> child1 = std::vector<unsigned int>();
    child1.resize(std::distance(parent1.getVector().begin(), parent1.getVector().end()));
    std::copy(parent1.getVector().begin(), parent1.getVector().end(), child1.begin());
    // copy vector of parent2
    std::vector <unsigned int> child2 = std::vector<unsigned int>();
    child2.resize(std::distance(parent2.getVector().begin(), parent2.getVector().end()));
    std::copy(parent2.getVector().begin(), parent2.getVector().end(), child2.begin());

    // generate individuals out of the vectors and return a pair-structure
    return std::pair<individual,individual>(individual(child1.size(),child1), individual(child2.size(),child2));
}
}
