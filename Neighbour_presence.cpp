#include <cstdlib>
#include "header.h"

// Different from version for cell migration because there is no source vs. destination locations to be checked for the cell.

bool Neighbour_presence(char cell_neighbour[LATTICE_X][LATTICE_Y][LATTICE_Z], int i_dest, int j_dest, int k_dest) {
	
	//srand( (unsigned)time( NULL ) ); // not needed because already used in main!!
	// Under probability p a random movement will be allowed and no neighbour test will be done
//	float p = 0.5;
//	float scale = RAND_MAX+1.;
//	float random_number = rand()/scale;
//	float fine = rand()/scale;
//	if(random_number<p) {
//		return true;
//	}
		
	// i-1
	if (i_dest!=0 && cell_neighbour[i_dest-1][j_dest][k_dest]>1 && cell_neighbour[i_dest-1][j_dest][k_dest]!=6) // neither empty nor MSC
		return true;
	
	// i+1
	if (i_dest!=LATTICE_X-1 && cell_neighbour[i_dest+1][j_dest][k_dest]>1 && cell_neighbour[i_dest+1][j_dest][k_dest]!=6)
		return true;
			
	// j-1
	if (j_dest!=0 && cell_neighbour[i_dest][j_dest-1][k_dest]>1 && cell_neighbour[i_dest][j_dest-1][k_dest]!=6)
		return true;
		
	// j+1
	if (j_dest!=LATTICE_Y-1 && cell_neighbour[i_dest][j_dest+1][k_dest]>1 && cell_neighbour[i_dest][j_dest+1][k_dest]!=6)
		return true;
	
	// k-1
	if (k_dest!=0 && cell_neighbour[i_dest][j_dest][k_dest-1]>1 && cell_neighbour[i_dest][j_dest][k_dest-1]!=6)
		return true;
		
	// k+1	
	if (k_dest!=LATTICE_Z-1 && cell_neighbour[i_dest][j_dest][k_dest+1]>1 && cell_neighbour[i_dest][j_dest][k_dest+1]!=6)
		return true;	
	
	return false; // the new position has no direct neighbour
}
