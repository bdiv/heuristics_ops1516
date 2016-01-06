#include<iostream>
#include<stdlib.h>
#include<sstream>
#include<fstream>
#include<time.h>
#include"include/population.h"
#include"include/variation.h"
#include"include/crossbreedFunctor.h"
#include"include/iterativeMaster.h"

void print(std::vector<std::vector <unsigned int> > solution,time_t timeIt,unsigned int n)
{
    std::string input;
    unsigned int c = 0;                 //which choice
    unsigned int s = solution.size();   //how many solutions

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
            for(unsigned int y=0;y<n;)
            {
                std::cout << solution[x][y] << ',';
            }
        }
        std::cout << "Found " << s << "Solutions in: " << timeIt/60 << "." << std::endl;
    }
    else if(c==2)
    {
        std::fstream datei("solutions.txt",std::ios::out);
        for(unsigned int z=0;z<solution.size();z++)
        {
            datei << "Solution Nr: " << z+1 << std::endl;
            for(int y=0;y<n;y++)
            {
                for(int x=0;x<n;x++)
                {
                    datei << "|";
                    if(solution[z][y] == x)
                    {
                        datei << "x";
                    }
                    else
                    {
                        datei << "_";
                    }
                    datei << "|";
                }
                datei << "   " << solution[z][y] << std::endl;
            }
        }
    }
}

int main()
{
    // this works...
    nQueens::crossbreedFunctor * br = new nQueens::variation::ordered_crossover();
    nQueens::iterativeMaster mast(100,10,0.1);
    if(mast.getSolutions().size() == 0)
    {
        mast.solve((*br));
    }
    std::vector<std::vector<unsigned int>> solutions = mast.getPopulation().getIndividuals();
    for(int i = 0; i < solutions.size(); i++)
    {
      for(int y = 0; y < solutions[i].size(); y++)
      {
        std::cout << solutions[i][y] << ", ";
      }
      std::cout << std::endl;

    }

    return 0;
    /* this somehow doesnt work
    //Init of Parameters
    unsigned int    n=5;        //Size of Chessboard;
    unsigned int    x=10;        //How many Individuals per Generation;
    double          p=0.1;      //Probability of mutation;
    unsigned int    sol=3;      //Which solve Methode
    unsigned int    i=0;        //Max Iteration;
    unsigned int    t=0;        //Time for Working;
    time_t          now;        //Time now for 3rd solve
    time_t          start;      //Time now for measure
    time_t          finish;     //Time after for measure
    time_t          timeIt;     //Time for how long it took
    unsigned int    w=1;        //Which crossbread;
    nQueens::crossbreedFunctor * breed;     //Pointer for breed funktion

    srand(time(NULL));

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
        if(myStream >> sol)
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
    // master.getPopulation().printAll();
    //std::cout << "nach master erstellung " << std::endl;
    if(w==1)
    {
        breed = new nQueens::variation::matched_crossover();
        //std::cout << "Pointer erstellt " << std::endl;
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
            }
            //std::cout << "Invalid" << std::endl;
            //Start of time measure
            start = time(NULL);
            //Solve wich max
            master.solve(i,(*breed));
            //Finish of time measure
            finish = time(NULL);
            timeIt = difftime(start,finish);
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
            //std::cout << "Invalid" << std::endl;

            //Time calculation
            now = time(NULL);
            now = now + t*60;
            //Solve with time
            master.solve(now,(*breed));
            //No time measure
            timeIt = now;
        }
        break;
        case 3:
        {
            //std::cout << "switch case 3 " << std::endl;
            //Start of time measure
            start = time(NULL);
            //Solve until first solution
            master.solve((*breed));
            //std::cout << "nach solve " << std::endl;
            //Finish time measure
            finish = time(NULL);
            timeIt = difftime(start,finish);
        }
        break;
        default:
        break;
    }
    //picking up solutions
    std::vector<std::vector <unsigned int> > solution = master.getSolutions();

    //Testausgaben
    std::cout << "Time diff = " << timeIt << std::endl;
    std::cout << "size of solution = " << solution.size() << std::endl;

    int test = 0;
    std::cout << "mit enter weiter ";
    getline(std::cin,input);

    //Call to print
    print(solution,timeIt,n);

    return 0; */
}
