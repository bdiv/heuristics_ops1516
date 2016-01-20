#include <iostream>
#include <stdlib.h>     
#include <time.h>
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
	bool give_outputs;
	bool stop_between_iterations;

	World(int n); //Konstruktor
	~World(); //Offizieller Destruktor (ruft clear() auf)
	void clear(); //Eigentlicher Destruktor

	bool sh_path(int n, double ** adjazenz, int start, int ende,
		double alpha, double beta, double rho, int m, bool give_outputs, 
		bool stop_between_iterations); //Berechnung des shortest-paths

	double compute_coefficient(int i, int j); //Berechnung des Koeffizienten zur Entscheidung, in welche Richtung die Ameise weiter geht (wird in Funktion select_next_edge() für jede der ausgehenden Kanten aufgerufen)
	int select_next_edge(int a); //Auswahl des nächsten Knotens anhand des aktuellen Knotens, an dem sich eine Ameise befindet und dem Knoten, von dem sie gekommen ist. Nach dem Aufruf von compute_coefficient für jede Kante (außer jender, von der die Ameise gekommen ist) wird entschieden, zu welchem Knoten die Ameise weitergeht.
								 //Rückgabewert: Nächster Knoten
	void evaporate(); //Funktion, die die vorhandenen Pheromone auf den Kanten wieder verdunsten lässt. Wäre der Pheromonlevel nach dem Verdunsten unter tau_min (1), wird der Wert auf 1 gesetzt. Die Funktion wird vor update_pheromones ausgeführt.
	void update_pheromones(); //Nach einer Iteration (alle Ameisen finden einen Weg vom Start bis zum Ziel) werden die verschiedenen Wege miteinander verglichen. Über die Variable aktueller_pfad, die in jeder Ameise gespeichert ist, ergibt sich die Fitness der einzelnen Wege. Für den kürzesten Weg werden am Meisten Pheromone vergeben
	void print_sh_path(); //Ausgabe des kürzesten gefundenen Weges in der Konsole 
	int randomize(); //Zufallsgenerator
	unsigned long World::mix(unsigned long a, unsigned long b, unsigned long c); //Funktion zum Erzeugen des Randomwerts
};


World::World(int n) {

	//Initialisierung mit Defaultwerten
	alpha = 0.3;
	beta = 0.7;
	tau_0 = 0.0;
	tau_neu_max = 1;
	rho = 0.3;
	m = 5;

	this->n = n;

	pheromone = new double*[n];
	for (int i = 0; i < n; ++i)
		pheromone[i] = new double[n];

	vi_edges = new bool*[n];
	for (int i = 0; i < n; ++i)
		vi_edges[i] = new bool[n];

	vi_nodes = new bool[n];

	shortest_path = new int[n];
}

World::~World() {

	clear();
}

void World::clear() {

	delete[] adjazenz;
	delete[] pheromone;
	delete[] vi_edges;
	delete[] vi_nodes;
	delete[] ameise;
}

