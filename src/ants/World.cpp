#include <iostream>
#include <stdlib.h>     
#include <time.h> 
#include <math.h>
#include "Ant.cpp"
#include "ReadInTXT.cpp"
using namespace std;

class World {

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
	double tau_neu_max; //Maximaler additiver Pheromonwert auf den Kanten des Graphen (pro Iteration)
	double rho; //Geschwindigkeit, mit der Pheromone wieder verdampfen
	int iterations; //Iterationen, die mit den Ameisen bereits durchlaufen wurden. Wenn alle Ameisen einen Pfad zum Ziel gefunden haben, ist eine Iteration abgeschlossen. Erst nach 3 Iterationen werden die Pheromone berücksichtigt. 
	bool ** vi_edges; //Gibt an, ob Ameise schon auf Knoten war oder nicht
	bool * vi_nodes; //Liste mit allen schon besuchten Knoten
	Ant *ameise;

	World(); //Konstruktor
	~World(); //Offizieller Destruktor (ruft clear() auf)
	void clear(); //Eigentlicher Destruktor

	void sh_path(int n, double **adjazenz, int start, int ende); //Berechnung des shortest-paths

	double compute_coefficient(int i, int j); //Berechnung des Koeffizienten zur Entscheidung, in welche Richtung die Ameise weiter geht (wird in Funktion select_next_edge() für jede der ausgehenden Kanten aufgerufen)
	int select_next_edge(int a); //Auswahl des nächsten Knotens anhand des aktuellen Knotens, an dem sich eine Ameise befindet und dem Knoten, von dem sie gekommen ist. Nach dem Aufruf von compute_coefficient für jede Kante (außer jender, von der die Ameise gekommen ist) wird entschieden, zu welchem Knoten die Ameise weitergeht.
								 //Rückgabewert: Nächster Knoten
	void evaporate(); //Funktion, die die vorhandenen Pheromone auf den Kanten wieder verdunsten lässt. Wäre der Pheromonlevel nach dem Verdunsten unter tau_min (1), wird der Wert auf 1 gesetzt. Die Funktion wird vor update_pheromones ausgeführt.
	void update_pheromones(); //Nach einer Iteration (alle Ameisen finden einen Weg vom Start bis zum Ziel) werden die verschiedenen Wege miteinander verglichen. Über die Variable aktueller_pfad, die in jeder Ameise gespeichert ist, ergibt sich die Fitness der einzelnen Wege. Für den kürzesten Weg werden am Meisten Pheromone vergeben
	void print_sh_path(); //Ausgabe des kürzesten gefundenen Weges in der Konsole 
	int randomize(); //Zufallsgenerator
};


World::World() {

	alpha = 0.3;
	beta = 0.7;
	tau_0 = 0;
	tau_neu_max = 1;
	rho = 0.3;
	m = 2; //Debugwert
	ameise = new Ant[m];
	iterations = 0;

}

World::~World() {

	clear();
}

void World::clear() {

	delete[] adjazenz;
	delete[] pheromone;
	delete[] vi_edges;

}

