#include"population.h"
#include<iostream>


namespace nQueens {


//Konstruktor
population::population(unsigned int n, unsigned int x, double pMutation)
{
    this->n = n;
    this->pMutation = pMutation;
    //fehlermeldung :
    //invalid user-defined conversion from 'nQueens::individual*' to 'const value_type& {aka const nQueens::individual&}'
    //ob das jetzt von dir so vorgesehen war kann ich nicht sagen :-(
    for(unsigned int i = 0; i < x; i++)
    {
        individual neues(n);
        this->individuals.push_back(neues);
    }
}

//Destruktor
population::~population()
{
    //dtor
}

//Methoden
/*
void population::scoreAll()
{
    for(int i=0;i<this->individuals.size();i++)
    {
        std::cout << "das da " << std::endl;
        individuals[i].score(individuals[i].v);
    }
}
*/
void population::printAll()
{
    for(int i=0;i<this->individuals.size();i++)
    {
        individuals[i].printIndu();
    }
}

//fehlermeldung:
//prototype for 'nQueens::individual* nQueens::population::getRandomIndividual()' does not match any in class 'nQueens::population'|
/*
individual population::getRandomIndividual()
{
    unsigned int key = rand() % this->individuals.size();
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
*/
//Fehlermeldung:
//prototype for 'std::pair<nQueens::individual*, nQueens::individual*> nQueens::population::chooseTwoRandom()' does not match any in class 'nQueens::population'|
//Ich kenn das nur das man in beiden mit dem & arbeitet so hab ich es verwendet beim raytracer
//aber wenn ich das mache dann ist die Fehleremeldung zwei zeilen darunter
//sorry
/*
std::pair<individual*,individual*> population::chooseTwoRandom()
{
    individual * parent1 = this->getRandomIndividual();
    individual * parent2 = this->getRandomIndividual();
    if(parent1->getScore() < parent2->getScore())
    {
        individual * temp = parent1;
        parent1 = parent2;
        parent2 = temp;
    }
    return std::pair<individual *, individual *>(parent1, parent2);
}
*/
}
