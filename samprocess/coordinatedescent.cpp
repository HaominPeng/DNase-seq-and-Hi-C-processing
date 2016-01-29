#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <time.h>
#include <math.h>
using namespace std;

// matrix * vector
void multiply(vector<vector<int> >& matrix, int rownum, int columnnum, vector<int>& vec, int length, int* result);

// sort the x vectors by their innerproduct with y
void selectsort(vector<double>& innerproduct, vector<vector<int> >& x, vector<int>& beta, vector<vector<int> >& matrix);

// the loss function
double getloss(vector<vector<int> >& matrix, vector<int>& y, vector<int>& beta, double lamda);

// return the beta_i such that loss function achieves the extremepoint

double optimalbeta(vector<vector<int> >& x, vector<int>& y, vector<int>& beta, double lamda, int index);

// coordinate descent
void CoordinateDescent(int argc, char* argv[]){
    if (argc != 6){
	cout << "Error! Invalid Parameter Number in CoordinateDescent()!" << endl; 	
    }

    clock_t timestart, timestop;
    string strMatrixFileName = string(argv[2]);
    string strYFileName = string(argv[3]);
    string strLamda = string(argv[4]);
    string strOutputName = string(argv[5]);
    
    double lamda; // the coefficient of |beta| in the loss function
    stringstream s1;
    s1 << strLamda;
    s1 >> lamda;

    ifstream matrixFile(strMatrixFileName.c_str());
    ifstream yfile(strYFileName.c_str());
    ofstream output(strOutputName.c_str());
    string tmpLine;
    // the parameter matrix (since the size is undetermined)
    vector< vector<int> > matrix;
    // the y-value vector
    vector<int> y;

    stringstream ss;
    int tmpValue;
    vector<int> tmpVector;

    ///////////////////////////////////////
    ////  1. read the parameter matrix
    ///////////////////////////////////////
    cout << "start to reading the matrix file..." << endl;
    timestart = clock();

    // dealing with the 1st row
    getline(matrixFile, tmpLine);

    // get the column number of the matrix
    int numColumns = (tmpLine.length() + 1)/2;
    int numRow = 1;
    ss << tmpLine;
    for (int i = 0; i < numColumns; i++){
	ss >> tmpValue;
	tmpVector.push_back(tmpValue);
    }
    matrix.push_back(tmpVector);
    tmpVector.clear();

    // dealing with the remaining rows
    while(getline(matrixFile, tmpLine)){
	numRow++;
	stringstream sss;
	sss << tmpLine;
	for (int i = 0; i < numColumns; i++){
	    sss >> tmpValue;
	    tmpVector.push_back(tmpValue);
	}
	matrix.push_back(tmpVector);
	tmpVector.clear();
    }

    timestop = clock();
    cout << "reading matrix file completed in " << (timestop - timestart)/1000 << " ms" << endl;


    ////////////////////////////////////
    // 2. read the y-value file
    ////////////////////////////////////
    cout << "start to reading the y-value vector file..." << endl;
    timestart = clock();

    while(getline(yfile, tmpLine)){
	stringstream sss;
	sss << tmpLine;
	int tmp;
	sss >> tmp;;
	y.push_back(tmp);
    }
    cout << "y[0] = " << y[0] << endl;

    timestop = clock();
    cout << "reading y-value vector file completed in " << (timestop - timestart)/1000 << " ms" << endl;

    cout << "-- statistics: " << endl;
    cout << "numRow = " << numRow << endl;
    cout << "numColumns = " << numColumns << endl;
    cout << "y-value vector length = " << y.size() << endl;

    /////////////////////////////////////
    ///   3. initialize the beta vector
    ////////////////////////////////////
    vector<int>  beta;
    for (int i = 0; i < numColumns; i++){
	beta.push_back(i);
    }

    /////////////////////////////////////////////
    ///  additional: test the multiply function
    /////////////////////////////////////////////
    /*int* tmpY = new int[numRow];
    // tmpY = matrix * beta
    multiply(matrix, matrix.size(), matrix[0].size(), beta, beta.size(), tmpY);

    for (int i = 0; i < matrix.size(); i++){
    output << tmpY[i] << endl;
    }*/

    ////////////////////////////////////////////////////
    /// 4. get the columns of the parameter matrix
    ///////////////////////////////////////////////////
    vector<vector<int> > x;
    vector<int> tmpX;
    for(int i = 0; i < matrix[0].size(); i++){
	for (int k = 0; k < matrix.size(); k++){
	    tmpX.push_back(matrix[k][i]);
	}
	x.push_back(tmpX);
	tmpX.clear();
    }

    ////////////////////////////////////////////////////
    //  5. sort the columns by the normalized inner product
    ////////////////////////////////////////////////////
    // y * x / |x| 
    vector<double> innerproduct;
    vector<int> order;
    double norm ;
    double product;
    for(int i = 0; i < x.size(); i++){
	norm = 0;
	product = 0;
	for (int k = 0; k < x[i].size(); k++){
	    norm += x[i][k];
	    product += x[i][k] * y[k];
	}
	norm = sqrt(norm);
	innerproduct.push_back(product/norm);
    }
    selectsort(innerproduct, x, beta, matrix);

  
    
    
    /*cout << "--- new matrix (from x):" << endl;
    for(int i = 0; i < x[0].size(); i++){
    	for (int k = 0; k < x.size(); k++){
		cout << x[k][i] << " ";
	}
	cout << endl;
    }*/

   
    for (int i = 0; i < beta.size(); i++){
    	order.push_back(beta[i]);
    }
    
    for(int i =0; i < beta.size(); i++){
	beta[i] = 0;
    }

    // test
    /*cout << "after sort...." << endl;
    for (int i = 0; i < innerproduct.size(); i++) cout << innerproduct[i] << "\t";
    cout << endl;*/

    ////////////////////////////////////////////////////////
    //  6. start coordinate descent
    ////////////////////////////////////////////////////////

    double loss = getloss(matrix, y, beta, lamda);
    
    cout << "initial loss = " << loss << endl;

    //beta[0] = optimalbeta(x, y, beta, lamda, 0);
    //cout << "beta[0] = " << beta[0] << endl;
    int newloss = 0; 
    int ii = 0;
    while(1){
	ii = ii++;
	cout << "iteration " << ii << endl;
    	for(int i = 0; i < numColumns; i++){
	    // for each vector x_i
	    // fix the others, find the optimal beta_i
	    beta[i] = optimalbeta(x, y, beta, lamda, i);
	   // loss = newloss;
	   // newloss = getloss(matrix, y, beta, lamda);
	}

	newloss = getloss(matrix,y,beta,lamda);
	if (newloss >= loss || (loss-newloss)/loss < 0.0001) break;
        loss = newloss;	
    }
    
    int size = beta.size();
    int reorderedbeta[size];
    cout << "loss = " << loss << endl;
    for(int i = 0; i < beta.size(); i++){
	reorderedbeta[order[i]] = beta[i];
    } 

    for(int i = 0; i < beta.size(); i++){
    	output << reorderedbeta[i] << endl;
    }
}

