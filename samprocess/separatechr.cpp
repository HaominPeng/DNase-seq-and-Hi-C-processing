#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;

// output the read start-position for the selected chromosome
// read trun the number of total reads in this chromosome
void SeparateChrs(int argc, char* argv[]){

    if (argc != 6){
	cout << "Error! Invalid Paramter Number in SeparateChrs()!" << endl;
	return;
    }

    string samFileName = string(argv[2]);
    string ChrName = string(argv[3]);
    string StopChrName = string(argv[4]);
    string strReadLength = string(argv[5]);

    stringstream ss;
    ss << strReadLength;
    int readLength;
    ss >> readLength;

    string::size_type position = samFileName.find(".");
    string prefix = samFileName.substr(0, position);
    string outputFileName = prefix + "-" + ChrName + ".txt";
    string tmpLine;
    string ele1, ele2, ele3, ele4;
    int startPosition, endPosition, number;

    ifstream input(samFileName.c_str());
    ofstream output(outputFileName.c_str());

    while(getline(input, tmpLine)){
    	stringstream ss1; 
	ss1 << tmpLine;
	ss1 >> ele1 >> ele2 >> ele3 >> ele4;

	if (ele3 == ChrName){
	    stringstream ss2;
	    ss2 << ele4;
	    ss2 >> startPosition;
	    endPosition = startPosition + readLength - 1;
	    output << startPosition << "\t" << endPosition << endl;
	}

	if (ele3 == StopChrName) break;
    }

    input.close();
    output.close();

}

