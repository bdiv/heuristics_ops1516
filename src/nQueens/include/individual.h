#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "sys/types.h"
#include <vector>
#include <algorithm>

class individual
{
    public:
        static unsigned int score(std::vector<unsigned int> & v);                // calculates the score of the individual
        individual(unsigned int n); // constructor
        individual(unsigned int n, std::vector<unsigned int> & v);
        virtual ~individual();      // destructor
        unsigned int getScore();    // returns the score of the individual
        bool isLocked();
        std::vector<unsigned int> & getVector(); // returns reference to the position vector
    protected:
    private:
        std::vector<unsigned int> v; // vector holding the horizontal positions of the queens
        unsigned int n;              // constant number of rows
        bool locked;                 // flag indicating this individual is used in a variation operator
        unsigned int sc;          // score of the individual
        void init();                 // initialize class variables
};

#endif // INDIVIDUAL_H
