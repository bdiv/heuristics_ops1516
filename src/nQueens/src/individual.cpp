#include"../include/individual.h"
#include<iostream>
#include<stdlib.h>
#include<time.h>

namespace nQueens {

//konstruktor nur mit Länge n
//Bei der Init werden immer die gleichen hergestellt :-(  können wir das noch anders machen / oder ist das egal ?
/*
individual::individual(unsigned int n): n(n)
{
    // create vector with elements [0,1,2...n]
    for(unsigned int i = 0; i < n; i++) this->v.push_back(i);
    // init seed with our pointer in memory converted to int
    srand(int(this));
    // randomize the elements
    for(unsigned int i = 0; i < n; i++) std::swap(v.at(i), v.at(srand(int(this))%n));
    this->init();
}
*/
individual::individual(unsigned int n)
 {
     int s = 0;
     this->n = n;
     for(int i=0;i<n;)
     {
         int c = rand() %n;
         for(int w=0;w<v.size();w++)
         {
             if(c==v[w])s = 1;
         }
         if(s==0)
         {
             i++;
             v.push_back(c);
         }
         else s = 0;
     }
     this->init();
 }

//Second constructor. n is the number of rows/genes, v is an already initialized vector of genes
// thats why we dont need to create one
individual::individual(unsigned int n, std::vector<unsigned int> & v): n(n),v(v)
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
    // individual is not in use für crossbreeding, so no lock-flag
    this->locked = false;
    // score the individual we just created
    this->sc=score(this->v);
}

unsigned int individual::getScore()
{
    return  this->sc;
}

bool individual::isLocked()
{
    return false;//this->isLocked();
}

std::vector<unsigned int> & individual::getVector()
{
    return this->v;
}

/*
*   scores the individual based on the following conditions:
*   - score = 0 is a solution
*   - increase score if:
*       - value(i) == value(j) -> horizontal overlap /
*       wir können gar keinen horizontalen overlap haben ? da müssten 2 einträge an einer Stelle im Vektor sein
*       ich glaube du meinst vertikal.
*       - value(i) == value(j) + (j-i) -> diagonal overlap
*       - value(i) == value(j) - (j-i) -> the other diagonal overlap

        Ich komm auch noch nicht ganz mit mit dem Score
        z.B v = 4,2,1,0,3; wenn ich das aufzeichne komm ich auf 3 aber es ist 4;
        z.B v = 4,0,3,2,1; wenn ich das aufzeichne komm ich auf 3 aber es ist 4;
        z.B v = 1,2,3,4,0; wenn ich das aufzeichne komm ich auf 3 aber es ist 6;
        z.B v = 1,3,2,0,4; hier stimmt es aber mit 2

        ich dachte du zählst nur nach unten ?
        Irgendwas ist noch nicht koscher aber ich seh nichts :-(
#############################
    Es verhält sich wie es soll. Beispiel: 1,2,3,4,0
    0 1 2 3 4
  0   X
  1     X
  2       X
  3         X
  4 X

  Kollisionen sind:
  {(0,1),(1,2)}, {(0,1),(2,3)}, {(0,1),(3,4)}, -> die sind alle trivial und die zählst du auch
  {(1,2),(2,3)}, {(1,2),(3,4)}, {(2,3),(3,4)}  -> die vergisst du, müssen aber mitgezählt werden.

*/
unsigned int individual::score(std::vector<unsigned int> & v)
{
    // initiate score to zero
    unsigned int sc = 0;
    // save length of the vector for convenience reasons
    unsigned int n = v.size();
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
}
