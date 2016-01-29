#ifndef _MATRIX_H
#define _MATRIX_H

#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;



void Matrix(int argc, char* argv[]){
    // matrix: [params] = [starting cutting-point file name ][ending cutting-point file name][y-value file name] [matrix output file name]"
    if (argc !=12){
        cout << "Error! Invalid Parameter Number in Matrix()!" << endl;
    }

    string file1name = string(argv[2]);
    string file2name = string(argv[3]);
    string file3name = string(argv[4]);
    string strStepOfFigure = string(argv[5]);
    string strWindowwidthOfFigure = string(argv[6]);
    string strSampleStep = string(argv[7]);
    string strMinGap = string(argv[8]);
    string outputname = string(argv[9]);
    string outputname2 = string(argv[10]);
    string outputname3 = string(argv[11]);
    int stepoffigure;
    int samplestep;
    int windowwidthoffigure;
    int mingap;
    stringstream ss1, ss2, ss3 ,ss4;
    ss1 << strStepOfFigure; ss1 >> stepoffigure;
    ss2 << strSampleStep;  ss2 >> samplestep;
    ss3 << strWindowwidthOfFigure; ss3>>windowwidthoffigure;
    ss4 << strMinGap; ss4>>mingap;
    // converting a string variable to an int variable

    ifstream file1(file1name.c_str());
    ifstream file2(file2name.c_str());
    ifstream file3(file3name.c_str());
    ofstream output(outputname.c_str());
    ofstream output2(outputname2.c_str());
    ofstream output3(outputname3.c_str());

    vector<int> startingpoints;
    vector<int> endingpoints;
    string tmpline;
//cout<<"0"<<endl;
    //each line contains the coordinate of a cutting-point
    while(getline(file1, tmpline)){
        int position;
        stringstream ss;
        ss << tmpline;
        ss >> position;
        startingpoints.push_back(position);    
    }

     while(getline(file2, tmpline)){
        int position;
        stringstream ss;
        ss << tmpline;
        ss >> position;
        endingpoints.push_back(position);
    }

    

    vector<int> yvalues;
    vector<int> xvalues;
    int countLines = 0;
    // each line contains the x-coordinates of a sampling point
    while(getline(file3, tmpline)){
        int yvalue;
        stringstream ss;
        ss << tmpline;
        ss >> yvalue;
	if (countLines % samplestep == 0){ 
	    xvalues.push_back(windowwidthoffigure + countLines * stepoffigure);
            output2 << yvalue << endl;
            yvalues.push_back(yvalue);
	}
	countLines++;
    }
    // the number of rows and columns of the parameter matrixx
   // int num_rows = xvalues.size();
    //int num_columns = (startingpoints.size()) * ((cuttingpoints.size());
    
    //bool** matrix = new bool*[num_rows];
    //for(int i = 0; i < num_rows; i++){
    //    matrix[i] = new bool[num_columns];
   // }

    //for(int i = 0; i < num_rows; i++){
    	//for(int k = 0; k < num_columns; k++){
		//matrix[i][k] = false;
//cout<<i<<","<<k<<endl;
//	}
//    }
    


    vector<int> segments_start;
    vector<int> segments_end;
    for(int i = 0; i < startingpoints.size() - 1; i++){
        for(int k = 0; k< endingpoints.size() - 1; k++){
	    if( endingpoints[k]-startingpoints[i]>= mingap ){
		
   		
		segments_start.push_back(startingpoints[i]);
                segments_end.push_back(endingpoints[k]);
	    }
	}
    }


    for(int i =0; i<segments_start.size();i++){
	output3 << segments_start[i]<<" "<<segments_end[i]<<endl;
    }
   	
    // the number of rows and columns of the parameter matrixx
    int num_rows = xvalues.size();
    int num_columns = segments_start.size();
    
    bool** matrix = new bool*[num_rows];
    for(int i = 0; i < num_rows; i++){
	matrix[i] = new bool[num_columns];
    }
    for(int i = 0; i < num_rows; i++){
	for(int k = 0; k < num_columns; k++){
           matrix[i][k] = false;
        }
    }
    
    int index = 0;
    
    for(int i = 0; i < segments_start.size() - 1; i++){
       
        for(int n = 0; n < xvalues.size(); n++){
            if(xvalues[n] >= segments_start[i] && xvalues[n] <= segments_end[i]) {
                  //  cout << "n = " << n << ", i = " << i << ", k =" << k << endl;
		  //  cout << "xvalues[n] = " << xvalues[n] << ", span = " << cuttingpoints[i] << ", " << cuttingpoints[k] << endl;
		  //  cout << "index = " << index << endl;
            matrix[n][i] = true;    
            }
        
            if(xvalues[n] > segments_end[i])break;
         }
    }   
          
       
    
    
    for(int i = 0; i < num_rows; i++){
        for(int k = 0; k < num_columns - 1; k++){
            output << (matrix[i][k] == true? "1" : "0") << " ";   
        }
        output << (matrix[i][num_columns-1] == true ? "1" : "0") << endl;
    }
    
}

#endif
