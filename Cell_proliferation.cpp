#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include "header.h"

using namespace std;

/*****************************************************************************
	Proliferates cells (randomly picked) according to Lacroix & Prendergasr 
	mechanoregulation values.
*****************************************************************************/

void Cell_proliferation(char cells_prol[LATTICE_X][LATTICE_Y][LATTICE_Z], short age_prol[LATTICE_X][LATTICE_Y][LATTICE_Z],int element_local_min [NUMBER_ELEMS][3],
	int element_local_max[NUMBER_ELEMS][3],int lattice_point_element[LATTICE_X][LATTICE_Y][LATTICE_Z], float stimulus_prol[NUMBER_ELEMS])
{
    int elem, elem2;
    int imin,imax,jmin,jmax,kmin,kmax;
    float stimulus;
    int i,j,k;
    int MSCs,fibroblasts,chondrocytes,inmature_osteoblasts,mature_osteoblasts;
    int MSCs_to_prol,fibroblasts_to_prol,chondrocytes_to_prol,inmature_osteoblasts_to_prol,mature_osteoblasts_to_prol;
    int proliferated;
    int r1,r2,r3;
    int cell; // cell type
        
    // Shuffle the elements to look them over in a random way (should not have a big impact though)
    int* elements = new int[NUMBER_ELEMS];
    for (elem=0;elem<NUMBER_ELEMS;elem++){
    	elements[elem] = elem;
	}
	int temp;
	for(i=0; i<NUMBER_ELEMS-1; i++) {
		j = i+nrand(NUMBER_ELEMS-1-i);
		temp = elements[i];
		elements[i] = elements[j];
		elements[j] = temp;
	}
	
    for (elem2=0;elem2<NUMBER_ELEMS;elem2++) 
    {
//        cout << "Iter " << elem2 << endl;
		elem = elements[elem2];
//        cout << "Element " << elem << endl;
		
		stimulus=stimulus_prol[elem];  
        imax=element_local_max[elem][0];
        imin=element_local_min[elem][0];
        jmax=element_local_max[elem][1];
        jmin=element_local_min[elem][1];
        kmax=element_local_max[elem][2];
        kmin=element_local_min[elem][2];
//        cout << "Stimulus " << stimulus << "; imin: " << imin << "; jmin: " << jmin << "; kmin: " << kmin << endl;

		MSCs=fibroblasts=chondrocytes=inmature_osteoblasts=mature_osteoblasts=0;
		proliferated=0;
		
		for (i=imin;i<=imax;i++)
		{
			for (j=jmin;j<=jmax;j++)
			{
				for (k=kmin;k<=kmax;k++)
				{
					if (lattice_point_element[i][j][k]==elem+1)
					{
						switch (cells_prol[i][j][k])
						{
							case 1:
								MSCs=MSCs+1;
								break;
							case 2:
								mature_osteoblasts=mature_osteoblasts+1;
								break;
							case 3:
								inmature_osteoblasts=inmature_osteoblasts+1;
								break;
							case 4:
								chondrocytes=chondrocytes+1;
								break;
							case 5:
								fibroblasts=fibroblasts+1;
								break;
						}
					}
				}
			}
		}
			 
			MSCs_to_prol=int(MSCs*0.6);
//			cout << "MSCs to proliferate: " << MSCs_to_prol << endl;
			fibroblasts_to_prol=int(fibroblasts*0.55);
			chondrocytes_to_prol=int(chondrocytes*0.20);
			inmature_osteoblasts_to_prol=int(inmature_osteoblasts*0.3);
			mature_osteoblasts_to_prol=int(mature_osteoblasts*0.3);         
					 
			/******************************************
					 Proliferate MSCs
			******************************************/

			while (proliferated<MSCs_to_prol)
			{
			    r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
		        r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
		        r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
				i=imin+r1;
				j=jmin+r2;
				k=kmin+r3;
				
				if (lattice_point_element[i][j][k]==elem+1) // if still in same element
				{
					if (cells_prol[i][j][k]==1) // if it is an MSC here
					{
						cell=1;
						Cell_mitosis(cells_prol,i,j,k,cell,age_prol);                        
						proliferated=proliferated+1;
						//cout << "One cell proliferated!" << endl;               
					}
				}
				//cout << proliferated*(elem+1) << endl;
			}
	   
			proliferated=0;
			 
			/******************************************
				Proliferate fibroblasts 
			******************************************/
			if (stimulus>3.0)
			{
				while (proliferated<fibroblasts_to_prol)
				{
				    r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
			        r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
			        r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
					i=imin+r1;
					j=jmin+r2;
					k=kmin+r3;
					if (lattice_point_element[i][j][k]==elem+1)
					{
						if (cells_prol[i][j][k]==5)
						{
							cell=5;
							Cell_mitosis(cells_prol,i,j,k,cell,age_prol);                        
							proliferated=proliferated+1;                   
						}
					}
				}		  
			}

			proliferated=0;         

			/******************************************
				Proliferate chondrocytes
			******************************************/
			if (stimulus<=3.0 && stimulus>1.0)
			{
				while (proliferated<chondrocytes_to_prol)
				{
				    r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
			        r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
			        r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
					i=imin+r1;
					j=jmin+r2;
					k=kmin+r3;
					  
					if (lattice_point_element[i][j][k]==elem+1)
					{
						if (cells_prol[i][j][k]==4)
						{
							cell=4;
							Cell_mitosis(cells_prol,i,j,k,cell,age_prol);                        
							proliferated=proliferated+1;                   
						}
					}
				}						 
			}     
			 
			proliferated=0;
						  
			/*******************************************
				Proliferate inmature osteoblasts
			*******************************************/
			if (stimulus>0.53 && stimulus<=1.0)
			{
				while (proliferated<inmature_osteoblasts_to_prol)
				{
				    r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
			        r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
			        r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
					i=imin+r1;
					j=jmin+r2;
					k=kmin+r3;
					  
					if (lattice_point_element[i][j][k]==elem+1)
					{
						if (cells_prol[i][j][k]==3)
						{
							cell=3;
							Cell_mitosis(cells_prol,i,j,k,cell,age_prol);                        
							proliferated=proliferated+1;                   
						}
					}
				}
			}     
			 
			proliferated=0;    
			 
			/******************************************
				Proliferate mature osteoblasts
			******************************************/
			if (stimulus>0.01 && stimulus<=0.53)
			{
				while (proliferated<mature_osteoblasts_to_prol)
				{
				    r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
			        r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
			        r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
					i=imin+r1;
					j=jmin+r2;
					k=kmin+r3;
					if (lattice_point_element[i][j][k]==elem+1)
					{
						if (cells_prol[i][j][k]==2)
						{
							cell=2;
							Cell_mitosis(cells_prol,i,j,k,cell,age_prol);                        
							proliferated=proliferated+1;                   
						}
					}
				}
			}     
//        }  // end else (when if loop used)		
    }// end for element
     

    //Change all the new cells to cell number  
    for (i=0;i<LATTICE_X;i++)
    {
        for (j=0;j<LATTICE_Y;j++)
        {
            for(k=0;k<LATTICE_Z;k++)
            {
                switch (cells_prol[i][j][k])
                {
                    case 6:
                        cells_prol[i][j][k]=1;
                        break;
                    case 7:
                        cells_prol[i][j][k]=2;
                        break;
                    case 8:
                        cells_prol[i][j][k]=3;
                        break;
                    case 9:
                        cells_prol[i][j][k]=4;
                        break;
                    case 10:
                        cells_prol[i][j][k]=5;
                        break;
                }
            }
        }
    }
    
}