// matrix * vector
void multiply(vector<vector<int> >& matrix1, int rownum1, int columnnum1, vector<int>& vec, int length, int* result){
    // check the legality
    if (columnnum1 != length){
	cout << "Error using matrix multiplication!" << endl;
	return;
    }

    // assuming that spaces of 'result' have already been allocated
    // i.e. result = int[rownum1]
    for (int i = 0; i < rownum1; i++){
	result[i] = 0;
	for (int m = 0; m < columnnum1; m++){
	    result[i] += matrix1[i][m] * vec[m];
	}
    }
}

// sort the x vectors by their innerproduct with y
void selectsort(vector<double>& innerproduct, vector<vector<int> >& x, vector<int>& beta, vector<vector<int> >& matrix){
    // check legality, sizes should all equal to column number

    if(innerproduct.size()!=x.size() || innerproduct.size() != beta.size()){cout << "Error in selectsort()!" << endl;}

    double tmpMax = 0;
    int tmpIndex = -1;

    for(int i = 0; i < innerproduct.size() - 1; i++){
	tmpMax = innerproduct[i];
	tmpIndex = i;
	for (int k = i + 1; k < innerproduct.size(); k++){
	    if (innerproduct[k] > tmpMax){
		tmpMax = innerproduct[k];
		tmpIndex = k;
	    }
	}

	// exchange the max term and the i-th term
	innerproduct[tmpIndex] = innerproduct[i];
	innerproduct[i] = tmpMax;

	// exchange the corresponding column and beta value
	for (int n = 0; n < x[i].size(); n++){
	    int tmp = x[i][n];
	    x[i][n] = x[tmpIndex][n];
	    matrix[n][i] = matrix[n][tmpIndex];
	    x[tmpIndex][n] = tmp;
	    matrix[n][tmpIndex] = tmp;
	}

	int tmp = beta[i];
	beta[i] = beta[tmpIndex];
	beta[tmpIndex] = tmp;
    }
}




