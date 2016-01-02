#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "sys/types.h"
#include <vector>
#include <algorithm>

namespace nQueens{

class individual
{
    public:
        //konstruktor
        individual(unsigned int n);
        individual(unsigned int n, std::vector<unsigned int> & v);

        //destruktor
        virtual ~individual();

        //Methoden
        static unsigned int score(std::vector<unsigned int> & v);   // calculates the score of the individual
        unsigned int getScore();                                    // returns the score of the individual
        bool isLocked();                                            // returns if Flag is true / false
        std::vector<unsigned int> & getVector();                    // returns reference to the position vector
        void printIndu();                                           // gibt alle Werte eines Indi aus (später nur den Vektor);
    //protected:
    private: //to do getter und setter !
        //Members
        std::vector<unsigned int> v;    // vector holding the horizontal positions of the queens
        unsigned int n;                 // constant number of rows
        bool locked;                    // flag indicating this individual is used in a variation operator
        unsigned int sc;                // score of the individual
        void init();                    // initialize class variables
};
}
#endif // INDIVIDUAL_H
