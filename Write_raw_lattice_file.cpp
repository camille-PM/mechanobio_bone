#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include "header.h"

using namespace std;

/***********************************************************************
	Writes the array describing the lattice into a raw file.
***********************************************************************/

void Write_raw_lattice_file(char const raw_cell[LATTICE_X][LATTICE_Y][LATTICE_Z], int td)
{
	ostringstream f;
	f<<"lattice_"<<td<<".raw";
	string name=f.str();
	ofstream file(name.c_str(), ios::out | ios::binary);
  
	if(file) // test if file could open without error
		file.write(reinterpret_cast<char const *>(raw_cell), (LATTICE_X-2*TRUNC)*(LATTICE_Y-2*TRUNC)*LATTICE_Z);
	else 
		cout << "ERROR:Impossible to open file." << endl;
  
	file.close();
}
