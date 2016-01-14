#include <iostream>
using namespace std; 

class Ant{

public:

	int * path; //Abfolge der von der Ameise besuchten Knoten am aktuellen Pfad
	int path_hops; //Anzahl der von der Ameise besuchten Knoten am aktuellen Pfad
	double path_length; //Länge des bisher von der Ameise zurückgelegten Weges am aktuellen Pfad

	inline Ant(); 
	inline ~Ant();

	void inline init(int dimension); //Initialisierung beim Aufruf der Ameise beim Suchen eines weiteren Weges vom Start zum Ziel (bereits besuchte Knoten 
	void inline knoten_hinzufuegen(int knoten, double kantenlaenge); //Jedesmal, wenn die Ameise einen Knoten weitergeht, wird über diese Methode der nächste Knoten und die Kantenlänge vom ursprünglichen Knoten zu diesem Knoten übergeben und dann in der Ameisenklasse gespeichert.

}; 


inline Ant::Ant() {}

inline Ant::~Ant() {
	delete[] path;
}

void inline Ant::init(int dimension){

	path = new int [dimension]; 
	path_hops = 0; 
	path_length = 0; 

}

void inline Ant::knoten_hinzufuegen(int knoten, double kantenlaenge){

	path_length += kantenlaenge; 
	path[++path_hops] = knoten; 	

}