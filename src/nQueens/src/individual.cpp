#include"individual.h"
#include<iostream>

namespace nQueens {

//konstruktor nur mit Länge n
//Bei der Init werden immer die gleichen hergestellt :-(  können wir das noch anders machen / oder ist das egal ?
individual::individual(unsigned int n)
{
    this->n = n;
    for(unsigned int i = 0; i < n; i++) this->v.push_back(i);
    std::random_shuffle(v.begin(),v.end());
    this->init();
}

//Konstruktor mit Länge n und einen Vektor (Für Paarung ?)
individual::individual(unsigned int n, std::vector<unsigned int> & v)
{
    this->n = n;
    this->init();
    this->v = v;
}

//destruktor
individual::~individual()
{
    //dtor
}

//Methoden
void individual::printIndu()
{
    std::cout << "Position der Damen horizontal = ";
    for(int i=0;i<n;i++)
    {
        std::cout << v[i] << ",";
    }
    std::cout << std::endl;
    int score = getScore();
    std::cout << "Score = " << score << std::endl;
    //std::cout << "Flag =  " << locked << std::endl;
}

void individual::init()
{
    this->locked = false;
    //individual::score(this->v); // wusste nicht was das machen soll ?
    this->sc = score(this->v);
}

unsigned int individual::getScore()
{
    return  this->sc;
}

bool individual::isLocked()
{
    return this->isLocked();
}

std::vector<unsigned int> & individual::getVector()
{
    return this->v;
}

/*
*   scores the individual based on the following conditions:
*   - score = 0 is a solution
*   - increase score if:
*       - value(i) == value(j) -> horizontal overlap
*       - value(i) == value(j) + (j-i) -> diagonal overlap
*       - value(i) == value(j) - (j-i) -> the other diagonal overlap
*/
unsigned int individual::score(std::vector<unsigned int> & v)
{
    unsigned int sc = 0;
    unsigned int n = v.size();
    for(unsigned int i = 0; i < n-1; i++)
    {
        for(unsigned int j = i+1; j < n; j++)
        {
            unsigned int m = j-i;
            if( v.at(i) == v.at(j) ||
                v.at(i) == (v.at(j) + m) ||
                v.at(i) == (v.at(j) - m))
            {
                sc++;
            }
        }
    }
    return sc;
}
}
