#include<iostream>
#include"include/population.h"
#include<stdlib.h>
#include<sstream>
#include<time.h>

int main()
{
    //Init of Parameters
    unsigned int    n=0;      //Size of Chessboard;
    unsigned int    x=1;      //How many Individuals per Generation;
    double p = 0.2;           //Probability of mutation;
    //unsigned int    i=4;      //Max Iteration;

    std::string input = "";
    //input n;
    while(true)
    {
        std::cout << "Size of Chessboard (min 4) ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> n)
        {
            if(n>3)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    //input x;
    while(true)
    {
        std::cout << "Number of Individuals " << std::endl;
        std::cout << "(Tipp: Small n big Number) = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> x)
            break;
        std::cout << "Invalid" << std::endl;
    }
    //input p;
    while(true)
    {
        std::cout << "Probability for Mutation " << std::endl;
        std::cout << "(Tipp: Standard is 0.1) = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> p)
            break;
        std::cout << "Invalid" << std::endl;
    }
    //summary of Inputs
    std::cout << "Size = " << n << std::endl;
    std::cout << "Induviduen = " << x << std::endl;
    std::cout << "Mutation = " << p << std::endl;

    return 0;
}
