#include"../include/population.h"
#include<iostream>
#include<algorithm>



namespace nQueens {

std::random_device population::rd;

std::vector<std::vector<unsigned int>> & population::getIndividuals()
{
    return this->individuals;
}

double population::getPMutation()
{
    return this->pMutation;
}

//Konstruktor
population::population(unsigned int n, unsigned int x, double pMutation) : n(n), pMutation(pMutation)
{
    // create n dudes for our population
    std::random_device & r = population::rd;
    this->mt = std::mt19937(r());
    this->randomIndividual = std::uniform_int_distribution<>(0,x-1);
    this->randomPosition = std::uniform_int_distribution<>(0,n-1);
    this->randomMutation = std::uniform_real_distribution<>(0,1);
    for(unsigned int i = 0; i < x; i++)
    {
        std::vector<unsigned int> indi;
        this->generateRandomizedIndividual(indi);
        this->individuals.push_back(indi);
    }
    this->scoreAll();
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
        // score individual
        individuals[i][this->n] = this->scoreIndividual(individuals[i]);
    }
}

// grab a random dude out of the bag and throw it in the callers face
std::vector<unsigned int>& population::getRandomIndividual()
{

    // choose a key at random
    unsigned int key = this->randomIndividual(this->mt);
    //std::cout << "Choose random parent:" << key << std::endl;
    // if the individual ist not locked, hence not engaged in intercourse
    return this->individuals.at(key);
}

// choose two dudes at random and throw them in somebodies face
std::pair<std::vector<unsigned int>&,std::vector<unsigned int>&> population::chooseTwoRandom()
{

    unsigned int key1 = this->randomIndividual(this->mt);
    unsigned int key2 = this->randomIndividual(this->mt);
    //std::cout << "Parents: " << key1 << ", " << key2 << std::endl;
    // we get one dude
    while(key1==key2)
    {
        key2=this->randomIndividual(this->mt);
    }
    //std::cout << "Parents: " << key1 << ", " << key2 << std::endl;

    //std::cout << "get parent 1" << std::endl;
    std::vector<unsigned int>& parent1 = this->individuals.at(key1);
    // we get another
    //std::cout << "get parent 2" << std::endl;
    std::vector<unsigned int>& parent2 = this->individuals.at(key2);

    //std::cout << "got 2 random parents" << std::endl;
    // order them according to their score
    // simplifies some things later
    if(parent1[parent1.size()-1] < parent2[parent2.size()])
    {
        //std::cout << "triangle swap" << std::endl;
        // triangle swap
        std::vector<unsigned int> temp = parent1;
        parent1 = parent2;
        parent2 = temp;
    }
    //std::cout << "peace out with parents" << std::endl;
    // tie up the dudes and throw them
    return std::pair<std::vector<unsigned int> &, std::vector<unsigned int> &>(parent1, parent2);
}

void population::generateIndividual(std::vector<unsigned int> & i)
{
  if(i.size() != 0) i.clear();
  for(unsigned int x = 0; x < this->n; x++)
  {
    i.push_back(x);
  }
  i.push_back(0);
}

void population::generateRandomizedIndividual(std::vector<unsigned int> & i)
{
  //std::cout << "Randomize Individual:" << std::endl;
  this->generateIndividual(i);
  for(unsigned int x = 0; x < i.size()-1; x++)
  {
    unsigned int temp = i[x];
    unsigned int key = this->randomPosition(this->mt);
    //std::cout << x << ">" << key << " values: " << i[x] << ">" << i[key] << std::endl;
    i[x] = i[key];
    i[key] = temp;
  }
}

unsigned int population::scoreIndividual(std::vector<unsigned int> & v)
{
    // initiate score to zero
    unsigned int sc = 0;
    // save length of the vector for convenience reasons
    unsigned int n = v.size()-1;
    // loop through the vector
    // we have to check every cell for a collision, that's why we loop two times
    // bc our vector is equivalent to a symmetric matrix it is optimized
    // the way you see below
    for(unsigned int i = 0; i < n-1; i++)
    {
        // we dont have to check cell (1,1) (2,2) and so on...
        // we dont have to check cell (2,1) if we checked (1,2) -> symmetry
        for(unsigned int j = i+1; j < n; j++)
        {
            // this part calculates diagonal overlaps using the
            // distance of j and i, pls note: j>i
            // also looks for horizontal overlaps
            unsigned int m = j-i;
            if( v.at(i) == v.at(j) ||
                v.at(i) == (v.at(j) + m) ||
                v.at(i) == (v.at(j) - m))
            {
                // that fucker's bad and he should feel bad
                // -100 points for Griffindor
                sc++;
            }
        }
    }
    // return calculated score
    return sc;
}

void population::printIndu(std::vector<unsigned int> & v)
{
    std::cout << "Position der Damen horizontal = ";
    for(unsigned int i=0;i<this->n;i++)
    {
        std::cout << v[i] << ",";
    }
    std::cout << std::endl;
    std::cout << "Score = " << v[this->n] << std::endl;
    //std::cout << "Flag =  " << locked << std::endl;
}
}
