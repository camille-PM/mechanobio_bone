#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "header.h"

using namespace std;

void Jump_migration(char cell_jump[LATTICE_X][LATTICE_Y][LATTICE_Z],int i_jump, int j_jump, int k_jump,short age_jump[LATTICE_X][LATTICE_Y][LATTICE_Z], 
	int cell_number, int lattice_points_element[LATTICE_X][LATTICE_Y][LATTICE_Z], bool graft[LATTICE_X][LATTICE_Y][LATTICE_Z], int td)
{
//	cout << i_jump << " " << j_jump << " " << k_jump << endl; 
	
    int position_occupied;
    int positioned;
    int possibilities[6];
    int r,r1,r2;
    const int critical_time = 5; // pure random migration allowed for 15 days, then it is driven by the graft
	 
    position_occupied=0;
    positioned=0;

    const int number_jumps=7;  // velocity = 30 micrometers/hour
    int jumps;
    jumps=0;
   
    while (jumps<number_jumps && position_occupied<6)
    {
        position_occupied=0;
        positioned=0;  
        possibilities[0]=0;
        possibilities[1]=1;
        possibilities[2]=2;
        possibilities[3]=3;
        possibilities[4]=4;
        possibilities[5]=5;    
        //cout<<"jump: "<<jumps<<endl;
        //cout<<"i_jump,j_jump,k_jump: "<<i_jump<<" ,"<<j_jump<<" ,"<<k_jump<<endl;
      
        while (positioned==0 && position_occupied<6) 
        {
            r=nrand(6);
            r1=possibilities[r];
            if (r1!=999)
            {
                if (r1==0)  //move +x
                {
                    if (i_jump==LATTICE_X-1 || cell_jump[i_jump+1][j_jump][k_jump]!=0)
                    {
                        position_occupied=position_occupied+1;
                        possibilities[r]=999;
                    }
                    else
                    {
                    	if (td<critical_time || graft[i_jump+1][j_jump][k_jump]) { // there is graft
                    		cell_jump[i_jump+1][j_jump][k_jump]=cell_number+5;
	                        cell_jump[i_jump][j_jump][k_jump]=0;
	                        age_jump[i_jump+1][j_jump][k_jump]=age_jump[i_jump][j_jump][k_jump];
	                        age_jump[i_jump][j_jump][k_jump]=0;
	                        positioned=1;
	                        jumps=jumps+1;
	                        i_jump=i_jump+1;
						}
						else { 
							position_occupied=position_occupied+1;
                    		possibilities[r]=999;
						}
                    }
                }
                else
                if (r1==1) //move -x
                {
                    if (i_jump==0 || cell_jump[i_jump-1][j_jump][k_jump]!=0)
                    {
                        position_occupied=position_occupied+1;
                        possibilities[r]=999;
                    }
                    else
                    {
	                    if (td<critical_time || graft[i_jump-1][j_jump][k_jump]) {
	                        cell_jump[i_jump-1][j_jump][k_jump]=cell_number+5;
	                        cell_jump[i_jump][j_jump][k_jump]=0;
	                        age_jump[i_jump-1][j_jump][k_jump]=age_jump[i_jump][j_jump][k_jump];
	                        age_jump[i_jump][j_jump][k_jump]=0;
	                        positioned=1;
	                        jumps=jumps+1;
	                        i_jump=i_jump-1;
                    	}
						else { 
							position_occupied=position_occupied+1;
                    		possibilities[r]=999;
						}
                    }     
                }
                else 
                if (r1==2) //move +y
                {
                    if (j_jump==LATTICE_Y-1 || cell_jump[i_jump][j_jump+1][k_jump]!=0)
                    {
                        position_occupied=position_occupied+1;
                        possibilities[r]=999;
                    }
                    else {
                    	if (td<critical_time || graft[i_jump][j_jump+1][k_jump]) {
	                        cell_jump[i_jump][j_jump+1][k_jump]=cell_number+5;
	                        cell_jump[i_jump][j_jump][k_jump]=0;
	                        age_jump[i_jump][j_jump+1][k_jump]=age_jump[i_jump][j_jump][k_jump];
	                        age_jump[i_jump][j_jump][k_jump]=0;
	                        positioned=1;
	                        jumps=jumps+1;
	                        j_jump=j_jump+1;
                    	}
						else { 
							position_occupied=position_occupied+1;
                    		possibilities[r]=999;
						}
                    }
                }
                else 
                if (r1==3) //move -y
                {
                    if (j_jump==0 || cell_jump[i_jump][j_jump-1][k_jump]!=0)
                    {
                        position_occupied=position_occupied+1;
                        possibilities[r]=999;
                    }
                    else
                    {
                    	if (td<critical_time || graft[i_jump][j_jump-1][k_jump]) {
	                        cell_jump[i_jump][j_jump-1][k_jump]=cell_number+5;
	                        cell_jump[i_jump][j_jump][k_jump]=0;
	                        age_jump[i_jump][j_jump-1][k_jump]=age_jump[i_jump][j_jump][k_jump];
	                        age_jump[i_jump][j_jump][k_jump]=0;
	                        positioned=1;
	                        jumps=jumps+1;
	                        j_jump=j_jump-1;
	                    }
						else { 
							position_occupied=position_occupied+1;
                    		possibilities[r]=999;
						}
                    }
                }
                else 
                if (r1==4) //move +z
                {
                    if (k_jump==LATTICE_Z-1 || cell_jump[i_jump][j_jump][k_jump+1]!=0)
                    {
                        position_occupied=position_occupied+1;
                        possibilities[r]=999;
                    }
                    else
                    {
                    	if (td<critical_time || graft[i_jump][j_jump][k_jump+1]) {
	                        cell_jump[i_jump][j_jump][k_jump+1]=cell_number+5;
	                        cell_jump[i_jump][j_jump][k_jump]=0;
	                        age_jump[i_jump][j_jump][k_jump+1]=age_jump[i_jump][j_jump][k_jump];
	                        age_jump[i_jump][j_jump][k_jump]=0;
	                        positioned=1;
	                        jumps=jumps+1;
	                        k_jump=k_jump+1;
	                    }
						else { 
							position_occupied=position_occupied+1;
                    		possibilities[r]=999;
						}
                    }
                }
                else 
                if (r1==5) //move -z
                {
                    if (k_jump==0 || cell_jump[i_jump][j_jump][k_jump-1]!=0)
                    {
                        position_occupied=position_occupied+1;
                        possibilities[r]=999;
                    }
                    else
                    {
                    	if (td<critical_time || graft[i_jump][j_jump][k_jump-1]) {
	                        cell_jump[i_jump][j_jump][k_jump-1]=cell_number+5;
	                        cell_jump[i_jump][j_jump][k_jump]=0;
	                        age_jump[i_jump][j_jump][k_jump-1]=age_jump[i_jump][j_jump][k_jump];
	                        age_jump[i_jump][j_jump][k_jump]=0;
	                        positioned=1;
	                        jumps=jumps+1;
	                        k_jump=k_jump-1;
	                    }
						else { 
							position_occupied=position_occupied+1;
                    		possibilities[r]=999;
						}
                    }
                }                      
            }
        }
    }
    
    // If the cell is not anymore in the callus (and not in the bone marrow), it is removed
    if (lattice_points_element[i_jump][j_jump][k_jump]==0 && !((k_jump<101 || k_jump>=LATTICE_Z-101) && (pow(i_jump-(LATTICE_X-1)/2,2)+pow(j_jump-(LATTICE_Y-1)/2,2)<=pow(75,2)))) {
    	cell_jump[i_jump][j_jump][k_jump] = 0;
    	age_jump[i_jump][j_jump][k_jump] = 0;
	}
	
}
