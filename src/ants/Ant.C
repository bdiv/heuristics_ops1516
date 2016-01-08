#include <iostream>
using namespace std; 

class Ant{

public:

	double * aktueller_pfad; //Abfolge der von der Ameise besuchten Knoten am aktuellen Pfad
	int index; //Index für aktueller_pfad
	double aktuelle_anzahl_besuchter_knoten; //Anzahl der von der Ameise besuchten Knoten am aktuellen Pfad
	double shortest_path_length; //Länge des bisher von der Ameise zurückgelegten Weges am aktuellen Pfad

	Ant();  
	~Ant(); 
	init(int dimension); //Initialisierung beim Aufruf der Ameise beim Suchen eines weiteren Weges vom Start zum Ziel (bereits besuchte Knoten 
	knoten_hinzufuegen(double knoten, double kantenlaenge); //Jedesmal, wenn die Ameise einen Knoten weitergeht, wird über diese Methode der nächste Knoten und die Kantenlänge vom ursprünglichen Knoten zu diesem Knoten übergeben und dann in der Ameisenklasse gespeichert.

}; 

Ant::Ant(){}


Ant::~Ant(){

	delete [] aktueller_pfad; 
}

Ant::init(int dimension){

	aktueller_pfad = new double [dimension]; 
	index = 0; 
	aktuelle_anzahl_besuchter_knoten = 0; 
	shortest_path_length = 0; 

}

Ant::knoten_hinzufuegen(double knoten, double kantenlaenge){

	shortest_path_length += kantenlaenge; 
	aktueller_pfad[index++] = knoten; 
	

}