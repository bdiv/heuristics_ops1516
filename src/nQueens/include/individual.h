#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "sys/types.h"
#include <vector>

class individual
{
    public:
        individual(unsigned int n); // constructor
        virtual ~individual();      // destructor
        unsigned int getScore();    // returns the score of the individual
        std::vector<unsigned int> & getVector(); // returns reference to the position vector
    protected:
    private:
        std::vector<unsigned int> v; // vector holding the horizontal positions of the queens
        unsigned int n;              // constant number of rows
        bool locked;                 // flag indicating this individual is used in a variation operator
        unsigned int score;          // score of the individual
        void score();                // calculates the score of the individual
        void init();                 // initialize class variables
};

#endif // INDIVIDUAL_H
