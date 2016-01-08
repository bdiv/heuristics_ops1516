#include <iostream>
#include <fstream>
#include <String.h>

using namespace std; 

class Matrix {
	//Matrix
	double ** adjazenz;
	//Dimension
	int n; 
	//Filename
	string path; 

public: 

	Matrix(); 
	
	//Change filename
	void setpath(string path); 

	//Return Matrix from txt
	double ** getMatrix(); 

	//Return Dimension
	int getDimension(); 


};

Matrix::Matrix()
{
	//Default
	setpath("Matrix.txt"); 
}


void Matrix::setpath(string path){

	this->path = path; 

}

double ** Matrix::getMatrix(){

	//Read-in from file and generate Matrix 

	//Convert string (Filename) to char
	const char* string = path.c_str();
	
	ifstream file(string); 
	if (!file) 
	{
		cout << "File not found!" << endl;
		return NULL; 
	}

	try
	{
		//Read in first row
		file >> n;

		//Assign to memory
		adjazenz = new double* [n]; 
		for(int a = 0; a < n; ++a)
		{
			adjazenz[a] = new double [n];
		}

		//Fill Matrix from txt
		for(int a = 0; a < n; a++)
		{
			for(int b = 0; b < n; b++)
			{
			  file >> adjazenz [a][b]; 
			}
		}

		//************************ ANFANG DEBUG ANFANG ****************************
		for(int a = 0; a < n; a++)
		{
			for(int b = 0; b < n; b++)
			{
			  cout << adjazenz [a][b] << "  "; 
			}

			cout << endl; 
		}
		//************************ ENDE DEBUG ENDE ****************************

		file.close();

	}
	catch (exception e)
	{
		cout << "Fehler beim Einlesen der Datei! Bitte Format beachten";
		return NULL;
	}

	//Return Matrix
	return adjazenz; 
}

int Matrix::getDimension(){

	//Return Dimension
	return this->n; 
}

int main(){

	Matrix matrix; 

	int n = matrix.getDimension(); 
	double ** adjazenz = matrix.getMatrix(); 

	

	return 0; 
}

