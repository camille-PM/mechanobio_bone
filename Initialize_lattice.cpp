#include <cstdlib>        //Includes pseudo-random number generation
#include <iostream>       //To read and write data from files as input/output streams
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include "header.h"


using namespace std;

void Initialize_lattice(char initial_lattice[LATTICE_X][LATTICE_Y][LATTICE_Z], short initial_age[LATTICE_X][LATTICE_Y][LATTICE_Z])
{
    int free_positions;
    int points=0;
    int r1,r2,r3;
    int osteoblasts_to_seed;
    int seeded_cells=0;
    int i,j,k;
    int a,b,c,d;
    int i1,i2;
     
    float tetha;
    #define PI 3.14159265
    float angle;
    int radio1, radio2;     
 
    //*********************************************************************
    //          Seeding of MSCs: marrow cavity
    //********************************************************************
 	
	radio1 = 75;
	
	for (i=0;i<LATTICE_X;i++)
    {
        for (j=0;j<LATTICE_Y;j++)
        {
            for (k=0;k<20;k+=3) // 30% positions, 2 mm height
            {
            	if (initial_lattice[i][j][100-k]==0 && pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(radio1,2)) { // in marrow cavity and free position
                	initial_lattice[i][j][100-k]=1; // seed one MSC with age 0
 					initial_age[i][j][100-k]=1;
 				}
 				if (initial_lattice[i][j][LATTICE_Z-100+k]==0 && pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(radio1,2)) { // in marrow cavity and free position
                	initial_lattice[i][j][LATTICE_Z-100+k]=1; // seed one MSC with age 0
 					initial_age[i][j][LATTICE_Z-100+k]=1;
 				}
 			}
 		}
 	}
	  
    //**********************
    //    Periosteum: no need to check for scaffold presence
    //**********************     
    
    radio2 = 100;
    
    for (k=0;k<100;k=k+3)  // 30% positions in 1 cm below defect
    {
        for (tetha=0.0;tetha<360.0;tetha++)
        {
            angle=PI*tetha/180;
            i=LATTICE_X/2+int(radio2*cos(angle)); 
            j=LATTICE_Y/2+int(radio2*sin(angle));
            
            initial_lattice[i][j][k]=1; // put an MSC with age 0
            initial_age[i][j][k]=1;
        }
    }
       
    for (k=LATTICE_Z-100;k<LATTICE_Z;k=k+3) // 30% positions in 1 cm above defect
    {
        for (tetha=0.0;tetha<360.0;tetha++)
        {
            angle=PI*tetha/180;
            i=LATTICE_X/2+int(radio2*cos(angle)); 
            j=LATTICE_Y/2+int(radio2*sin(angle));
            
            initial_lattice[i][j][k]=1;
            initial_age[i][j][k]=1;
        }
    }
    
    //*********************************************************************
    //          Seeding of MSCs: in the graft -> with artificial age to spot them later
    //********************************************************************
 	radio1 = 50; // scaffold inner radius
 	
 	free_positions=0;
	for (i=0;i<LATTICE_X;i++) {
        for (j=0;j<LATTICE_Y;j++) {
            for (k=100;k<LATTICE_Z-100;k++) {
            	if (initial_lattice[i][j][k]==0 && pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(radio2,2) &&
					pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)>pow(radio1,2)) { 
            		free_positions+=1;
 				}
 			}
 		}
 	}
 	
 	int MSCs_to_seed = free_positions/1000; // 1% of the graft is occupied by MSCs
	while (seeded_cells<MSCs_to_seed) {
		i = nrand(LATTICE_X);
		j = nrand(LATTICE_Y);
		r3 = nrand(LATTICE_Z-200);
		k = 100+r3;
		
		if (initial_lattice[i][j][k]==0 && pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(radio2,2) &&
			pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)>pow(radio1,2)) {
			initial_lattice[i][j][k]=1;
			initial_age[i][j][k]=1; // 300 for the low proliferation case; 1 otherwise
			seeded_cells += 1;
		}
	}
      
}
