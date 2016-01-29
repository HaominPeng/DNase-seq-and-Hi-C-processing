#include "functions.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>
using namespace std;

void TTest(int argc, char* argv[]){
    if (argc != 6){
	cout << "Error! Invalid Parameter Number in TTest()!" << endl;
    }

    string sample1name = string(argv[2]);
    string sample2name = string(argv[3]);
    string strAlpha = string(argv[4]);
    string outputname = string(argv[5]);

    stringstream ss;
    ss << strAlpha;
    double alpha;
    ss >> alpha;

    // two file, n = 2, v = n - 1 = 1
    map<double,double> Tdistribution;
    Tdistribution.insert(make_pair(0.001,636.6));
    Tdistribution.insert(make_pair(0.002,318.3));
    Tdistribution.insert(make_pair(0.005,127.3));
    Tdistribution.insert(make_pair(0.01,63.66));
    Tdistribution.insert(make_pair(0.02,31.82));
    Tdistribution.insert(make_pair(0.05,12.71));
    Tdistribution.insert(make_pair(0.1,6.314));
    Tdistribution.insert(make_pair(0.2,3.078));
    Tdistribution.insert(make_pair(0.5,1.0));

    map<double, double>::iterator iter;
    iter = Tdistribution.find(alpha);
    double threshold = iter->second;

    ifstream sample1file(sample1name.c_str());
    ifstream sample2file(sample2name.c_str());
    ofstream output(outputname.c_str());

    string tmpline1, tmpline2;
    double sample1, sample2;
    double lastsample1, lastsample2;

    getline(sample1file, tmpline1);
    getline(sample2file, tmpline2);
    stringstream ss1, ss2;
    ss1 << tmpline1; ss2 << tmpline2;
    ss1 >> lastsample1; ss2 >> lastsample2;

    while(getline(sample1file, tmpline1) && getline(sample2file, tmpline2)){
	double sample1, sample2;
	stringstream ss1, ss2;
	ss1 << tmpline1; ss2 << tmpline2;
	ss1 >> sample1; ss2 >> sample2;
	double d1 = sample1 - lastsample1;
	double d2 = sample2 - lastsample2;
	double d = (d1 + d2) / 2;
	double sd = sqrt(((d1-d)*(d1-d)+(d2-d)*(d2-d))/2);
	double se = sd / 1.414;
	double t = abs(d / se);
	if (t > threshold) output << "1" << endl;
	else output << "0" << endl;
    }
}
