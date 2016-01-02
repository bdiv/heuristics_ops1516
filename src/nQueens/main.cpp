#include<iostream>
#include<stdlib.h>
#include<sstream>
#include<time.h>
#include"include/population.h"
#include"include/variation.h"
#include"include/crossbreedFunctor.h"
#include"include/iterativeMaster.h"

int main()
{
    //Init of Parameters
    unsigned int    n=0;        //Size of Chessboard;
    unsigned int    x=0;        //How many Individuals per Generation;
    double          p=0.0;      //Probability of mutation;
    unsigned int    sol=0;      //Which solve Methode
    unsigned int    i=0;        //Max Iteration;
    unsigned int    t=0;        //Time for Working;
    time_t          now;        //Time now
    unsigned int    w=0;        //Which crossbread;
    std::vector<unsigned int> solution;
    nQueens::crossbreedFunctor &breed = nQueens::variation::matched_crossover();

    std::string input = "";
    //input n;
    while(true)
    {
        std::cout << "Size of Chessboard (min 4) =";
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
    //input breed
    while(true)
    {
        std::cout << "Crosbreading Method  "    << std::endl;
        std::cout << "1: Matched Crossover "    << std::endl;
        std::cout << "2: Ordered Crossover = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> w)
        {
            if(w>0||w<3)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    //solve Methode
    while(true)
    {
        std::cout << "Which solving Method: "       << std::endl;
        std::cout << "1: Until Max Iteration;"      << std::endl;
        std::cout << "2: Until Time out (in min); " << std::endl;
        std::cout << "3: Until first solution; "    << std::endl;
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> p)
        {
            if(sol>0||sol<4)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    //summary of Inputs
    std::cout << "Size = "          << n    << std::endl;
    std::cout << "Induviduen = "    << x    << std::endl;
    std::cout << "Mutation = "      << p    << std::endl;
    std::cout << "Solve Methode = " << sol  << std::endl;

    //Init of Population
    nQueens::iterativeMaster master(n,x,p);
    if(w==1)
    {
        breed = nQueens::variation::matched_crossover;
    }
    else
    {
        breed = nQueens::variation::ordered_crossover;
    }

    //Which solve used
    switch(sol)
    {
        case 1:
        {
            while(true)
            {
                std::cout << "Max Iterations = ";
                getline(std::cin,input);
                std::stringstream myStream(input);
                if(myStream >> n)
                    break;
            }
            std::cout << "Invalid" << std::endl;
            //Solve wich max
            master.solve(n,breed);
        }
        break;
        case 2:
        {
            while(true)
            {
                std::cout << "How Long (minutes)= ";
                getline(std::cin,input);
                std::stringstream myStream(input);
                if(myStream >> t)
                    break;
            }
            std::cout << "Invalid" << std::endl;

            //Time calculation
            time_t now = time(NULL);
            now = now + t*60;
            //Solve with time
            master.solve(now,breed);
        }
        break;
        case 3:
        {
            //Solve until first solution
            master.solve(breed);
        }
        break;
        default:
        break;
    }


    return 0;
}
