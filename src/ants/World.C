#include <iostream>
#include "Ant.C"
using namespace std; 

class World{

public:

	int n; // Anzahl der Knoten
	double ** adjazenz; //  Adjazenzmatrix mit den Kantengewichten. Da die Kanten nicht gerichtet sind, ist die Matrix symmetrisch, d.h. a_ij = a_ji
	int start; //Nummer des Startknotens für das shortest Path Problem
	int ende; //Nummer des Endknotens für das shortest Path Problem. 
	int m; //Anzahl der Ameisen
	double ** pheromone; //Adjazenzmatrix mit den Pheromonleveln auf den einzelnen Kanten. Wie bei der Adjazenzmatrix mit den Kantengewichten (adjazenz), ist auch diese Matrix symmetrisch, also auch hier gilt: a_ij = a_ji
	int * shortest_path; //Abfolge der Knotenreihenfolge des bisher kürzesten Weges
	int shortest_path_hops; //Anzahl der Knoten von Start- bis Ziel
	double shortest_path_length; //Länge des bisher kürzesten Weges
	double alpha; //Einfluss der Pheromone bei der Wahl des nächsten Knotens
	double beta; //Einfluss von zusätzlichen Daten (z.B. Kantengewicht und erfassten Daten, ob eine Kante / ein Knoten bereits von der Ameise besucht wurde)
	double tau_0; //Initialer Pheromonwert auf den Kanten des Graphen
	double tau_min; //Minimaler Pheromonwert auf den Kanten des Graphen
	double tau_max; //Maximaler Pheromonwert auf den Kanten des Graphen
	double tau_neu_max; //
	double rho; //Geschwindigkeit, mit der Pheromone wieder verdampfen
	int iterations; //Iterationen, die mit den Ameisen bereits durchlaufen wurden. Wenn alle Ameisen einen Pfad zum Ziel gefunden haben, ist eine Iteration abgeschlossen. Erst nach 3 Iterationen werden die Pheromone berücksichtigt. 
	bool ** vi_edges; //Gibt an, ob Ameise schon auf Knoten war oder nicht; 
	int * nodes; //Liste mit allen schon besuchten Knoten

	World(); //Konstruktor
	~World(); //Offizieller Destruktor (ruft clear() auf)
	void clear(); //Eigentlicher Destruktor

	void sh_path(int n, double **adjazenz, int start, int ende); //Berechnung des shortest-paths

	double compute_coefficient(int i, int j); //Berechnung des Koeffizienten zur Entscheidung, in welche Richtung die Ameise weiter geht (wird in Funktion select_next_edge() für jede der ausgehenden Kanten aufgerufen)
	int select_next_edge(int letzerKnoten, int aktuellerKnoten); //Auswahl des nächsten Knotens anhand des aktuellen Knotens, an dem sich eine Ameise befindet und dem Knoten, von dem sie gekommen ist. Nach dem Aufruf von compute_coefficient für jede Kante (außer jender, von der die Ameise gekommen ist) wird entschieden, zu welchem Knoten die Ameise weitergeht.
						//Rückgabewert: Nächster Knoten
	void evaporate(); //Funktion, die die vorhandenen Pheromone auf den Kanten wieder verdunsten lässt. Wäre der Pheromonlevel nach dem Verdunsten unter tau_min (1), wird der Wert auf 1 gesetzt. Die Funktion wird vor update_pheromones ausgeführt.
 	void update_pheromones(); //Nach einer Iteration (alle Ameisen finden einen Weg vom Start bis zum Ziel) werden die verschiedenen Wege miteinander verglichen. Über die Variable aktueller_pfad, die in jeder Ameise gespeichert ist, ergibt sich die Fitness der einzelnen Wege. Für den kürzesten Weg werden am Meisten Pheromone vergeben
 	void print_sh_path(); //Ausgabe des kürzesten gefundenen Weges in der Konsole 



};


World::World(){

	alpha = 0.7; 
	beta  = 0.3; 
	tau_0 = 2;
	rho = 0.3; 
	m = 10; //Debugwert
	iterations = 0; 

}

World::~World(){

	clear();
}

void World::clear(){

	delete [] adjazenz; 
	delete [] pheromone; 
	delete [] vi_edges; 

}