bool World::sh_path(int n, double ** adjazenz, int start, int ende,		
	double alpha, double beta, double rho, int m, bool give_outputs, 
	bool stop_between_iterations) {
	//Abspeichern der Übergabeparameter
	//n wird nicht mehr benötigt, da es schon im Konstruktor übergeben wird.
	//this->n = n;
	this->start = start;
	this->ende = ende;
	this->adjazenz = adjazenz;
	this->alpha = alpha;
	this->beta = beta;
	this->rho = rho;
	this->m = m;
	this->give_outputs = give_outputs;
	this->stop_between_iterations = stop_between_iterations;

	//Initialisierung
	ameise = new Ant[m];
	iterations = 0;
	
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			pheromone[i][j] = tau_0;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			vi_edges[i][j] = false;

	for (int i = 0; i < n; ++i)
		vi_nodes[i] = false;

	bool finished = false;
	bool Zielknotenerreicht = false;
	char abfrage;

	while (!finished)
	{
		for (int a = 0; a < m; a++)
		{
			//Ameise initialisieren
			if (give_outputs)
				cout << "Ameise " << a + 1 << ", Iteration " << iterations << ":\n";
			ameise[a].init(n);
			Zielknotenerreicht = false;

			//Zaehler für Neuinitialisierungen:
			int neuinitialisierungen = 0;

			//die aktuelle Position der Ameise auf den Startknoten stellen
			int aktueller_Knoten = start;

			//hinzufügen des Startknotens zu den besuchten Knoten der Ameise:
			ameise[a].path[0] = start;

			while (!Zielknotenerreicht)
			{
				int naechster_knoten = select_next_edge(a);

				if (naechster_knoten == 0) {
					if (give_outputs)
						cout << "Ameise kann nicht mehr weitergehen (nur noch bereits besuchte Knoten stehen zur Auswahl) --> Neuinitialisierung.\n\n";
					++neuinitialisierungen;

					//Wurde bereits mehrere Male Neuinitialisiert --> breche ab
					if (neuinitialisierungen == 100)
						return false;
					ameise[a].init(n);
					aktueller_Knoten = start;
					ameise[a].path[0] = start;
				}
				else {
					if (give_outputs)
						cout << "Stehe in Knoten " << aktueller_Knoten << " und gehe jetzt zu Knoten ---> " << naechster_knoten << " <--- weiter.\n\n";
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
						if (give_outputs) {
							cout << "Ameise " << a + 1 << " hat den Zielknoten erreicht! :) \n";
							cout << "Die zurueckgelegte Wegstrecke war " << ameise[a].path_length << "\n\n\n";
						}
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

			for (int i = 0; i <= stop && !finished; i++)
			{
				int zaehler = 1;

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
				if (zaehler >= anteil_absolut) {
					finished = true;
					if (give_outputs)
						cout << "Abbruchbedingung erreicht. " << int(double(zaehler/m) * 100) << "% der Ameisen gehen den selben Weg.\n\n";
				}
			}
		}		

		//Weitere Iteration abgeschlossen
		++iterations;

		if (!finished) {
			if (give_outputs) {
				cout << iterations << ". Iteration abgeschlossen! :D \n";
				cout << "Ausgabe der updadateten Pheromonlevel:\n";
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						cout << pheromone[i][j] << "	";
					}
					cout << "\n";
				}
				cout << "----------------------------------------\n\n\n";
				if (stop_between_iterations) {
					cout << "Weiterfuehren bestaetigen (beliebiges Zeichen eingeben) ";
					cin >> abfrage;
				}
			}
		}
	}
	return true;
}

