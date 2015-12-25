#include<iostream>
#include"population.h"

using namespace std;

int main()
{
    //Festlegung der Parameter später als Benutzereingabe
    unsigned int    n = 5;      //Größe des Schachbretts
    unsigned int    x = 4;      //Wieviele Individuen pro Generation (Errechnen oder über Eingabe ?)
    unsigned int    i = 4;      //Maximale Anzahl der Iterationen
    double p = 0.2;             //Mutationswahscheinlichkeit

    //erstellung der ersten Generation  mit population/void init()
    //2 Möglichkeiten:
    //1: Mit komplett zufälligen Individuen
    nQueens::population poli(n,x,p);
    //2: Da wollten wir doch was machen das es keine horizontalen überschneidungen gibt ?
    //oder wahr das gleich die erste und einzige Möglichkeit

    poli.printAll();

    return 0;
}
