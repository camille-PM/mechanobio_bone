/************************************************************
    Algorithm to migrate cells
        7 jumps each iteration
        (30 micrometers/hour)
        MSCs only (fibroblasts possible)
************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include "header.h"

using namespace std;

void Cell_migration(char cells_migration[LATTICE_X][LATTICE_Y][LATTICE_Z], short age_migration[LATTICE_X][LATTICE_Y][LATTICE_Z], int lattice_points_element[LATTICE_X][LATTICE_Y][LATTICE_Z],
	bool graft[LATTICE_X][LATTICE_Y][LATTICE_Z], int td)
{
    int i,j,k;
    int cell_type;
    int MSCs=0;
    int migrated=0;
    vector<Point> indices;
    
    // Get total number of MSCs
    for (i=0;i<LATTICE_X;i++) {
        for (j=0;j<LATTICE_Y;j++) {
            for (k=0;k<LATTICE_Z;k++) {
                if (cells_migration[i][j][k]==1) { // it is an MSC
					if (lattice_points_element[i][j][k]!=0 || ((k<101 || k>=LATTICE_Z-101) && (pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(75,2)))) { // it is in an element or in the marrow cavity
						MSCs += 1;
						Point temp = {i,j,k};
						indices.push_back(temp);
					}
				}
            }
        }
    }
    
//    cout << "    Number of MSCs: " << MSCs << endl;
    random_shuffle(indices.begin(), indices.end()); // shuffle the vector of indices
    
	for(Point& p:indices) {
	    i = p.x;
	    j = p.y;
	    k = p.z;
	    
	    if (cells_migration[i][j][k]==1) {    
		    if (lattice_points_element[i][j][k]!=0 || ((k<101 || k>=LATTICE_Z-101) && (pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(75,2)))) {  
		        //cout<<"i,j,k: "<<i<<" ,"<<j<<" ,"<<k<<endl;
		        cell_type=1;
		        Jump_migration(cells_migration,i,j,k,age_migration,cell_type,lattice_points_element,graft,td);
		        migrated += 1;
		        //system("PAUSE");
		    }
		}
	}
	
	// Replace 6 by 1 for all migrated MSCs
    for (i=0;i<LATTICE_X;i++) {
        for (j=0;j<LATTICE_Y;j++) {
            for (k=0;k<LATTICE_Z;k++) {
                if (cells_migration[i][j][k]==6)
					cells_migration[i][j][k]=1;                      
            }
        }
    }
}
