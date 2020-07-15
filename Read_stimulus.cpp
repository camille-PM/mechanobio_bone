#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <string>
#include "header.h"

using namespace std;

/***********************************************************************
	Reads Abaqus FEA output (.dat file) and computes the Lacroix & Prendergast 
	stimulus for each FE
***********************************************************************/

void Read_stimulus(float Stimulus_read[NUMBER_ELEMS],int elements_read[NUMBER_ELEMS][NODES_PER_ELEM])
{
    int elem;
    float flvelm_read;
    float Eoct_read;
   
    ifstream infile("SW12.dat");
	string temp;
 	string line;
 	float value; 
   
    infile>>temp;
    
    while (temp!="FLVELM")
    {
    	infile>>temp;
    }
    getline (infile,line);
    //cout<<"line: "<<line<<endl;
    
    getline (infile,line);
    //cout<<"line: "<<line<<endl;
    
    getline (infile,line);
    //cout<<"line: "<<line<<endl;

    infile>>temp; //first element
    //cout<<"element: "<<temp<<endl;

    float a,b,c,d,e,f;
    int j;
    
    for (elem=0;elem<NUMBER_ELEMS;elem++)
    {
        flvelm_read=0;
        Eoct_read=0;
        a=b=c=d=e=f=0;
        
        for (j=0;j<4;j++)
        {
            infile>>temp;  //node
            infile>>value;   //flvelm
            flvelm_read=value+flvelm_read;
            //cout<<"Fluid velocity: "<<flvelm_read<<endl;
            infile>>a;     //e11
            //cout<<"E11: "<<a<<endl;
            infile>>b;     //e22
            //cout<<"E22: "<<E[i][1]<<endl;
            infile>>c;     //e33
            //Eoct_read[i]=Eoct_read[i]+((sqrt((a-b)*(a-b)+(b-c)*(b-c)+(a-c)*(a-c)))/3);
            //cout<<"E33: "<<E[i][2]<<endl;
            infile>>d;
            //cout<<"E12: "<<E[i][3]<<endl;
            infile>>e; 
            //cout<<"E13: "<<E[i][4]<<endl;
            infile>>f;  
            //cout<<"E23: "<<E[i][5]<<endl;
            Eoct_read=Eoct_read+(sqrt((a-b)*(a-b)+(b-c)*(b-c)+(a-c)*(a-c)+6*d*d+6*e*e+6*f*f))*2/3;
            infile>>temp;  //next element      
            //system("PAUSE");
        }
        
        flvelm_read=flvelm_read/4;
        //Eoct_read[i]=(sqrt((a-b)*(a-b)+(b-c)*(b-c)+(a-c)*(a-c)+6*d*d+6*e*e+6*f*f))*2/3;
        Eoct_read=Eoct_read/4;
        //cout<<"Eoct:"<<Eoct_read<<endl;
        Stimulus_read[elem]=(Eoct_read/0.0375)+(flvelm_read/0.003);
        //cout<<"Stimulus: "<<Stimulus_read[elem]<<endl;
        //system("PAUSE");
        
  
    }
    
     
     
}