void World::sh_path(int n, double ** adjazenz, int start, int ende)
{
	
	//**************************************************** ALLOZIEREN ********************************************************************
	
	adjazenz = new double* [n]; 
		for(int a = 0; a < n; ++a)
		{
			adjazenz [a] = new double [n];
		}

	pheromone = new double* [n]; 
		for(int a = 0; a < n; ++a)
		{
			pheromone [a] = new double [n];
		}


	vi_edges = new bool* [n]; 
		for(int a = 0; a < n; ++a)
		{
			vi_edges [a] = new bool [n]; 
		}


	//**************************************************** BERECHNEN ********************************************************************

	bool finished = false;
	bool abort = false; 
	bool Zielknotenerreicht = false; 
	Ant ameise [m];

	while (!finished && !abort)
	{
		iterations++; 

		for(int a = 0; a < m; a++)
		{
			ameise[a].init(n); 
			Zielknotenerreicht = false; 

			while(!Zielknotenerreicht && !abort)
			{
				//Speichern des nächsten Knotens in der Ameise:
				//ameisen[a].knoten_hinzufuegen(...)

				/*wenn die neue Kante / der neue
				Knoten bisher noch von keiner Ameise 
				zurück gelegt wurde --> abspeichern 
				der Kante in vi_edges bzw. 
				abspeichern des Knotens in vi_nodes */ 
			}
		}

		//Verdunsten der vorhandenen Pheromone
		evaporate(); 

		/*Updaten der Pheromone anhand der Gewichtung 
		der Wegstrecken, die die Ameisen zurückgelegt 
		haben. (Kürzeste Strecken bekommen am Meisten
		Pheromone)*/ 
		update_pheromones();

		//Kontrolle ob kürzester Pfad bereits gefunden wurde
		if(iterations > 3 ){

			double anteil_relativ = 0.8; 
			//Berechnen des absoluten Anteils anhand unserer Populationsgröße:
			int anteil_absolut = anteil_relativ * m;
			//Aufrunden (wir möchten mindestens anteil_relativ Ameisen haben)
			if (anteil_relativ * m - double(anteil_absolut) > 0)
				++anteil_absolut;

			int stop = m * (1 - anteil_relativ);
			if (m * (1 - anteil_relativ) - double(stop) > 0)
				++stop;

			for(int i = 0; i <= stop; i++)
			{
				int zaehler = 0;

				for (int j = i+1; j < m ; ++j)
				{
					//Beide Ameisen haben zumindest die selbe Weglänge zurückgelegt:
					if (ameise[i].path_hops == ameise[j].path_hops) 
					{
						bool gleich = true;
						for (int k = 0; k < ameise[i].path_hops && gleich; ++k)
						{
							if (ameise[i].path[k] != ameise[j].path[k])
								gleich = false;
						}

						//sind alle Knoten am Weg der Ameise gleich, zähle weiter
						if (gleich)
							++zaehler;
					}
				}

				//Haben xx% (Variable Anteil) der Ameisen den selben Weg gewählt,
				//setzen wir finished auf TRUE, was zum Ende der while-Schleife führt

				if (zaehler > anteil_absolut)
					finished = true;
			}

		}

	}

	//Ausgabe der Lösung in der Command Line:
	print_sh_path(); 



}

double World::compute_coefficient(int i, int j){


}

int World::select_next_edge(int letzerKnoten, int aktuellerKnoten){

}

void World::update_pheromones(){

}
void World::evaporate(){

	for(int a = 0; a < n; a++)
	{
		for(int b = 0; b < n; b++)
		{
			pheromone [a] [b] = pheromone [a] [b] * (1 - rho); 
			if(pheromone [a] [b] < tau_min) pheromone [a] [b] = tau_min;
			//if(pheromone [a] [b] > tau_max) pheromone [a] [b] = tau_max; 
		}
	}
}

void World::print_sh_path(){

	cout << "*** Shortest Path *** " << endl << endl; 


	for(int a = 0; a < shortest_path_length; a++)
	{
		cout << "Knoten [ " << a+1 << " ] = " << shortest_path[a] << endl;
	}
}


int main(){

	World * world = new World(); 


	delete world;
	return 0;
}