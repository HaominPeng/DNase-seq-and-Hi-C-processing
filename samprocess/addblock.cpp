#ifndef _ADDBLOCK_H
#define _ADDBLOCK_H

#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;



void Addblock(int argc, char* argv[]){
    // matrix: [params] = [segment filename ][points file name][beta filename][output block filename]
    if (argc !=5){
     cout << "Error! Invalid Parameter Number in Matrix()!" << endl;
                     }
 
        string file1name = string(argv[2]);
        string file2name = string(argv[3]);
//	string file3name = string(argv[4]);
     string outputname = string(argv[4]);
     
	ifstream file1(file1name.c_str());
	ifstream file2(file2name.c_str());
	//ifstream file3(file3name.c_str());
	ofstream output(outputname.c_str());

	vector<int> segment_start;
	vector<int> segment_end;
	vector<double> segment_height;
	vector<int> points;
	vector<double> block_height;
	string tmpline;
        int ele1,ele2,ele3;
	//double ele3;
	int position;

	while(getline(file1,tmpline)){
	stringstream ss;
	ss << tmpline;
	ss >> ele1 >>ele2>>ele3 ;
	segment_start.push_back(ele1);
	segment_end.push_back(ele2);
	 segment_height.push_back(ele3);	
	}	


	while(getline(file2,tmpline)){
	stringstream ss;
	ss << tmpline;
	ss >> position;
	points.push_back(position);
	}

//	while(getline(file3,tmpline)){
 //       stringstream ss;
 //       ss << tmpline;
  //      ss >> ele3;
  //      segment_height.push_back(ele3);
   //     }
 
	int i,k;
		
	for( i = 0 ; i < points.size()-1 ; i++){
	block_height.push_back(0);
	}

	for (i = 0; i < points.size()-1; i++){
	for (k = 0; k < segment_start.size() ; k++){
	if(segment_start[k]<=points[i]&&segment_end[k]>=points[i+1]){
		
	block_height[i] = block_height[i] + segment_height[k];
	}
	
	if (segment_start[k] >= points[i+1]) break;
	}
	}


        for ( i = 0 ; i < block_height.size() ; i++){
	output <<points[i]<<" "<<points[i+1]<<" "<<block_height[i]<<endl;
	}

}

#endif











      
