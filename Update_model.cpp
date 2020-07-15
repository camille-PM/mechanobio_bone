#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "header.h"

using namespace std;

/***********************************************************************
	Updates the material properties for each callus FE (rule of mixture)
	and writes the new Abaqus input file for the next FEA.
***********************************************************************/

void Update_model(char lattice[LATTICE_X][LATTICE_Y][LATTICE_Z], float Young_modulus_prop[NUMBER_ELEMS][NUMBER_ITERATIONS], float Poison_ratio_prop[NUMBER_ELEMS][NUMBER_ITERATIONS], 
	float Permeability_prop[NUMBER_ELEMS][NUMBER_ITERATIONS], float Bulk_modulus_prop[NUMBER_ELEMS][NUMBER_ITERATIONS],int t_prop,int element_local_min[NUMBER_ELEMS][3],
	int element_local_max[NUMBER_ELEMS][3],int lattice_point_element[LATTICE_X][LATTICE_Y][LATTICE_Z])
{    
    float* Young_mod_elem = new float[NUMBER_ELEMS];
	float* Poison_rat_elem = new float[NUMBER_ELEMS];
	float* Permeability_elem = new float[NUMBER_ELEMS]; 
	float* Bulk_mod_elem = new float[NUMBER_ELEMS];
     
	int elem;
    int i,j,k;
    int imin,jmin,kmin;
    int imax,jmax,kmax;
    float total;
    float number_MSC,number_chondrocytes,number_inmature_osteoblasts,number_fibroblasts,number_mature_osteoblasts;
    int count;
    int count2;
    int first_element = 1; // first element in callus
     
    for(elem=0;elem<NUMBER_ELEMS;elem++)
    {
        Young_mod_elem[elem]=Poison_rat_elem[elem]=Permeability_elem[elem]=Bulk_mod_elem[elem]=0.0;
    }
     
     
    for (elem=0;elem<NUMBER_ELEMS;elem++)
    {
            
        imin=element_local_min[elem][0];
        jmin=element_local_min[elem][1];
        kmin=element_local_min[elem][2];
        
        imax=element_local_max[elem][0];
        jmax=element_local_max[elem][1];
        kmax=element_local_max[elem][2];
         
        total=0.0;
        
        for (i=imin;i<imax;i++)
        {
            for (j=jmin;j<jmax;j++)
            {
                for (k=kmin;k<kmax;k++)
                {
                   if (lattice_point_element[i][j][k]==elem+1)
                   {
                        total=total+1.0;
                   }
                }
            }
        }
                        
        if (total==0.0)
        {
            Young_mod_elem[elem]=0.2;
            Poison_rat_elem[elem]=0.167;
            Permeability_elem[elem]=9.8e-8;
            Bulk_mod_elem[elem]=2300.0;
        }
        else
        {
         
            number_MSC=number_chondrocytes=number_inmature_osteoblasts=number_fibroblasts=number_mature_osteoblasts=0;
    
            for (i=imin;i<imax;i++)
            {
                for (j=jmin;j<jmax;j++)
                {
                    for (k=kmin;k<kmax;k++)
                    {
                        if (lattice_point_element[i][j][k]==elem+1)
                        {
                            switch (lattice[i][j][k])
                            {
                                case 1:
                                    number_MSC=number_MSC+1.0;
                                    break;
                                case 2:
                                    number_mature_osteoblasts=number_mature_osteoblasts+1.0;
                                    break;
                                case 3:
                                    number_inmature_osteoblasts=number_inmature_osteoblasts+1.0;
                                    break;
                                case 4:
                                    number_chondrocytes=number_chondrocytes+1.0;  
                                    break;
                                case 5:
                                    number_fibroblasts=number_fibroblasts+1.0; 
                                    break;    
                            }   
                        }
                    }
                }
            } //end for
            
         
        Young_modulus_prop[elem][t_prop]=(number_chondrocytes*10.0/total)+(number_inmature_osteoblasts*1000.0/total)+(number_fibroblasts*2.0/total)
			+(number_mature_osteoblasts*17000.0/total)+((total-number_fibroblasts-number_inmature_osteoblasts-number_mature_osteoblasts-number_chondrocytes)*0.2/total); // 6000 or 17000MPa for mature osteoblasts depending on species
        Poison_ratio_prop[elem][t_prop]=(number_chondrocytes*0.167/total)+(number_inmature_osteoblasts*0.3/total)+(number_fibroblasts*0.167/total)
			+(number_mature_osteoblasts*0.3/total)+((total-number_fibroblasts-number_inmature_osteoblasts-number_chondrocytes-number_mature_osteoblasts)*0.167/total);
        Permeability_prop[elem][t_prop]=(number_chondrocytes*4.9e-8/total)+(number_inmature_osteoblasts*98e-8/total)+(number_fibroblasts*9.8e-8/total)
			+(number_mature_osteoblasts*362.6e-8/total)+((total-number_fibroblasts-number_inmature_osteoblasts-number_chondrocytes-number_mature_osteoblasts)*9.8e-8/total);
        Bulk_modulus_prop[elem][t_prop]=(number_chondrocytes*3400.0/total)+(number_inmature_osteoblasts*13920.0/total)+(number_fibroblasts*2300.0/total)
			+(number_mature_osteoblasts*13920.0/total)+((total-number_fibroblasts-number_inmature_osteoblasts-number_mature_osteoblasts-number_chondrocytes)*2300.0/total); 

         
        i=t_prop;         
        count=0;
        while (i>=0 && count<10)
        {
            Young_mod_elem[elem]=Young_mod_elem[elem]+Young_modulus_prop[elem][i];
            Poison_rat_elem[elem]=Poison_rat_elem[elem]+Poison_ratio_prop[elem][i];
            Permeability_elem[elem]=Permeability_elem[elem]+Permeability_prop[elem][i];
            Bulk_mod_elem[elem]=Bulk_mod_elem[elem]+Bulk_modulus_prop[elem][i];
            count=count+1;
            i=i-1;
        }
         
        //added to consider change of material properties****************
        if (count<=9)
        {
            count2=10-count;
            Young_mod_elem[elem]=Young_mod_elem[elem]+0.2*count2;
            Poison_rat_elem[elem]=Poison_rat_elem[elem]+0.167*count2;
            Permeability_elem[elem]=Permeability_elem[elem]+count2*9.8e-8;
            Bulk_mod_elem[elem]=Bulk_mod_elem[elem]+count2*2300;
            
        }
        //*******************************************************************
         
        Young_mod_elem[elem]=Young_mod_elem[elem]/10;
        Poison_rat_elem[elem]=Poison_rat_elem[elem]/10;
        Permeability_elem[elem]=Permeability_elem[elem]/10;
        Bulk_mod_elem[elem]=Bulk_mod_elem[elem]/10;

        } // end else total==0.0  
         
    }// end for element
     
	if (t_prop>=0) //>5 
	{
     
		 //************* Write to file ************

		system("rm SW12.inp");
		system("cp Scaffold_model.txt SW12.inp"); // cp in LINUX; copy in WINDOWS
		ofstream myfile;
		myfile.open("SW12.inp",ios::app);
		for (elem=0;elem<NUMBER_ELEMS;elem++)
		{
			ostringstream s1;
			s1<<"material_"<<elem+first_element;
			string materialname=s1.str();
			 
			myfile<<"\n*Material, name="<<materialname<<endl;
			myfile<<"*Elastic"<<endl;
			myfile<<Young_mod_elem[elem]<<","<<Poison_rat_elem[elem]<<endl;
			myfile<<"*Permeability, specific=9.8e-06"<<endl;
			myfile<<Permeability_elem[elem]<<",4"<<endl;
			myfile<<"*Porous Bulk Moduli"<<endl;
			myfile<<Bulk_mod_elem[elem]<<",2300.";
		}		 
		 
		//*** Write to file rest of FEM information
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nCallus-1.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nImplant-1.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nCorticalBone-1.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nCorticalBone-2.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nMarrow-1.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nMarrow-2.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nPlate_with_holes-1.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-1.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-2.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-3.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-4.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-5.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-6.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-7.Set-1, 4."<<endl; 
		myfile<<"\n**\n*INITIAL CONDITIONS, TYPE=RATIO\nScrew-8.Set-1, 4."<<endl; 
		myfile<<"** --------------------------------------------------------------- "<<endl;
		myfile<<"** BOUNDARY CONDITIONS"<<endl;
		myfile<<"**"<<endl;
		myfile<<"** Name: BC-1 Type: Displacement/Rotation"<<endl;
		myfile<<"*Boundary"<<endl;
		myfile<<"m_Set-2, 1, 1"<<endl;
		myfile<<"m_Set-2, 2, 2"<<endl;
		myfile<<"m_Set-2, 6, 6"<<endl;
		myfile<<"** Name: BC-2 Type: Symmetry/Antisymmetry/Encastre"<<endl;
		myfile<<"*Boundary"<<endl;
		myfile<<"m_Set-1, ENCASTRE"<<endl;
		myfile<<"** Name: BC-3 Type: Pore pressure"<<endl;
		myfile<<"*Boundary"<<endl;
		myfile<<"surf_fluidBC_PP_, 8, 8"<<endl;
		myfile<<"**"<<endl;
		myfile<<"** ----------------------------------------------------------------"<<endl;
		myfile<<"**"<<endl;
		myfile<<"**"<<endl;
		myfile<<"**"<<endl;
		myfile<<"** STEP: Step-1"<<endl;
		myfile<<"** "<<endl;
		myfile<<"*Step, name=Step-1, AMPLITUDE=RAMP"<<endl; // ramp to achieve convergence
		myfile<<"*Soils, consolidation, end=PERIOD, utol=1., creep=none"<<endl;
		myfile<<"1., 1., 1e-05, 1.,"<<endl; 
		myfile<<"** "<<endl;
		myfile<<"**--------------  Loads  ------------------------------------------"<<endl;
		myfile<<"**"<<endl;
		myfile<<"**"<<endl;
		myfile<<"** Name: Load-1   Type: Concentrated force"<<endl;
		myfile<<"*Cload"<<endl;
		myfile<<"m_Set-2, 3, -1372."<<endl;
		myfile<<"** Name: Load-2   Type: Moment"<<endl;
		myfile<<"*Cload"<<endl;
		myfile<<"m_Set-2, 4, -17125."<<endl;
		myfile<<"** "<<endl;
		myfile<<"**"<<endl;
		myfile<<"**---------------- OUTPUT REQUESTS -------------------------------"<<endl;
		myfile<<"** "<<endl;
		myfile<<"*Restart, write, frequency=0"<<endl;
		myfile<<"** "<<endl;
		myfile<<"** FIELD OUTPUT: F-Output-1"<<endl;
		myfile<<"**"<<endl; 
		myfile<<"*Output, field"<<endl;
		myfile<<"*Element Output, directions=YES"<<endl;
		myfile<<"E, FLVEL, S"<<endl;
		myfile<<"**"<<endl;
		myfile<<"**"<<endl;
		myfile<<"*EL PRINT, FREQUENCY=100, ELSET=Callus-1.Set-1"<<endl;
		myfile<<"FLVELM,E"<<endl;
		myfile<<"**"<<endl;
		myfile<<"**"<<endl;
		myfile<<"** HISTORY OUTPUT: H-Output-1"<<endl;
		myfile<<"**"<<endl;
		myfile<<"**"<<endl;
		myfile<<"*Output, history"<<endl;
		myfile<<"*Energy Output"<<endl;
		myfile<<"ALLAE,"<<endl;
		myfile<<"*End Step"<<endl;

	    myfile.close();        
	}
	
	delete[] Young_mod_elem;
	delete[] Poison_rat_elem;
	delete[] Permeability_elem; 
	delete[] Bulk_mod_elem;
    
}
