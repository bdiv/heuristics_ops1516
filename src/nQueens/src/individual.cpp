#include "individual.h"

individual::individual(unsigned int n)
{
    this->n = n;
    for(int i = 0; i < n; i++) this->v->push_back(i);
    std::random_shuffle(v.begin(),v.end());
    this->init();
}

individual::individual(unsigned int n, std::vector & <unsigned int> v)
{
    this->n = n;
    this->init();
    this->v = v;
}

individual::~individual()
{
    //dtor
}

unsigned int individual::getScore()
{
    return this->score;
}

void individual::init()
{
    this->locked = false;
    this->score();
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
void individual::score()
{
    for(int i = 0; i < n-1; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            unsigned int m = j-i;
            if( this->v->at(i) == this->v->at(j) ||
                this->v->at(i) == (this->v->at(j) + m) ||
                this->v->at(i) == (this->v->at(j) - m))
            {
                this->score++;
            }
        }
    }

}