double World::compute_coefficient(int i, int j) {

	i--; //Knoten auf Index aktualisieren
	j--; //Knoten auf Index aktualisieren

	if (pheromone[i][j] == 0)
	{
		if (give_outputs)
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
			if (give_outputs)
				cout << "Berechne Koeffizient anhand des Pheromonlevels. Pheromone betragen: " << pheromone[i][j] << ".\n";

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
				if (give_outputs)
					cout << "Pruefe Kante in Richtung Knoten " << i + 1 << "\n";
				double current = compute_coefficient(aktueller_Knoten, (i + 1));
				if (give_outputs)
					cout << "Berechneter Koeffizient: " << current << "\n";
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
		if (give_outputs)
			cout << "Muss Zufaellig entscheiden...";
		//Generiere Zufallszahl:
		srand(time(NULL));
		int random = randomize();
		index = double(random / 10000.0) * double(index + 1);
		naechster_knoten = moegliche_knoten[index];
		if (give_outputs)
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
		if (ameise[a].path_length < shortest_path_length 
			|| (ameise[a].path_length == shortest_path_length && shortest_path_hops < ameise[a].path_hops)) {
			shortest_path_length = ameise[a].path_length;
			copy(ameise[a].path, ameise[a].path + n, shortest_path);
			shortest_path_hops = ameise[a].path_hops;
		}
	}
	if (give_outputs)
		cout << "shortest_path_length = " << shortest_path_length << "\n";
	for (int a = 0; a < m; a++)	{

		//Lege Pheromone
		double additiver_pheromonlevel = (double(shortest_path_length) / double(ameise[a].path_length)) * tau_neu_max;
		if (give_outputs) {
			cout << "path_length von Ameise " << a + 1 << " betraegt: " << ameise[a].path_length << "\n";
			cout << "Der additive Pheromonlevel betragegt: " << additiver_pheromonlevel << "\n";
		}

		for (int b = 0; b < ameise[a].path_hops; b++)
		{
			int i = ameise[a].path[b];
			int j = ameise[a].path[b + 1];
		
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

	cout << "Zurueckgelegte Laenge: " << shortest_path_length << "\n\n";

	cout << "Abfolge der Knoten (inklusive Start- und Endknoten):\n";
	for (int a = 0; a <= shortest_path_hops; a++)
	{
		cout << "Knoten [" << a + 1 << "] = " << shortest_path[a] << endl;
	}
	cout << "\n\n";
}

int World::randomize()
{
	//srand(time(NULL));
	//return rand() % 10000;
	unsigned long seed = mix(clock(), time(NULL), 1147483647);
	seed = seed % 10000;
	if (seed < 0)
		seed = seed * -1;
	return int(seed);
}

unsigned long World::mix(unsigned long a, unsigned long b, unsigned long c)
{
	a = a - b;  a = a - c;  a = a ^ (c >> 13);
	b = b - c;  b = b - a;  b = b ^ (a << 8);
	c = c - a;  c = c - b;  c = c ^ (b >> 13);
	a = a - b;  a = a - c;  a = a ^ (c >> 12);
	b = b - c;  b = b - a;  b = b ^ (a << 16);
	c = c - a;  c = c - b;  c = c ^ (b >> 5);
	a = a - b;  a = a - c;  a = a ^ (c >> 3);
	b = b - c;  b = b - a;  b = b ^ (a << 10);
	c = c - a;  c = c - b;  c = c ^ (b >> 15);
	return c;
}


int main() {
	Matrix read;

	double **adjazenz = read.getMatrix();
	int dimension = read.getDimension();
	cout << "Dimension = " << dimension << "\n";
	
	World world(dimension);

	cout << "Wilkommen beim Shortest Path Problem Solver durch den Ameisenalgorithmus!\n";
	cout << "-------------------------------------------------------------------------\n\n";

	bool programm_laufen_lassen = true;
	
	while (programm_laufen_lassen) {
		int start = 0;
		cout << "Startknoten:\n";
		cout << "-------------------\n";
		cout << "Bitte den Startknoten eingeben (erster Knoten = 1).\n";
		while (start <= 0 || start > dimension) {
			cout << "Eingabe: ";
			if (!(cin >> start) || start == 0) {
				cout << "\nFalsche Eingabe!\n";
				cin.clear();
				while (cin.get() != '\n');
			}
			else if (start <= 0 || start > dimension)
				cout << "\nEingegebener Wert liegt nicht im Wertebereich  [1," << dimension << "] !\n";
		}
		cout << "\n\n";

		int stop = 0;
		cout << "Zielknoten:\n";
		cout << "-------------------\n";
		cout << "Bitte den Zielknoten eingeben (erster Knoten = 1).\n";
		while (stop <= 0 || stop > dimension) {
			cout << "Eingabe: ";
			if (!(cin >> stop) || stop == 0) {
				cout << "\nFalsche Eingabe!\n";
				cin.clear();
				while (cin.get() != '\n');
			}
			else if (stop <= 0 || stop > dimension)
				cout << "\nEingegebener Wert liegt nicht im Wertebereich  [1," << dimension << "] !\n";
		}
		cout << "\n\n";

		bool exit = false;

		while (!exit) {
			cout << "Im folgenden koennen die Parameter definiert werden:\n\n";
			double alpha = 0.0;
			cout << "Alpha:\n";
			cout << "------\n";
			cout << "Alpha ist der Einfluss der Pheromone bei der Wahl des naechsten Knotens "
				<< "und liegt im Bereich ]0,1[. Defaultwert ist 0.3\n";
			while (alpha <= 0 || alpha >= 1) {
				cout << "Eingabe von Alpha: ";
				if (!(cin >> alpha) || alpha == 0) {
					cout << "\nFalsche Eingabe!\n";
					cin.clear();
					while (cin.get() != '\n');
				}
				else if (alpha <= 0 || alpha >= 1)
					cout << "\nEingegebener Wert liegt nicht im Wertebereich ]0,1[ !\n";
			}
			cout << "\n\n";

			double beta = 0.0;
			cout << "Beta:\n";
			cout << "------\n";
			cout << "Beta ist der Einfluss von zusaetzlichen Daten (z.B. Kantengewicht und erfassten "
				<< "Daten, ob eine Kante / ein Knoten bereits von der Ameise besucht wurde) "
				<< "und liegt ebenfalls im Bereich ]0,1[. Defaultwert ist 0.7\n";
			while (beta <= 0 || beta >= 1) {
				cout << "Eingabe von Beta: ";
				if (!(cin >> beta) || beta == 0) {
					cout << "\nFalsche Eingabe!\n";
					cin.clear();
					while (cin.get() != '\n');
				}
				else if (beta <= 0 || beta >= 1)
					cout << "\nEingegebener Wert liegt nicht im Wertebereich ]0,1[ !\n";
			}
			cout << "\n\n";

			double rho = 0.0;
			cout << "Rho:\n";
			cout << "----\n";
			cout << "Rho ist Geschwindigkeit, mit der Pheromone nach jeder Iteration wieder verdampfen.\n";
			cout << "Rho liegt ebenfalls im Bereich ]0,1[. 0 wuerde bedeuten, dass keine Pheromone verdampfen "
				<< "wuerden, 1 wuerde bedeuten, dass alle Pheromone nach jeder Iteration verdampfen.\n";
			cout << "Defaultwert ist 0.3\n";
			while (rho <= 0 || rho >= 1) {
				cout << "Eingabe von Rho: ";
				if (!(cin >> rho) || rho == 0) {
					cout << "\nFalsche Eingabe!\n";
					cin.clear();
					while (cin.get() != '\n');
				}
				else if (rho <= 0 || rho >= 1)
					cout << "\nEingegebener Wert liegt nicht im Wertebereich ]0,1[ !\n";
			}
			cout << "\n\n";

			int m_max = 0;
			cout << "Anzahl der Ameisen:\n";
			cout << "-------------------\n";
			cout << "Die Anzahl der Ameisen beeinflusst nachhaltig die Qualität des Ergebnisses.\n";
			cout << "Wertebereich: >= 1, Defaultwert ist 5.\n";
			while (m_max < 1) {
				cout << "Eingabe: ";
				if (!(cin >> m_max) || m_max == 0) {
					cout << "\nFalsche Eingabe!\n";
					cin.clear();
					while (cin.get() != '\n');
				}
				else if (m_max <= 1)
					cout << "\nEingegebener Wert liegt nicht im Wertebereich >= 1 !\n";
			}
			cout << "\n\n";

			//Globales bestes Ergebnis:
			int * overall_shortest_path = new int[dimension]; //Abfolge der Knotenreihenfolge des bisher kürzesten Weges
			int overall_shortest_path_hops; //Anzahl der Knoten von Start- bis Ziel
			double overall_shortest_path_length; //Länge des bisher kürzesten Weges

			bool wrong_parameters = false;

			for (int i = 1; i <= m_max; ++i) {
				int retry = 0;
				while (!world.sh_path(dimension, adjazenz, start, stop, alpha, beta, rho, i, false, false) && !wrong_parameters) {
					++retry;
					if (retry > 100) {
						wrong_parameters = true;
						cout << "Leider konvergiert der Ameisenalgorithmus nicht. Das kann daran liegen, dass Zufallswerte "
							 << "auf ihrem Computer nicht in ausreichend guter Qualität generiert werden koennen, oder noch "
							 << "wahrschienlicher haben Sie schlechte Parameter definiert. Versuchen Sie es bitte mit anderen "
							 << "Parameter nochmals.\n\n";
					}
				}

				if (!wrong_parameters) {
					//beim Ersten Mal die Werte kopieren, um die Variablen zu befüllen:
					if (i == 1) {
						copy(world.shortest_path, world.shortest_path + dimension, overall_shortest_path);
						overall_shortest_path_hops = world.shortest_path_hops;
						overall_shortest_path_length = world.shortest_path_length;
					}

					if (world.shortest_path_length < overall_shortest_path_length
						|| (world.shortest_path_length == overall_shortest_path_length
							&& world.shortest_path_hops < overall_shortest_path_hops)) {
						copy(world.shortest_path, world.shortest_path + dimension, overall_shortest_path);
						overall_shortest_path_hops = world.shortest_path_hops;
						overall_shortest_path_length = world.shortest_path_length;
					}
					cout << "Gefundener Shortest Path mit " << i << " Ameise(n):\n";
					world.print_sh_path();
				}
			}

			if (!wrong_parameters) {
				//Auagabe des globalen Optimums:
				cout << "Bester gefundener Shortest Path:\n";
				cout << "--------------------------------\n";
				cout << "--------------------------------\n";
				copy(overall_shortest_path, overall_shortest_path + dimension, world.shortest_path);
				world.shortest_path_hops = overall_shortest_path_hops;
				world.shortest_path_length = overall_shortest_path_length;
				world.print_sh_path();
				cout << "\n\n";
			}

			bool correct = false;
			char input;
			while (!correct) {
				cout << "Wollen Sie das Programm neu starten? (mit neuem Start- und Zielknoten)\n";
				cout << "...dann geben Sie 'n' ein.\n\n";
				cout << "Wollen Sie neue Parameter eingeben? (mit gleichen Start- und Zielknoten)\n";
				cout << "...dann geben Sie 'p' ein.\n\n";
				cout << "Wollen Sie das Programm beenden?\n";
				cout << "...dann geben Sie 'e' ein.\n\n";
				cin >> input;
				if (input == 'n' || input == 'p' || input == 'e')
					correct = true;
				else
					cout << "Falsche Eingabe!\n\n";
			}
			if (input == 'n')
				exit = true;
			if (input == 'e') {
				exit = true;
				programm_laufen_lassen = false;
			}
			cout << "\n\n";
		}
	}
}