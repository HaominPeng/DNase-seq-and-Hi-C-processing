#include "functions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

void Usage(){
    cout << "---------------------------------" << endl;	
    cout << "samprocess [option] [params]" << endl;
    cout << "[option] = separate, calculate, dataforttest, ttest" << endl;
    cout << "1. separate: [params] = [sam file name] [chromosome name] [chromosome to stop process] [rean length]" << endl;
    cout << "2. calculate: [params] = [txt file name] [step length] [window width]" << endl;
    
    cout << "3. trim-figure:[params]=[figure name][output name]"<<endl; 
    
    cout << "6. ptest-cut: [params] = [file 1 name] [file 2 name] [pillar-width] [p-value threshold] [window shift] [window width][min gap] [output file name for staring points] [output filename for ending points][points filename][distribution filename]" << endl;
    cout << "7. matrix: [params] = [starting point file name][ending point file name][figure file name] [step of figure][windowwidth of figure] [sample step][mingap] [matrix file name][sample filename][segment index filename]" << endl;

    cout << "8.addblock: [params] = [segment filename ][points file name][output block filename]"<< endl; 
    cout << "9.heatmap : [params] = [input file] [resolution] [max end target] [output file]" << endl;
    cout <<"10.hic : [params] = [input file] [resolution] [max end target] [output file]" << endl;
    cout <<"11.coordinatedescent:[params] = [matrix file][y-value vector] [lamda] [output filename]"<<endl;
   }
int main(int argc, char* argv[]){
    if (argc < 2) Usage();
//    else if (strcmp("separate", argv[1]) == 0){
//   	SeparateChrs(argc, argv);
//    }
    else if (strcmp("calculate", argv[1]) == 0){
    	CalculateReadNumber(argc, argv);
    }
    
/*    else if (strcmp("ptest-cut", argv[1]) == 0){
//    	PTest_Cut(argc, argv);
//    }
//    else if (strcmp("matrix", argv[1]) == 0){
//        Matrix(argc, argv);    
//    }
//    else if (strcmp("addblock", argv[1]) == 0){
//        Addblock(argc, argv);
//    }
//    else if (strcmp("heatmap",argv[1]) == 0){
        DrawHiC(argc,argv);
    }*/
    else if (strcmp("hic",argv[1])==0){
	Hic(argc,argv);
    }
//    else if (strcmp("coordinatedescent",argv[1])==0){
//	CoordinateDescent(argc,argv);
//    }
    else Usage();
    
    return 1;
}