void World::sh_path(int n, double ** adjazenz, int start, int ende)
{
	//Abspeichern der Übergabeparameter
	this->n = n;
	this->start = start;
	this->ende = ende;
	this->adjazenz = adjazenz;

	//**************************************************** ALLOZIEREN ********************************************************************

	pheromone = new double*[n];
	for (int i = 0; i < n; ++i)
	{
		pheromone[i] = new double[n];
		for (int j = 0; j < n; ++j)
			pheromone[i][j] = tau_0;
	}


	vi_edges = new bool*[n];
	for (int i = 0; i < n; ++i)
	{
		vi_edges[i] = new bool[n];
		for (int j = 0; j < n; ++j)
			vi_edges[i][j] = false;
	}

	vi_nodes = new bool[n];
	for (int i = 0; i < n; ++i)
		vi_nodes[i] = false;

	shortest_path = new int[n];

	//**************************************************** BERECHNEN ********************************************************************

	bool finished = false;
	bool abort = false;
	bool Zielknotenerreicht = false;


	while (!finished && !abort)
	{
		for (int a = 0; a < m; a++)
		{
			//Ameise initialisieren
			cout << "Ameise " << a + 1 << ", Iteration " << iterations << ":\n";
			ameise[a].init(n);
			Zielknotenerreicht = false;

			//die aktuelle Position der Ameise auf den Startknoten stellen
			int aktueller_Knoten = start;

			//hinzufügen des Startknotens zu den besuchten Knoten der Ameise:
			ameise[a].path[0] = start;

			while (!Zielknotenerreicht && !abort)
			{
				int naechster_knoten = select_next_edge(a);
				if (naechster_knoten == 0) {
					cout << "Ameise kann nicht mehr weitergehen --> Neuinitialisierung.\n\n";
					ameise[a].init(n);
					aktueller_Knoten = start;
					ameise[a].path[0] = start;
				}
				else {
					cout << "Stehe in Knoten " << aktueller_Knoten << " und gehe jetzt zu Knoten " << naechster_knoten << " weiter.\n\n";
					double kantenlaenge = adjazenz[aktueller_Knoten - 1][naechster_knoten - 1];

					ameise[a].knoten_hinzufuegen(naechster_knoten, kantenlaenge);

					//updaten von vi_edges:
					if (vi_edges[aktueller_Knoten - 1][naechster_knoten - 1] == false)
					{
						vi_edges[aktueller_Knoten - 1][naechster_knoten - 1] = true;
						vi_edges[naechster_knoten - 1][aktueller_Knoten - 1] = true;
					}

					//updaten von vi_nodes:
					if (vi_nodes[naechster_knoten - 1] == false)
						vi_nodes[naechster_knoten - 1] = true;

					//updaten von aktuellem Knoten:
					aktueller_Knoten = naechster_knoten;

					//prüfen, ob die Ameise den Zielknoten erreicht hat:
					if (aktueller_Knoten == ende) {
						Zielknotenerreicht = true;
						cout << "Ameise " << a + 1 << " hat den Zielknoten erreicht! :) \n\n";
					}
				}
			}
		}

		//Verdunsten der vorhandenen Pheromone
		evaporate();

		//Updaten der Pheromone anhand der Gewichtung der Wegstrecken, die die Ameisen 
		//zurückgelegt haben. (Kürzeste Strecken bekommen am Meisten Pheromone)
		update_pheromones();

		//Kontrolle ob kürzester Pfad bereits gefunden wurde

		if (iterations > 3) {

			double anteil_relativ = 0.8;
			//Berechnen des absoluten Anteils anhand unserer Populationsgröße:
			int anteil_absolut = anteil_relativ * m;
			//Aufrunden (wir möchten mindestens anteil_relativ Ameisen haben)
			if (anteil_relativ * m - double(anteil_absolut) > 0)
				++anteil_absolut;

			int stop = m * (1 - anteil_relativ);
			if (m * (1 - anteil_relativ) - double(stop) > 0)
				++stop;

			for (int i = 0; i <= stop; i++)
			{
				int zaehler = 0;

				for (int j = i + 1; j < m; ++j)
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

		//Weitere Iteration abgeschlossen
		++iterations;
		cout << iterations << ". Iteration abgeschlossen! :D \n";
		cout << "Ausgabe der updadateten Pheromonlevel:\n";
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				cout << pheromone[i][j] << "	";
			}
			cout << "\n";
		}
		cout << "----------------------------------------\n\n\n";
		cout << "Weiterfuehren bestaetigen (0 = nein, alles andere = ja)";
		int i;
		cin >> i;
		if (i == 0)
			finished = true;
	}

	//Ausgabe der Lösung in der Command Line:
	print_sh_path();

}

double World::compute_coefficient(int i, int j) {

	i--; //Knoten auf Index aktualisieren
	j--; //Knoten auf Index aktualisieren

	if (pheromone[i][j] == 0)
	{
		cout << "Berechne Alternativen Pheromonlevel...\n";

		if (vi_nodes[j] == false && vi_edges[i][j] == false)
		{
			return pow((1.0 / adjazenz[i][j]), alpha) * pow((1.0 + beta), 2.0);
		}
		if (vi_nodes[j] == true && vi_edges[i][j] == false)
		{
			return pow((1.0 / adjazenz[i][j]), alpha) * (1.0 + beta);
		}
		if (vi_nodes[j] == false && vi_edges[i][j] == true)
		{
			return pow((1.0 / adjazenz[i][j]), alpha) * (1.0 + beta);
		}
		else return pow((1.0 / adjazenz[i][j]), alpha);
	}
	else
	{
		if (iterations < 3)
		{
			return 0;
		}

		else
		{
			cout << "Berechne Koeffizient anhand des Pheromonlevels. Pheromone von Knoten " << i << " in Richtung Knoten " << j << " betragen: " << pheromone[i - 1][j - 1] << ".\n";

			if (vi_nodes[j] == false && vi_edges[i][j] == false)
			{
				return pow(pheromone[i][j], alpha) * pow((1.0 + beta), 2.0);
			}
			if (vi_nodes[j] == true && vi_edges[i][j] == false)
			{
				return pow(pheromone[i][j], alpha) * (1.0 + beta);
			}
			if (vi_nodes[j] == false && vi_edges[i][j] == true)
			{
				return pow(pheromone[i][j], alpha) * (1.0 + beta);
			}
			else return pow(pheromone[i][j], alpha);
		}
	}
}

