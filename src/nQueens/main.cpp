#include<iostream>
#include"population.h"

using namespace std;

int main()
{
    //Festlegung der Parameter sp�ter als Benutzereingabe
    unsigned int    n = 5;      //Gr��e des Schachbretts
    unsigned int    x = 4;      //Wieviele Individuen pro Generation (Errechnen oder �ber Eingabe ?)
    unsigned int    i = 4;      //Maximale Anzahl der Iterationen
    double p = 0.2;             //Mutationswahscheinlichkeit

    //erstellung der ersten Generation  mit population/void init()
    //2 M�glichkeiten:
    //1: Mit komplett zuf�lligen Individuen
    nQueens::population poli(n,x,p);
    //2: Da wollten wir doch was machen das es keine horizontalen �berschneidungen gibt ?
    //oder wahr das gleich die erste und einzige M�glichkeit

    poli.printAll();

    return 0;
}
