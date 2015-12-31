#include<iostream>
#include"include/population.h"
#include<stdlib.h>
#include<time.h>

using namespace std;

int main()
{

    //Festlegung der Parameter später als Benutzereingabe
    unsigned int    n = 5;      //Größe des Schachbretts
    unsigned int    x = 1;      //Wieviele Individuen pro Generation (Errechnen oder über Eingabe ?)
    unsigned int    i = 4;      //Maximale Anzahl der Iterationen
    double p = 0.2;             //Mutationswahscheinlichkeit
    srand(time(NULL));

    //erstellung der ersten Generation  mit population/void init()
    //2 Möglichkeiten:

    //1: Mit komplett zufälligen Individuen
    nQueens::population poli(n,x,p);

    //2: Da wollten wir doch was machen das es keine horizontalen überschneidungen gibt ?
    //oder wahr das gleich die erste und einzige Möglichkeit

    // horizontale überschneidungen gibt es nicht, wenn die population erstellt wird.
    // Sie könnte theoretisch auftreten, wenn man einen blöden crossbreedFunctor designed.
    // die jetzigen sollten aber darauf achten
    // als letztes Notfallnetz haben wir immer die Score, die unterirdisch schlecht wird,
    // wenn wir zusätzlich zu diagonalen auch horizontale Überschneidungen haben

    poli.printAll();

    return 0;
}
