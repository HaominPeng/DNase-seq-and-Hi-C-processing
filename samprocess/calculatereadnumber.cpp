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

    input.close();
    input.open(inputFileName.c_str());

    int* startPos = new int[numLine];
    int* endPos = new int[numLine];
    
    for(int i = 0; i < numLine; i++){
	input >> startPos[i] >> endPos[i];
    }

    int maxPos = endPos[numLine - 1];

    int intervalStartPos = 0;
    int numInterval = (maxPos) / step;
    
    int* countEndIn = new int[numInterval];
    int* countStartIn = new int[numInterval];
    int* countFullIn = new int[numInterval];
    for (int i = 0; i < numInterval; i++){
    	countEndIn[i] = 0;
	countStartIn[i] = 0;
	countFullIn[i] = 0;
    }

    int count = 0;
    for (int n = 0; n < numLine; n++){
	if((startPos[n] >= 0 && startPos[n] <= windowWidth)
		|| (endPos[n] >= 0 && endPos[n] <= windowWidth)){
	    count++;
	}

	if (startPos[n] > windowWidth) break;
    }

    for (int n = 0; n < numLine; n++){	
    	if (startPos[n]/step == endPos[n]/step){
		countFullIn[startPos[n]/step] += 1;
	}
	else{
		countStartIn[startPos[n]/step] += 1;
		countEndIn[endPos[n]/step] += 1;
	}
    }

    int numWindow = (maxPos - windowWidth)/step + 1;
    int* countWindow = new int[numWindow];
    countWindow[0] = count;

    output << count << endl;
    for (int i = 1; i < numWindow; i++){
    	// interval = 100*i ~ 100*i + 100000
	countWindow[i] = countWindow[i-1];
	countWindow[i] -= countFullIn[i-1];
	countWindow[i] -= countEndIn[i-1];
	countWindow[i] += countStartIn[i + windowWidth/step];
	countWindow[i] += countFullIn[i + windowWidth/step];
	output << countWindow[i] << endl;
    }
}
