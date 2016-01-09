#include <iostream>
using namespace std; 

class Ant{

public:

	int * path; //Abfolge der von der Ameise besuchten Knoten am aktuellen Pfad
	int path_hops; //Anzahl der von der Ameise besuchten Knoten am aktuellen Pfad
	double path_length; //Länge des bisher von der Ameise zurückgelegten Weges am aktuellen Pfad

	Ant();  
	~Ant(); 
	init(int dimension); //Initialisierung beim Aufruf der Ameise beim Suchen eines weiteren Weges vom Start zum Ziel (bereits besuchte Knoten 
	knoten_hinzufuegen(double knoten, double kantenlaenge); //Jedesmal, wenn die Ameise einen Knoten weitergeht, wird über diese Methode der nächste Knoten und die Kantenlänge vom ursprünglichen Knoten zu diesem Knoten übergeben und dann in der Ameisenklasse gespeichert.

}; 

Ant::Ant(){}


Ant::~Ant(){

	delete [] path; 
}

Ant::init(int dimension){

	path = new int [dimension]; 
	index = 0; 
	path_hops = 0; 
	path_length = 0; 

}

Ant::knoten_hinzufuegen(double knoten, double kantenlaenge){

	path_length += kantenlaenge; 
	path[++path_hops] = knoten; 	

}