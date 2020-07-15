#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "header.h"

// For a new model: 
//       header.h: LATTICE_X, LATTICE_Y, LATTICE_Z, NUMBER_ELEMS, NUMBER_NODES, NUMBER_ITERATIONS, NODES_PER_ELEM
//       Initial cell seeding (Initialize_lattice.cpp, main.cpp)
//       Update_model.cpp: loads & BCs + file names; min_elem
//       Read_stimulus: file name

using namespace std;

int main()
{
    float (*nodes)[3] = new float[NUMBER_NODES][3];
	int (*elements)[NODES_PER_ELEM] = new int[NUMBER_ELEMS][NODES_PER_ELEM]; 
	float* stimulus = new float[NUMBER_ELEMS];
	
	int (*element_local_min)[3] = new int[NUMBER_ELEMS][3];
	int (*element_local_max)[3] = new int[NUMBER_ELEMS][3];
	float* Global_minimum = new float[3];
	float* Global_maximum = new float[3];
	
	char (*lattice)[LATTICE_Y][LATTICE_Z] = new char[LATTICE_X][LATTICE_Y][LATTICE_Z];
	short (*age_cells)[LATTICE_Y][LATTICE_Z] = new short[LATTICE_X][LATTICE_Y][LATTICE_Z];
	int (*lattice_point_element)[LATTICE_Y][LATTICE_Z] = new int[LATTICE_X][LATTICE_Y][LATTICE_Z]; //element number for each lattice point

	float (*Young_modulus)[NUMBER_ITERATIONS] = new float[NUMBER_ELEMS][NUMBER_ITERATIONS];
	float (*Poison_ratio)[NUMBER_ITERATIONS] = new float[NUMBER_ELEMS][NUMBER_ITERATIONS];
	float (*Permeability)[NUMBER_ITERATIONS] = new float[NUMBER_ELEMS][NUMBER_ITERATIONS];
	float (*Bulk_modulus)[NUMBER_ITERATIONS] = new float[NUMBER_ELEMS][NUMBER_ITERATIONS];
	
	// Initialization for first iterations (granulation tissue properties)
	for(int elem=0; elem!=NUMBER_ELEMS; elem++) {
		for(int i=0;i!=6;i++) {
			Young_modulus[elem][i] = 0.2; 
			Poison_ratio[elem][i]=0.167;
            Permeability[elem][i]=9.8e-8;
            Bulk_modulus[elem][i]=2300;
		}
	}
	
	srand((unsigned)time(NULL)); // seed for random numbers
    
    const float Cell_diameter=0.1;
    int iteration;
    int i,j,k;
    
    /****************************************************************************************
    	Set lattice points (12:scaffold, 11:bone, 0:free, age 666)
    *****************************************************************************************/
	cout<<"Initialize lattice as scaffold, bone or empty"<<endl;
	for (i=0;i<LATTICE_X;i++) {
        for (j=0;j<LATTICE_Y;j++) {
            for (k=0;k<LATTICE_Z;k++) {
                age_cells[i][j][k]=666;
                lattice[i][j][k]=0; // first empty everywhere, then given regions will be filled appropriately
                
            	if (k>101 && k<LATTICE_Z-101 && pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(107,2) &&
					pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)>=pow(50,2)) {// not in the inner cylinder
				// points in the cylinder containing the scaffold and modeled in ABAQUS are initialized as scaffold
                	lattice[i][j][k]=12; // relevant positions will be freed for tissue later on
                }

                if (i>=(LATTICE_X-1)/2+100 && i<(LATTICE_X-1)/2+144 && j>=(LATTICE_Y-1)/2-68 && j<(LATTICE_Y-1)/2+68) 
                	lattice[i][j][k]=13; // fixation plate	4.3 mm thick *13.6 mm wide
				
				if((k<100 || k>=LATTICE_Z-100)) {
					if (pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(100,2) &&  
						pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)>pow(75,2)) 
						lattice[i][j][k]=11; // intact bone in the upper and lower cylinders
	            	if (pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)>pow(100,2) &&  
						pow(i-(LATTICE_X-1)/2,2)+pow(j-(LATTICE_Y-1)/2,2)<=pow(107,2)) 
						lattice[i][j][k]=12; // possible presence of scaffold fixation 
				}
            }
        }
    }

    /**************************************************************************************
                                              Read node file
    *****************************************************************************************/
    cout<<"Read nodes"<<endl;
    Read_node_file(nodes);
 
    /**************************************************************************************
                                              Read element file
    *****************************************************************************************/
    cout<<"Read elements"<<endl;
    Read_element_file(elements);
 
    /**************************************************************************************
                                     Calculate dimensions of the lattice
    *****************************************************************************************/
    cout<<"Calculate dimensions lattice"<<endl;
    Calculate_lattice(elements,nodes,Global_minimum,Global_maximum);
    
    /**************************************************************************************
                               Calculate extreme values of x,y and z of the element
    *****************************************************************************************/
    cout<<"Calculate extreme dimensions element (cube that contains it)"<<endl;
    Extremes_element(elements,nodes,element_local_min,element_local_max,Global_minimum,Global_maximum);    

    /**************************************************************************************
                               Calculate lattice points inside each element
    *****************************************************************************************/
    cout<<"Calculate lattice points inside each tissue element and make them free"<<endl;
    Lattice_point_in_element(lattice,lattice_point_element,element_local_min,element_local_max,Global_minimum,Global_maximum,Cell_diameter,nodes,elements);   
    
    /**************************************************************************************
                                        Initialize lattice
    *****************************************************************************************/
    cout<<"Initialize lattice"<<endl;
    Initialize_lattice(lattice,age_cells);                    
    
    /**************************************************************************************
                                     Write_lattice_file_for_gnuplot
    *****************************************************************************************/
    cout<<"Write results file"<<endl;
    iteration=999;
    Write_raw_lattice_file(lattice, iteration); // full 3D model as binary file    
    
    //system("PAUSE");
    
    for (iteration=0;iteration<NUMBER_ITERATIONS;iteration++)
    {
    
        /****************************************************************************************
                                               Read stimulus
        *****************************************************************************************/   
        cout<<"Read stimulus"<<endl;
        Read_stimulus(stimulus,elements);   
        
        /**************************************************************************************
                                               Differentiation
        *****************************************************************************************/   
        if (iteration>5)
        {
           cout<<"Cell differentiation"<<endl;
           Cell_differentiation(lattice,age_cells,stimulus,element_local_min,element_local_max,lattice_point_element);           
        }  
        
        /**************************************************************************************
                                               Proliferation
        *****************************************************************************************/   
        cout<<"Cell proliferation"<<endl;
        Cell_proliferation(lattice,age_cells,element_local_min,element_local_max,lattice_point_element,stimulus); 
            
        /**************************************************************************************
                                               Migration
        *****************************************************************************************/   
        cout<<"Cell migration"<<endl;
        Cell_migration(lattice,age_cells,lattice_point_element);
        
        /**************************************************************************************
                                     Write lattice content into files
        *****************************************************************************************/
        cout<<"Write results files"<<endl;
        Write_raw_lattice_file(lattice, iteration); // full 3D model as binary file
        
        /**************************************************************************************
                                              Update model
        *****************************************************************************************/  
        if (iteration>5 && iteration<NUMBER_ITERATIONS-1) // no need to update model in the last iteration
        {
           cout<<"Update model"<<endl;
           Update_model(lattice, Young_modulus, Poison_ratio, Permeability, Bulk_modulus, iteration, element_local_min, element_local_max, lattice_point_element);
        }
        /**************************************************************************************
                                           Increase cell age
        *****************************************************************************************/               
        
		if (iteration<NUMBER_ITERATIONS-1) // not for last iteration
		{
			cout<<"Increase_age"<<endl;
        	Cell_age(age_cells); 
    	}
             
        /**************************************************************************************
                                              Run abaqus
        *****************************************************************************************/  
        if (iteration>5 && iteration<NUMBER_ITERATIONS-1 && iteration%3==0) // no need to run ABAQUS model in the last iteration; run every 3 iterations
        {
            cout<<"run abaqus"<<endl;
            system("./run.bat"); // ./run.bat in LINUX; run.bat in WINDOWS
        }
        
        cout<<"Iteration no."<<iteration<<" completed!"<<endl;
        
    }
    
	delete[] nodes;
	delete[] elements;
	delete[] stimulus;

	delete[] element_local_min;
	delete[] element_local_max;
	delete[] Global_minimum;
	delete[] Global_maximum;
	
	delete[] lattice;
	delete[] age_cells;
	delete[] lattice_point_element;

	delete[] Young_modulus;
	delete[] Poison_ratio;
	delete[] Permeability;
	delete[] Bulk_modulus;
	
    //system("PAUSE");
    return 0;
}
