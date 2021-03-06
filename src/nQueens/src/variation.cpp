#include "../include/variation.h"
#include <algorithm>
#include<iostream>

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
std::vector<unsigned int> variation::matched_crossover::operator()(population & pop)
{
    // initialization block...
    // choose two parents at random
    std::pair<std::vector<unsigned int>&, std::vector<unsigned int>&> parents = pop.chooseTwoRandom();
    std::vector<unsigned int> & parent1 = parents.first;
    std::vector<unsigned int> & parent2 = parents.second;
    // vectors all have the same size
    unsigned int n = parent1.size();
    // choose the area to work on -> this is a pair of vector entry keys: (startKey, endKey)
    std::pair<unsigned int, unsigned int> area = variation::chooseRandomArea(n, pop);
    unsigned int start = area.first;
    unsigned int stop = area.second;
    // copy the vectors
    std::pair<std::vector<unsigned int>, std::vector<unsigned int> > children = generateChildren(parent1,parent2);
    // here we generate two children
    std::vector<unsigned int> child1 = children.first;
    std::vector<unsigned int> child2 = children.second;
    //....
    // optimization for certain conditions (start = 0, stop = n;)
    if(start != 0 && stop != n-2)
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
                //std::vector<unsigned int>::iterator it = std::find(child2.begin(), child2.end(), child1.at(i));
                // note: I dont even use the iterator... wtf... seems that i just randomly stopped developing that
                unsigned int temp = child1.at(i);
                child1.at(i) = child2.at(i);
                child2.at(i) = temp;
            }
            // else nothing todo bc entries are equal, switching them makes no sense
        }
    }
    variation::mutation(child1, pop);
    variation::mutation(child2, pop);
    child1[child1.size()-1] = pop.scoreIndividual(child1);
    child2[child2.size()-1] = pop.scoreIndividual(child2);
    // parent1 always has the worst score of the two parents
    if(child1[child1.size()-1] <= child2[child2.size()-1])
    {
        // overwrite parent with best child
        parent1 = child1;
    }
    else
    {
        // overwrite parent with best child
        parent1 = child2;
    }
    return parent1;
}

/*  This crossover produces only one child by selecting an area in the first parent, erasing all elements of said area in parent two
*   then insert that area from parent1 into parent2 moving back all elements that are in the way
*   see: "Landscape analysis and efficient metaheuristics for solving the n-queens problem"
*/
std::vector<unsigned int> variation::ordered_crossover::operator()(population & pop)
{

    // initialization block
    // choose two random parents
    std::pair<std::vector<unsigned int>&, std::vector<unsigned int>&> parents = pop.chooseTwoRandom();
    std::vector<unsigned int> & parent1 = parents.first;
    std::vector<unsigned int> & parent2 = parents.second;
    // save the length of the vector for convenience
    unsigned int n = parent1.size();
    // choose an area to operate in
    std::pair<unsigned int, unsigned int> area = variation::chooseRandomArea(n, pop);
    unsigned int start = area.first;
    unsigned int stop = area.second;
    // generate children
    std::pair<std::vector<unsigned int>, std::vector<unsigned int>> children = variation::generateChildren(parent1, parent2);
    std::vector<unsigned int> child1 = children.first;
    std::vector<unsigned int> child2 = children.second;


    /*  if our range equals vector child1 we dont have to do anything at all. child1 equals parent1. We would completely overwrite
    *   overwrite child2 so that child1 == child2 and after that set parent1 = child2. Which is pointless.
    */
    if(child1.size()-2 > stop-start)
    {
        // step through our working area
        for(unsigned int i = start; i <= stop; i++)
        {
            // well... this reads nice doesnt it?
            // here we search child2 for the element in child1 at position i
            // once found we erase it
            unsigned int elementChild1 = child1[i];
            auto placeOfElement = std::find(child2.begin(), child2.end()-1, elementChild1);
            if(placeOfElement != child2.end()-1)
            {
                child2.erase(placeOfElement);
            }
            else
            {
                std::cout << "couldn't find element!!! panic!!! this should not happen. Pls contact the programmer!! I think i'm burning!" << std::endl;
            }
        }
        // after all matching elements are erased in child2 we insert the working area from child1 into it
        // pushing back all elements that are in the way
        child2.insert(child2.begin()+start, child1.begin()+start, child1.begin()+stop+1);
        // parent1 always has the worst score
        variation::mutation(child2,pop);
        child2[child2.size()-1] = pop.scoreIndividual(child2);
        parent1 = child2;
        // we don't care for child1, he was just a stepping stone
    }
    // return new individual
    return parent1;
}

// mutate all things!!!
void variation::mutation(std::vector<unsigned int> & i, population & pop)
{
    // do a statistical test if we mutate or not using the
    // mutation probabilitie pMutation

    // if the test is a success
    if(pop.randomMutation(pop.mt) <= pop.getPMutation())
    {
        // save individuals vector for convenience
        // create two keys at random
        unsigned int key1 = pop.randomPosition(pop.mt);
        unsigned int key2 = pop.randomPosition(pop.mt);
        // add one to key2 if they are equal
        if(key1 == key2 && key2 == 0) key2++;
        // substract one from key2 if addition would leave us out of bounds
        if(key1 == key2 && key2 == i.size()-1) key2--;
        // triangle exchange of the values bc std::swap is bullshit...
        unsigned int x = i[key1];
        i[key1] = i[key2];
        i[key2] = x;
    }
}

// choose a random area to work on
std::pair<unsigned int, unsigned int> variation::chooseRandomArea(unsigned int n, population & pop)
{
    // just another way of assigning two random values that should not be equal afterwards
    unsigned int start = pop.randomPosition(pop.mt);
    unsigned int stop = start;
    // until they are not equal
    while(stop == start)
    {
        stop = pop.randomPosition(pop.mt);
    }
    // triangle swap if start>stop, bc we cant have that right?
    if(start > stop)
    {
        unsigned int temp = start;
        start = stop;
        stop = temp;
    }
    // return our area
    return std::pair<unsigned int, unsigned int>(start, stop);
}

// generates children from the parents by copying their vectors
std::pair<std::vector<unsigned int>, std::vector<unsigned int>> variation::generateChildren(std::vector<unsigned int> & parent1, std::vector<unsigned int> & parent2)
{
     // copy vector of parent1
    std::vector<unsigned int> child1 = parent1;
    // copy vector of parent2
    std::vector <unsigned int> child2 = parent2;

    // generate individuals out of the vectors and return a pair-structure
    return std::pair<std::vector<unsigned int>,std::vector<unsigned int>>(child1,child2);
}
}
