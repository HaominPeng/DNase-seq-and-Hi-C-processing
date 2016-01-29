#ifndef _P_TEST_H
#define _P_TEST_H

#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
	  
void PTest_Cut(int argc, char* argv[]){
    if (argc != 13){
	cout << "Error! Invalid Parameter Number in PTest_Distribution()!" << endl;
    }

    string file1name = string(argv[2]);
    string file2name = string(argv[3]);
    string strColumnWidth = string(argv[4]);
    string strThreshold = string(argv[5]);
    string strStep = string(argv[6]);
    string strGap = string(argv[8]);
    string strWindowWidth = string(argv[7]);
    string outputname1 = string(argv[9]);
    string outputname2 = string(argv[10]);
    string outputname3 = string(argv[11]);
    string outputname4 = string(argv[12]);    

    int columnWidth; // the width of each ocolumn in the histogram, typical value is 10 or 100
    double threshold; // the threshold of p-value
    int step; // the step of window shift, typical value is 100
    int windowwidth; // the width of window for cutting point prediction, typical value is 1M
    int mingap; // 
    // converting string variables to double/int variable
    stringstream ss; 	ss << strColumnWidth; 		ss >> columnWidth;
    stringstream sss; 	sss << strThreshold; 		sss >> threshold;
    stringstream ssss; 	ssss << strStep; 		ssss >> step;
    stringstream sssss; sssss << strWindowWidth;	sssss >> windowwidth;
    stringstream s5; s5<<strGap; s5>>mingap;

    ifstream file1(file1name.c_str());
    ifstream file2(file2name.c_str());
    ofstream output1(outputname1.c_str());
    ofstream output2(outputname2.c_str());
    ofstream output3(outputname3.c_str());
    ofstream output4(outputname4.c_str());
    
     // store all sample values
    vector<int> allsample1;
    vector<int> allsample2;
    string tmpline1,tmpline2;
 
    while(getline(file1, tmpline1) && getline(file2, tmpline2)){
        stringstream ss1, ss2;
        int sample1, sample2;
        ss1 << tmpline1; ss2 << tmpline2;
        ss1 >> sample1; ss2 >> sample2;

        allsample1.push_back(sample1);
        allsample2.push_back(sample2);
    }
   
    int stepInWindow = windowwidth/100;
    int stepIndex = step/100;
    if (stepIndex < 1) stepIndex = 1;


    double minValue = 10000, maxValue = 0;
    double permutation1, permutation2, permutation3, permutation4, permutation5, permutation6;
    vector<double> allValues;

    for(int i = 0; i < allsample1.size() - stepInWindow; i+=stepIndex){

	// 6 kinds of permutations
	permutation1 = ((allsample1[i]+allsample1[i+stepInWindow]) - (allsample2[i]+allsample2[i+stepInWindow]))/2;
	permutation2 = ((allsample1[i+stepInWindow]+allsample2[i+stepInWindow]) - (allsample1[i]+allsample2[i]))/2;
	permutation3 = ((allsample1[i+stepInWindow]+allsample2[i]) - (allsample1[i]+allsample2[i+stepInWindow]))/2;
	permutation4 = -permutation3;
	permutation5 = -permutation2;
	permutation6 = -permutation1;

	allValues.push_back(permutation1);
	allValues.push_back(permutation2);
	allValues.push_back(permutation3);
	allValues.push_back(permutation4);
	allValues.push_back(permutation5);
	allValues.push_back(permutation6);

	if (permutation1 > maxValue) maxValue = permutation1;	
	if (permutation2 > maxValue) maxValue = permutation2;	
	if (permutation3 > maxValue) maxValue = permutation3;	
	if (permutation4 > maxValue) maxValue = permutation4;	
	if (permutation5 > maxValue) maxValue = permutation5;	
	if (permutation6 > maxValue) maxValue = permutation6;

	if (permutation1 < minValue) minValue = permutation1;	
	if (permutation2 < minValue) minValue = permutation2;	
	if (permutation3 < minValue) minValue = permutation3;	
	if (permutation4 < minValue) minValue = permutation4;	
	if (permutation5 < minValue) minValue = permutation5;	
	if (permutation6 < minValue) minValue = permutation6;	

	

    }

    cout << "minValue = " << minValue << endl;
    cout << "maxVALUE = " << maxValue << endl;
    int numColumns = 2* (int)(maxValue/columnWidth) + 2;
    cout << "numColumns = " << numColumns << endl;

    int* stat = new int[numColumns];
    for (int i = 0; i < numColumns; i++) stat[i] = 0;
    // 0 ~ (numColumns/2-1) ---- negative part
    // (numColumns/2) ~ (numColumns-1) ---- positive part
    for(int i = 0; i < allValues.size(); i++){
	int index = (int)(allValues[i]/columnWidth) + (numColumns/2);
	stat[index]++;
    }


    int total = allValues.size();
    for (int i = 0; i < numColumns; i++){
    
	output4 << stat[i] << endl;
    }
    cout << "total = " << total << endl;




    int indexThreshold = 0;
    double total_copy = (double)total;
    // calcute the typical p-value when probability is 0.01 and 0.05
    double integration = (double)stat[numColumns-1];

    cout << "threshold = " << threshold << endl;
    for (int i = numColumns - 2; i >= 0; i--){
    	if (integration < threshold*total_copy && integration+(double)stat[i] >= threshold*total_copy){
		indexThreshold = i;
		break;
	}
	integration += (double)stat[i];
    }

    cout << "index of threshold value = " << indexThreshold << endl;
    // move the pointers back to the head of files
   

    

    int count1 = 0;
    int count2 = 0;
    int count = 0;
    
    if (stepIndex < 1) stepIndex = 1;

    vector<double> alldif;
    vector<int> pos;
    vector<int> postemp;

    for (int i = 0; i < allsample1.size() - stepInWindow; i+=stepIndex){
    	double mean_old = (allsample1[i] + allsample2[i])/2;
	double mean_new = (allsample1[i + stepInWindow] + allsample2[i + stepInWindow])/2;

	double dif = mean_new - mean_old;

	if (dif < 0) {
		dif = -dif;
		int index = (int)(dif/columnWidth) + (numColumns/2);
		if (index > indexThreshold){
			alldif.push_back(-dif);
			pos.push_back(windowwidth+i*100);
			output3 << windowwidth+i*100 << endl;
//	    		output2 << windowwidth+i*100 << endl;
	    		count ++;
		}
    	}
	else{
        	int index = (int)(dif/columnWidth) + (numColumns/2);
                if (index > indexThreshold){
			alldif.push_back(dif);
			pos.push_back(windowwidth+i*100);
			output3 << windowwidth+i*100 << endl;
//                      output1 << windowwidth+i*100 << endl;
                       count ++;
		}
	    }
        
    } 

   cout << "total raw points=" << count << endl;

   postemp.push_back(pos[0]);
   int startindex = 0;
   for (int i = 1; i<pos.size();i++){

	if( pos[i] - postemp[postemp.size()-1] >= mingap){
	int startdif = alldif[startindex];
	int enddif = alldif[startindex+postemp.size()-1];
	if(startdif>0){output1 << pos[startindex]<<endl ; count1++;}

 	else {output2 << pos[startindex]<<endl; count2++;}

	if(enddif>0) {output1 << pos[startindex+postemp.size()-1]<<endl ; count1++;}
 	else {output2 << pos[startindex+postemp.size()-1]<<endl; count2++;}
	
        postemp.clear();
	postemp.push_back(pos[i]);
	startindex = i; 

	}
	
	else postemp.push_back(pos[i]);
   }
     int startdif = alldif[startindex];
    int enddif = alldif[startindex+postemp.size()-1];
        if(startdif>0){output1 << pos[startindex] << endl ; count1++;}

        else {output2 << pos[startindex] << endl; count2++;}

        if(enddif>0) {output1 << pos[startindex+postemp.size()-1] <<endl; count1++;}
        else {output2 << pos[startindex+postemp.size()-1] << endl; count2++;}
                    
        cout << "total staring points= " << count1 << endl;
        cout << "total ending points= " << count2 << endl; 	


}

#endif