// the loss function
double getloss(vector<vector<int> >& matrix, vector<int>& y, vector<int>& beta, double lamda){
    // loss function = sum of (y_i - sum of x_ji*beta_j)^2 + lamda*(|beta1| + |beta2| + ... + |betaN|)
    double loss = 0;
    double sum1 = 0; // sum1 = (y_i - sum of x_ji*beta_j)^2
    double sum2 = 0; // sum2 = sum of all sum1

    // note that, matrix[i] is the i-th row of the parameter matrix
    // x[i] is the i-th column of the parameter matrix

    // for each row in the parameter matrix
    for (int i = 0; i < matrix.size();i++){
	sum1 = y[i];
	// for each column, calculate the sum
	for (int k = 0; k < matrix[i].size(); k++){
	    sum1 -= matrix[i][k]*beta[k];
	}
	sum1 = sum1 * sum1;
	sum2 += sum1;
    }

    loss = sum2;
    for (int i = 0; i < beta.size(); i++){
	loss += lamda * fabs(beta[i]);
    }

    return loss;
}


// return the beta_i such that loss function achieves the extremepoint

double optimalbeta(vector<vector<int> >& x, vector<int>& y, vector<int>& beta, double lamda, int k){
    // check legality
    if (x.size() != beta.size() || x[0].size() != y.size()){
    	cout << "Error in optimalbeta()!" << endl;
	cout << "x.size() = " << x.size() << endl;
	cout << "beta.size() = " << beta.size() << endl;
	cout << "x[0].size() = " << x[0].size() << endl;
	cout << "y.size() = " << y.size() << endl;
    }
    
    
    int denominator = 0;

    // note that, lamda is not considered in numerator in the following loop
    int numerator = 0;

    for(int i = 0; i < x[0].size(); i++){
	//actually x[j][i] = 0 or 1, square is meaningless
	denominator += x[k][i] * x[k][i];
	int tmpSum = 0;
	for (int j = 0; j < x.size(); j++){
		if (j == k) continue;
		tmpSum += x[j][i] * beta[j];
	}
	tmpSum = y[i] - tmpSum;
	numerator += x[k][i] * tmpSum;
    }

    double beta_n, beta_p;// two possible solutions

    beta_n = (double)((double)numerator + lamda)/((double)denominator);
    beta_p = (double)((double)numerator - lamda)/((double)denominator);

    /*if (beta_p < 0 && beta_n > 0){
    	//cout << "Error! No valid solution for optimal beta!" << endl;
	return beta[k];
    }
    else if (beta_p > 0) return beta_p;
    else return beta_n;*/
    if (beta_p > 0) return beta_p;
    else return beta[k];
}
