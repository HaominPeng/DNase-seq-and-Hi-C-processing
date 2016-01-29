#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;

void CalculateReadNumber(int argc, char* argv[]){
    if (argc != 5){
    	cout << "Error! Invalid Parameter Number in CalculateReadNumber()!" << endl;
	return;
    }

    string inputFileName = string(argv[2]);
    string strStep = string(argv[3]);
    string strWindowWidth = string(argv[4]);

    stringstream ss;
    ss << strWindowWidth;
    int windowWidth;
    ss >> windowWidth;

    stringstream ss2;
    ss2 << strStep;
    int step;
    ss2 >> step;
    
    stringstream ss3;
    ss3 << windowWidth;
    string prefix2;
    ss3 >> prefix2;

    stringstream ss4;
    ss4 << step;
    string prefix3;
    ss4 >> prefix3;
 

    
    string::size_type position = inputFileName.find(".");
    string prefix = inputFileName.substr(0, position);

    string outputFileName = prefix + "-" +prefix2 + "-" +prefix3 + "-figure.txt";

    ifstream input(inputFileName.c_str());
    ofstream output(outputFileName.c_str());

    string tmpLine;
    int numLine = 0;
    while(getline(input, tmpLine)){
	numLine++;
    }
cout<< numLine<<endl;
    input.close();
    input.open(inputFileName.c_str());
    
    int* startPos = new int[numLine];
    int* endPos = new int[numLine];
    int* interaction = new int[numLine];

    string ele1,ele4;
    int ele2,ele3,ele5; 
        
    
    int i = 0;
    while(getline(input,tmpLine)){;
	stringstream ss5;
	ss5 << tmpLine;
	ss5 >> ele1 >>ele2>>ele3>>ele4>>ele5;
	startPos[i] = ele2;
	endPos[i] = ele3; 
	interaction[i] = ele5;
	i++;
//cout << ele2<<" "<<ele3<<endl; 
//cout<<start<<" " <<end<<endl;  
//      cout<<startPos[i]<<" "<<endPos[i]<<" "<<interaction[i]<<endl;
    }

cout<<"input done"<<endl;

    int maxPos = endPos[numLine - 1];

cout << maxPos << endl;
    int intervalStartPos = 0;
    int numInterval = (maxPos) / step;
    
    
    

    int* count1 = new int[numInterval];
    
    for (int i = 0; i < numInterval; i++){
    	
	
	count1[i] = 0;

    }

    int count = 0;
    for (int n = 0; n < numLine; n++){
	if(startPos[n] >= 0 && startPos[n] <= windowWidth){
	    count++;
	}

	if (startPos[n] > windowWidth) break;
    }

     for (int n = 0; n < numLine; n++){
        if(endPos[n] >= 0 && endPos[n] <= windowWidth){
            count++;
        }

        if (endPos[n] > windowWidth) break;
    }

    for (int n = 0; n < numLine; n++){	
        int startstep = (startPos[n])/step;
	int endstep = (endPos[n])/step; 	
	
	for(int m = startstep; m < endstep; m++){
		count1[m] += interaction[n];
	}
		

    }
cout<<"count every 100 done"<<endl;
    int numWindow = (maxPos - windowWidth)/step + 1;
    int* countWindow = new int[numWindow];
    countWindow[0] = count;

    output << count << endl;
    for (int i = 1; i < numWindow; i++){
    	// interval = 100*i ~ 100*i + 100000
	countWindow[i] = countWindow[i-1];
	countWindow[i] -= count1[i-1];
	countWindow[i] += count1[i-1 + windowWidth/step];
	output << countWindow[i] << endl;
    }
}
