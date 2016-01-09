#include<iostream>
#include<stdlib.h>
#include<sstream>
#include<fstream>
#include<time.h>
#include"include/population.h"
#include"include/variation.h"
#include"include/crossbreedFunctor.h"
#include"include/iterativeMaster.h"

void print(std::vector<std::vector <unsigned int>> & solutions,time_t timeIt,unsigned int n)
{
    std::string input;
    unsigned int c = 0;                 //which choice
    unsigned int s = solutions.size();   //how many solutions

    //Choice if in console or File
    while(true)
    {
        std::cout << "Choose Output"    << std::endl;
        std::cout << "1: Console "      << std::endl;
        std::cout << "2: File (Same Folder) = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> c)
        {
            if(c>0 || c<3)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    if(c==1)
    {
        for(unsigned int x=0;x<s;x++)
        {
            for(unsigned int y=0;y<n;y++)
            {
                std::cout << solutions[x][y] << ',';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Found " << s << " Solutions in: " << timeIt/60 << " min." << std::endl;
    }
    else if(c==2)
    {
        std::fstream datei("solutions.txt",std::ios::out);
        datei << "Found " << s << "Solutions in : " << timeIt/60 << " min." << std::endl;
        for(unsigned int z=0;z<solutions.size();z++)
        {
            datei << "Solution Nr: " << z+1 << std::endl;
            for(unsigned int y=0;y<n;y++)
            {
                for(unsigned int x=0;x<n;x++)
                {
                    datei << "|";
                    if(solutions[z][y] == x)
                    {
                        datei << "x";
                    }
                    else
                    {
                        datei << "_";
                    }
                    datei << "|";
                }
                datei << "   " << solutions[z][y] << std::endl;
            }
            datei << "   " << std::endl;
        }
    }
}

int main()
{
    //Init of Parameters
    int             test=0;     //To test if positiv;
    unsigned int    n=0;        //Size of Chessboard;
    unsigned int    x=0;        //How many Individuals per Generation;
    double          p=0.0;      //Probability of mutation;
    unsigned int    sol=0;      //Which solve Methode
    unsigned int    i=0;        //Max Iteration;
    unsigned int    t=0;        //Time for Working;
    time_t          now;        //Time now for 3rd solve
    time_t          start;      //Time now for measure
    time_t          finish;     //Time after for measure
    time_t          timeIt;     //Time for how long it took
    unsigned int    w=1;        //Which crossbread;
    nQueens::crossbreedFunctor * breed; //Pointer for breed function

    std::string input = "";
    //input n;
    while(true)
    {
        std::cout << "Size of Chessboard (min 4) = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> test)
        {
            if(test>3)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    n = test;
    test = 0;

    //input x;
    while(true)
    {
        std::cout << "Number of Individuals " << std::endl;
        std::cout << "Tipp: Small Chessboard, many Individuals  = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> test)
        {
            if(test > 0)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    x = test;
    test = 0;

    //input p;
    double dtest = 0.0;
    while(true)
    {
        std::cout << "Probability for Mutation " << std::endl;
        std::cout << "Between 0-1. 0.1 = 10%  = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> dtest)
        {
            if(dtest>0 && dtest<1)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    p = dtest;
    dtest = 0;

    //input breed
    while(true)
    {
        std::cout << "Crosbreading Method  "    << std::endl;
        std::cout << "1: Matched Crossover "    << std::endl;http://www.cplusplus.com/forum/beginner/103051/#msg554829
        std::cout << "2: Ordered Crossover = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> test)
        {
            if(test>0||test<3)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    w = test;
    test = 0;

    //solve Methode
    while(true)
    {
        std::cout << "Which solving Method: "       << std::endl;
        std::cout << "1: Until Max Iteration;"      << std::endl;
        std::cout << "2: Until Time out (in min); " << std::endl;
        std::cout << "3: Until first solution = ";
        getline(std::cin,input);
        std::stringstream myStream(input);
        if(myStream >> test)
        {
            if(test>0||test<4)
            break;
        }
        std::cout << "Invalid" << std::endl;
    }
    sol = test;
    test = 0;

    //summary of Inputs
    std::cout << "Size = "          << n    << std::endl;
    std::cout << "Induviduen = "    << x    << std::endl;
    std::cout << "Mutation = "      << p    << std::endl;
    std::cout << "Breed Methode "   << w    << std::endl;
    std::cout << "Solve Methode = " << sol  << std::endl;

    //Init of Population
    nQueens::iterativeMaster master(n,x,p);

    if(w==1)
    {
        breed = new nQueens::variation::matched_crossover();
    }
    else
    {
        breed = new nQueens::variation::ordered_crossover();
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
                if(myStream >> i)
                    break;
                std::cout << "Invalid" << std::endl;
            }
            //Start of time measure
            start = time(NULL);
            //Solve wich max
            master.solve(i,(*breed));
            //Finish of time measure
            finish = time(NULL);
            timeIt = difftime(finish,start);
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
                std::cout << "Invalid" << std::endl;
            }
            //Time meassure
            start = time(NULL);
            //Time calculation
            now = time(NULL);
            now = now + t*60;
            //Solve with time
            master.solve(now,(*breed));
            //No time measure
            finish = time(NULL);
            timeIt = difftime(finish,start);
        }
        break;
        case 3:
        {
            //Start of time measure
            start = time(NULL);
            //Solve until first solution
            master.solve((*breed));
            //Finish time measure
            finish = time(NULL);
            timeIt = difftime(finish,start);
        }
        break;
        default:
        break;
    }

    //picking up solutions
    std::vector<std::vector<unsigned int>> solutions = master.getSolutions();

    //Testausgaben
    std::cout << "Time diff = " << timeIt/60 << "min"<< std::endl;
    std::cout << "size of solution = " << solutions.size() << std::endl;

    std::cout << "mit enter weiter ";
    getline(std::cin,input);

    //Call to print
    print(solutions,timeIt,n);

    return 0;
}