int World::select_next_edge(int a) {

	//Auslesen des aktuellen Knotens der Ameise:
	int aktueller_Knoten = ameise[a].path[ameise[a].path_hops];

	//erstellen einer Liste mit allen Knoten, zu denen die Ameise
	//vom aktuellen Knoten aus gehen kann:

	//Zeile der Adjazenzmatrix auslesen:
	double* zeile = adjazenz[aktueller_Knoten - 1];

	//Checken, an welchen Positionen ausgehende Kanten sind
	//und prüfen, ob noch nicht besuchte Knoten noch vorhanden sind:

	int naechster_knoten = 0;
	double best = -1.0;

	bool entscheide_zufaellig = false;
	int *moegliche_knoten = new int[n];
	int index = 0;

	for (int i = 0; i < n; ++i) {
		//prüfen, ob Grundsätzlich eine Kante vorhanden ist:
		if (zeile[i] > 0) {
			//prüfen, ob die Ameise bereits auf diesem Knoten war:
			bool bereits_besucht = false;
			for (int j = 0; j < ameise[a].path_hops; ++j) {
				// (i + 1) --> weil die Knotennummern bei 1 zu zählen beginnen
				if (ameise[a].path[j] == (i + 1))
					bereits_besucht = true;
			}
			//wenn die Ameise noch nicht auf dem Knoten war --> berechnen 
			//des Kantengewichts mit Hilfe von compute_coefficient:
			if (!bereits_besucht) {
				double current = compute_coefficient(aktueller_Knoten, (i + 1));
				cout << "Pruefe Kante in Richtung Knoten " << i + 1 << ". Berechneter Koeffizient: " << current << "\n";
				//prüfen, ob der aktuelle coefficient besser ist, als der bisher ermittelte:
				if (current > best) {
					best = current;
					naechster_knoten = i + 1;
					if (entscheide_zufaellig) {
						entscheide_zufaellig = false;
					}
				}
				//wenn der aktuelle Wert gleich dem Besten bisher ermitteltem Wert ist:
				//speichere den Knoten in eine Liste und entscheide später 
				else if (current == best) {
					if (!entscheide_zufaellig) {
						moegliche_knoten[0] = naechster_knoten;
						index = 0;
						entscheide_zufaellig = true;
					}
					moegliche_knoten[++index] = i + 1;
				}
			}
		}
	}

	//ist eine Entscheidung zwischen mehreren möglichen Knoten zu treffen,
	//entscheide zufällig:
	if (entscheide_zufaellig) {
		cout << "Muss Zufaellig entscheiden...";
		//Generiere Zufallszahl:
		srand(time(NULL));
		int random = randomize();
		cout << "Zufallswert = " << random << "\n";
		index = double(random / 10000.0) * double(index + 1);
		naechster_knoten = moegliche_knoten[index];
		cout << " und habe mich fuer Knoten " << naechster_knoten << " entschieden.\n";

	}

	delete[] moegliche_knoten;

	return naechster_knoten;

}

void World::update_pheromones() {

	if (iterations == 0)
	{
		//Blinde Annahme dass 1. Ameise kürzeste Wegstrecke hat
		shortest_path_length = ameise[0].path_length;
		copy(ameise[0].path, ameise[0].path + n, shortest_path);
		shortest_path_hops = ameise[0].path_hops;
	}

	for (int a = 0; a < m; a++)
	{
		//1. Ameise in 1. Iteration hat bereits zuweisung
		if (iterations == 0 && a == 0) continue;
		//Finde kürzeste Wegstrecke aller Ameisen
		if (ameise[a].path_length < shortest_path_length) {
			shortest_path_length = ameise[a].path_length;
			copy(ameise[a].path, ameise[a].path + n, shortest_path);
			shortest_path_hops = ameise[a].path_hops;
		}
	}

	for (int a = 0; a < m; a++)
	{
		for (int b = 0; b < ameise[a].path_hops; b++)
		{
			int i = ameise[a].path[b];
			int j = ameise[a].path[b + 1];
			//Lege Pheromone
			double additiver_pheromonlevel = (double(shortest_path_length) / double(ameise[a].path_length)) * tau_neu_max;
			cout << "shortest_path_length = " << shortest_path_length << "\n";
			cout << "path_length von Ameise " << a + 1 << " betraegt: " << ameise[a].path_length << "\n";
			cout << "Der additive Pheromonlevel fuer Kante " << i << "-" << j << " betragegt: " << additiver_pheromonlevel << "\n";
			pheromone[i - 1][j - 1] += additiver_pheromonlevel;
			pheromone[j - 1][i - 1] += additiver_pheromonlevel;
		}
	}

}
void World::evaporate() {

	for (int a = 0; a < n; a++)
	{
		for (int b = 0; b < n; b++)
		{
			pheromone[a][b] = pheromone[a][b] * (1 - rho);
			if (pheromone[a][b] < tau_min) pheromone[a][b] = tau_min;
		}
	}
}

void World::print_sh_path() {

	cout << "*** Shortest Path *** " << endl << endl;


	for (int a = 0; a < shortest_path_length; a++)
	{
		cout << "Knoten [ " << a + 1 << " ] = " << shortest_path[a] << endl;
	}
}

int World::randomize()
{
	bool help = false;
	return rand() % 10000 + !help;
}


int main() {
	cout << "Hello World!";

	Matrix read;
		
	double **adjazenz = read.getMatrix();
	
	World world;

	world.sh_path(5, adjazenz, 2, 5);

	return 0;
}