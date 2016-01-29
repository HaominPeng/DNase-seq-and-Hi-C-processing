#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;


void DrawHiC(int argc, char* argv[]){
	if (argc != 6){
		cout << "Error! Invalid Parameter Number in DrawHiC()!" << endl;
	}
	    
	string inputfilename = string(argv[2]);
	string strResolution = string(argv[3]);
	string strMaxEndTarget = string(argv[4]);
	string outputfilename = string(argv[5]);

	int resolution;
	int maxendtarget;
	int matrixsize;
	int** matrix;// output for draw the heat map

	stringstream ss1; ss1 << strResolution; ss1 >> resolution;
	stringstream ss2; ss2 << strMaxEndTarget; ss2 >> maxendtarget;

	// get the size of the output matrix
	matrixsize = ceil(((double)maxendtarget)/((double)resolution));
	cout << "Matrix size = " << matrixsize << endl;

	// initialize the matrix
	matrix = new int*[matrixsize];
	for(int i = 0; i < matrixsize; i++){
		matrix[i] = new int[matrixsize];
		for (int k = 0; k < matrixsize; k++){
			matrix[i][k] = 0;
		}
	}

	// read file
	ifstream input(inputfilename.c_str());

	ofstream output(outputfilename.c_str());

	// the SuppData file contains 6 elements in each line
	// ele1 = chromosome name
	// ele2 = target start position
	// ele3 = target end position
	// ele4 = Anchor ID
	// ele5 = contact frequency
	string tmpline, ele1 , ele4;
        int ele2, ele3, ele5;

	while(getline(input, tmpline)){
		stringstream ss;
		ss << tmpline;
		ss >> ele1 >> ele2 >> ele3 ;

		// start position, end position, and the contact frequency
		int start, end, freq;

		stringstream ss1, ss2, ss3;
		ss1 << ele1; ss1 >> start;
		ss2 << ele2; ss2 >> end;
		ss3 << ele3; ss3 >> freq;

		// the start index in the matrix
		start = floor(((double)start)/((double)resolution));
		start < 0 ? start = 0: start = start;// to avoid array out of bounds

		// the end index in the matrix
		end = ceil(((double)end)/((double)resolution));
		end > matrixsize ? end = matrixsize : end = end;// to avoid array out of bounds

		// add freq to the given area in the matrix
		for (int i = start; i < end; i++){
			for (int k = start; k < end; k++){
				matrix[i][k] += freq;
			}
		}
	}

	for (int i = 0; i < matrixsize; i++){
		for(int k = 0; k < matrixsize; k++){
			output << matrix[i][k] << " ";
		}
		output << endl;
	}

	input.close();
	output.close();
}

