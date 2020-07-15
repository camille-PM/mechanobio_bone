#include <cstdlib>
#include <iostream>
#include <fstream>
#include "header.h"
#include <ctime>

using namespace std;

void Cell_differentiation(char cells_dif[LATTICE_X][LATTICE_Y][LATTICE_Z],short age_dif[LATTICE_X][LATTICE_Y][LATTICE_Z],float stimulus_dif[NUMBER_ELEMS], 
	int element_local_min[NUMBER_ELEMS][3],int element_local_max[NUMBER_ELEMS][3],int lattice_point_element[LATTICE_X][LATTICE_Y][LATTICE_Z])
{
    /************************************************************
        Randomly differentiates MSCs into new phentypes and picks cells for apoptosis
		according to Lacroix & Prendergast mechanoregulation theory:
			If Stimulus > 3               Fibrous tissue
	        If 1 < Stimulus <= 3           Cartilage          
	        If 0.53 < Stimulus <= 1        Inmature bone
	        If 0.01 < Stimulus <= 0.53     Mature bone
	        If 0 < Stimulus <= 0.01         Resorption
                       
    *************************************************************/ 
     
     
    int elem;
    int points;
    int i,j,k;
    int imin,imax,jmin,jmax,kmin,kmax;
    int r1,r2,r3;
    int i1,j1,k1;
    int i2,j2,k2;
    int i3,j3,k3;
    int EC;
    int cells_to_differentiate;
    int cells_differentiated;
    int inmature_osteoblasts,mature_osteoblasts,chondrocytes,fibroblasts;
    const int age_mature_cell=6;
    const int oxygen_diffusion_distance=10;
    
    //srand((unsigned)time(0)); // not needed because already in main!
    
    int chondrocytes_apoptosis;
    int fibroblasts_apoptosis;
    int mature_osteoblasts_apoptosis;
    int inmature_osteoblasts_apoptosis;
    
    const float apoptosis_rate_osteoblasts=0.16;
    const float apoptosis_rate_chondrocytes=0.1;
    const float apoptosis_rate_fibroblasts=0.05;
    
    int chondrocytes_removed;
    int inmature_osteoblasts_removed;
    int fibroblasts_removed;
    int mature_osteoblasts_removed;
    
    for (elem=0;elem<NUMBER_ELEMS;elem++)
    {
        cells_to_differentiate=0;
        cells_differentiated=0;

        mature_osteoblasts=0;
        inmature_osteoblasts=0;
        chondrocytes=0;
        fibroblasts=0;
        
        imax=element_local_max[elem][0];
        imin=element_local_min[elem][0];
        jmax=element_local_max[elem][1];
        jmin=element_local_min[elem][1];
        kmax=element_local_max[elem][2];
        kmin=element_local_min[elem][2];
         
/*	    if ((imax>0.32*LATTICE_X && imin<0.64*LATTICE_X && jmax>0.32*LATTICE_Y && jmin<0.32*LATTICE_Y) && (kmax<0.49*LATTICE_Z || kmin>0.51*LATTICE_Z)) // to be adapted to real geometry of cavity - also in cell proliferation !!!!!
	    {
	       // cout<<"in the cavity: "<<endl;;         
	    } 
	    else
	    { */
	        for (i=imin;i<imax;i++)
	        {
	            for (j=jmin;j<jmax;j++)
	            {
	                for (k=kmin;k<kmax;k++)
	                {
	                    if (lattice_point_element[i][j][k]==elem+1)
	                    {
	                        
	                        if (cells_dif[i][j][k]==1 && age_dif[i][j][k]>=age_mature_cell)
	                        {
	                            cells_to_differentiate=cells_to_differentiate+1;
	                        }
	                        if (cells_dif[i][j][k]==2)
	                        {
	                            mature_osteoblasts=mature_osteoblasts+1;                          
	                        }
	                        if (cells_dif[i][j][k]==3)
	                        {
	                           inmature_osteoblasts=inmature_osteoblasts+1;
	                        }
	                        if (cells_dif[i][j][k]==4)
	                        {
	                           chondrocytes=chondrocytes+1;
	                        }
	                        if (cells_dif[i][j][k]==5)
	                        {
	                           fibroblasts=fibroblasts+1;
	                        }
	                    }
	                }
	            }
	        }
	        
	        cells_to_differentiate=int(cells_to_differentiate*0.3);
	        
	        mature_osteoblasts_apoptosis=int(mature_osteoblasts*apoptosis_rate_osteoblasts);
	        chondrocytes_apoptosis=int(chondrocytes*apoptosis_rate_chondrocytes);
	        inmature_osteoblasts_apoptosis=int(inmature_osteoblasts*apoptosis_rate_osteoblasts);
	        fibroblasts_apoptosis=int(fibroblasts*apoptosis_rate_fibroblasts);
	         
	        chondrocytes_removed=inmature_osteoblasts_removed=fibroblasts_removed=mature_osteoblasts_removed=0;
	        
	        //cout<<"mature_osteoblasts_apoptosis: "<<mature_osteoblasts_apoptosis<<endl;
	        //cout<<"chondrocytes_apoptosis: "<<chondrocytes_apoptosis<<endl;
	        //cout<<"inmature_osteoblasts_apoptosis: "<<inmature_osteoblasts_apoptosis<<endl;
	        //cout<<"fibroblasts_apoptosis: "<<fibroblasts_apoptosis<<endl;
	         
	         
	        /*********************************************************************************************************
	                                            FIBROUS TISSUE         
	        **********************************************************************************************************/
	        if (stimulus_dif[elem]>3)
	        {
	            //cout<<"fibroblast differentiation"<<endl;                 
	            //CELL APOPTOSIS
	            while (chondrocytes_removed<chondrocytes_apoptosis)
	            {
	                //cout<<"apoptosis chondrocytes under fibroblast stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {                       
	                    if (cells_dif[i][j][k]==4)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        chondrocytes_removed=chondrocytes_removed+1;                    
	                    }
	                }
	            }
	                 
	            while (inmature_osteoblasts_removed<inmature_osteoblasts_apoptosis)
	            {
	                //cout<<"apoptosis inmature osteoblasts under fibroblast stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {                 
	                    if (cells_dif[i][j][k]==3)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        inmature_osteoblasts_removed=inmature_osteoblasts_removed+1;                    
	                    }
	                  
	                }
	            }
	            
	            while (mature_osteoblasts_removed<mature_osteoblasts_apoptosis)
	            {
	                //cout<<"apoptosis mature osteoblasts under fibroblast stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==2)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        mature_osteoblasts_removed=mature_osteoblasts_removed+1;                    
	                    }                                     
	                }
	                  
	            }
	                  
	            //CELL DIFFERENTIATION
	            while (cells_differentiated<cells_to_differentiate)
	            {
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==1 && age_dif[i][j][k]>=age_mature_cell)
	                    {
	                        cells_dif[i][j][k]=5;
	                        age_dif[i][j][k]=1;
	                        cells_differentiated=cells_differentiated+1;
	                    }                                        
	                }
	                  
	            }                
	                  
	        }//end stimulus fibrous tissue
	        
	        /*********************************************************************************************************
	                                             CHONDROCYTES         
	        **********************************************************************************************************/
	          
	        if (stimulus_dif[elem]>1 && stimulus_dif[elem]<=3)
	        {
	            //cout<<"cartilage differentiation"<<endl;
	            //CELL APOPTOSIS
	            while (fibroblasts_removed<fibroblasts_apoptosis)
	            {
	                //cout<<"apoptosis fibroblasts under cartilage stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==5)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        fibroblasts_removed=fibroblasts_removed+1;                    
	                    }
	                }
	            }
	            
	
	            while (inmature_osteoblasts_removed<inmature_osteoblasts_apoptosis)
	            {
	                //cout<<"apoptosis inmature osteoblasts under cartilage stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==3)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        inmature_osteoblasts_removed=inmature_osteoblasts_removed+1;                    
	                    }                                    
	                }
	            }
	            
	            
	            while (mature_osteoblasts_removed<mature_osteoblasts_apoptosis)
	            {
	                //cout<<"apoptosis mature osteoblasts under cartilage stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==2)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        mature_osteoblasts_removed=mature_osteoblasts_removed+1;                    
	                    }                                    
	                }
	            }
	                  
	                  
	            //CELL DIFFERENTIATION
	            while (cells_differentiated<cells_to_differentiate)
	            {
	                //cout<<"cartilage differentiation"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==1 && age_dif[i][j][k]>=age_mature_cell)
	                    {
	                        cells_dif[i][j][k]=4;
	                        age_dif[i][j][k]=1;
	                        cells_differentiated=cells_differentiated+1;
	                    }                                     
	                }
	            }                
	                                  
	        } //end stimulus chondrocyte
	        
	        /*********************************************************************************************************
	                                             INMATURE BONE         
	        **********************************************************************************************************/
	         
	        if (stimulus_dif[elem]>0.53 && stimulus_dif[elem]<=1)
	        {
	            //cout<<"inmature bone differentiation"<<endl;
	            //CELL APOPTOSIS
	            while (fibroblasts_removed<fibroblasts_apoptosis)
	            {
	                //cout<<"apoptosis fibroblasts under inmature bone stimulus"<<endl;
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==5)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        fibroblasts_removed=fibroblasts_removed+1;                    
	                    }                                      
	                }
	            }
	                 
	            while (chondrocytes_removed<chondrocytes_apoptosis)
	            {
	                //cout<<"apoptosis chondrocytes under inmature bone stimulus"<<endl;    
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==4)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        chondrocytes_removed=chondrocytes_removed+1;                    
	                    }
	                }
	            }        
	       
	                 
	            while (mature_osteoblasts_removed<mature_osteoblasts_apoptosis)
	            {
	                //cout<<"apoptosis mature osteoblasts under inmature bone stimulus"<<endl; 
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==2)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        mature_osteoblasts_removed=mature_osteoblasts_removed+1;                    
	                    }                                    
	                }
	            }
	
	
	            //CELL DIFFERENTIATION
	            while (cells_differentiated<cells_to_differentiate)
	            {
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                { 
                        if (cells_dif[i][j][k]==1 && age_dif[i][j][k]>=age_mature_cell)
                        {
                        	cells_dif[i][j][k]=3;
                        	age_dif[i][j][k]=1;
                        	cells_differentiated=cells_differentiated+1;
                        }                                       
	                }
	            }  
	        } //end stimulus immature bone
	                 
	        /*********************************************************************************************************
	                                             MATURE BONE         
	        **********************************************************************************************************/                  
	        if (stimulus_dif[elem]>0.01 && stimulus_dif[elem]<=0.53)
	        {
	            //cout<<"mature bone differentiation"<<endl;                    
	            //CELL APOPTOSIS
	            while (fibroblasts_removed<fibroblasts_apoptosis)
	            {
	                       
	                //cout<<"fibroblast apoptosis under mature bone stimulus"<<endl;                    
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==5)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        fibroblasts_removed=fibroblasts_removed+1;                    
	                    }                                         
	                }
	            }
	
	            while (chondrocytes_removed<chondrocytes_apoptosis)
	            {
	                //cout<<"chondrocytes apoptosis under mature bone stimulus"<<endl; 
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==4)
	                    {
                            cells_dif[i][j][k]=0;
                            age_dif[i][j][k]=0;   
	                        chondrocytes_removed=chondrocytes_removed+1;                    
	                    }                                             
	                }
	            }        
	            
	            while (inmature_osteoblasts_removed<inmature_osteoblasts_apoptosis)
	            {
	                //cout<<"inmature osteoblasts apoptosis under mature bone stimulus"<<endl; 
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
	                    if (cells_dif[i][j][k]==3)
	                    {
	                        cells_dif[i][j][k]=0;
	                        age_dif[i][j][k]=0;   
	                        inmature_osteoblasts_removed=inmature_osteoblasts_removed+1;                    
	                    }                                                   
	                }
	            }
	     
	            //CELL DIFFERENTIATION
	            while (cells_differentiated<cells_to_differentiate)
	            {
	                r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
	                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
	                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                i=imin+r1;
	                j=jmin+r2;
	                k=kmin+r3;
	                if (lattice_point_element[i][j][k]==elem+1)
	                {
                       if (cells_dif[i][j][k]==1 && age_dif[i][j][k]>=age_mature_cell)
                       {
                          cells_dif[i][j][k]=2;
                          age_dif[i][j][k]=1;
                          cells_differentiated=cells_differentiated+1;
                       }                                                                           
	                }// end if         
	            }             
	        } //end stimulus mature osteoblasts
	
	        /*********************************************************************************************************
	                                             BONE RESORPTION      
	        **********************************************************************************************************/        
	        if (stimulus_dif[elem]<=0.01)
	        {
	                    //CELL APOPTOSIS
	                                  
	                //cout<<"bone resorption"<<endl;
	
	                while (fibroblasts_removed<fibroblasts_apoptosis)
	                {
			            r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
		                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
		                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                	i=imin+r1;
	                    j=jmin+r2;
	                    k=kmin+r3;
	                    if (lattice_point_element[i][j][k]==elem+1)
	                    {
	                        if (cells_dif[i][j][k]==5)
	                        {
	                            cells_dif[i][j][k]=0;
	                            age_dif[i][j][k]=0;   
	                            fibroblasts_removed=fibroblasts_removed+1;                    
	                        }                                      
	                    }
	                }
	                  
	                while (chondrocytes_removed<chondrocytes_apoptosis)
	                {
	                	//cout<<"apoptosis chondrocytes under fibroblast stimulus"<<endl;
			            r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
		                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
		                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                    i=imin+r1;
	                    j=jmin+r2;
	                    k=kmin+r3;
	                    if (lattice_point_element[i][j][k]==elem+1)
	                    {                       
	                        if (cells_dif[i][j][k]==4)
	                        {
	                            cells_dif[i][j][k]=0;
	                            age_dif[i][j][k]=0;   
	                            chondrocytes_removed=chondrocytes_removed+1;                    
	                        }
	                    }
	                }
	                 
	                while (inmature_osteoblasts_removed<inmature_osteoblasts_apoptosis)
	                {
			            r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
		                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
		                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                    i=imin+r1;
	                    j=jmin+r2;
	                    k=kmin+r3;
	                    if (lattice_point_element[i][j][k]==elem+1)
	                    {
	                        if (cells_dif[i][j][k]==3)
	                        {
	                            cells_dif[i][j][k]=0;
	                            age_dif[i][j][k]=0;   
	                            inmature_osteoblasts_removed=inmature_osteoblasts_removed+1;                    
	                        }                                      
	                    }
	                }
	
	                while (mature_osteoblasts_removed<mature_osteoblasts_apoptosis)
	                {
			            r1=nrand(imax-imin+1); //r1=rand()%(imax-imin+1);
		                r2=nrand(jmax-jmin+1); //r2=rand()%(jmax-jmin+1);
		                r3=nrand(kmax-kmin+1); //r3=rand()%(kmax-kmin+1);
	                    i=imin+r1;
	                    j=jmin+r2;
	                    k=kmin+r3;
	                    if (lattice_point_element[i][j][k]==elem+1)
	                    {
	                        if (cells_dif[i][j][k]==2)
	                        {
	                            cells_dif[i][j][k]=0;
	                            age_dif[i][j][k]=0;   
	                            mature_osteoblasts_removed=mature_osteoblasts_removed+1;                    
	                        } 
	                    }
	            	} 
	         }//end for bone resorption
	        // system("PAUSE");
        //} // end else
    }// end for element
} 
