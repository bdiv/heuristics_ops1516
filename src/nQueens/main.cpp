#include<iostream>
#include"include/population.h"
#include<stdlib.h>
#include<time.h>

using namespace std;

int main()
{

    //Festlegung der Parameter sp�ter als Benutzereingabe
    unsigned int    n = 5;      //Gr��e des Schachbretts
    unsigned int    x = 1;      //Wieviele Individuen pro Generation (Errechnen oder �ber Eingabe ?)
    unsigned int    i = 4;      //Maximale Anzahl der Iterationen
    double p = 0.2;             //Mutationswahscheinlichkeit
    srand(time(NULL));

    //erstellung der ersten Generation  mit population/void init()
    //2 M�glichkeiten:

    //1: Mit komplett zuf�lligen Individuen
    nQueens::population poli(n,x,p);

    //2: Da wollten wir doch was machen das es keine horizontalen �berschneidungen gibt ?
    //oder wahr das gleich die erste und einzige M�glichkeit

    // horizontale �berschneidungen gibt es nicht, wenn die population erstellt wird.
    // Sie k�nnte theoretisch auftreten, wenn man einen bl�den crossbreedFunctor designed.
    // die jetzigen sollten aber darauf achten
    // als letztes Notfallnetz haben wir immer die Score, die unterirdisch schlecht wird,
    // wenn wir zus�tzlich zu diagonalen auch horizontale �berschneidungen haben

    poli.printAll();

    return 0;
}
