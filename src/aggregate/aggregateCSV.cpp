/*
	Aggregates data in a .csv file.
	===============================
	
	*Input:
	./aggregateCSV <char*> filepath <int> aggregateOn
	
		filepath - path to .csv file storing x,y csv pairs.
			File structure:
				Pair of doubles or ints or combination per line
				eg:	36,1.6144e-06
					36,1.3968e-06
					36,1.4526e-06
					36,1.4646e-06
					36,2.6592e-06
		
		aggregateOn - Index of number to aggregate on (currently only support 0=x or 1=y)
	
	*Output:
	<int/double> aggregatedX, <int/double> Y
OR	<int/double> X, <int/double> aggregatedY

		eg:	36,2.22556e-06
			41,4.94345e-06
			46,1.73232e-05
			51,0.00011239
			56,0.00201846
	
	*Limitations:
	As indicated, currently only works for 2-tuples.

*/

#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <string>
#include <omp.h>
#include <vector>

using namespace std;

typedef struct{
	double value;
	double sum;
	int count;
}distinct;

//Extract data
double** getData(char* file, int numLines);
	int getNumLines(char* fileName);
	double* extract2DDatapoint(string line, char separator);

int findDistinct(vector<distinct> distincts, double value);
vector<distinct> aggregate(int position, double** xyValuesIn, int numLines);

int main (int argc, char *argv[]){
	int numLines = getNumLines(argv[1]);
	
	double** xyValues = getData(argv[1], numLines);
	
	vector<distinct> aggregated = aggregate(strtol(argv[2], NULL, 10), xyValues, numLines);	//0=x 1=y
	
	for(int i = 0; i < aggregated.size(); i++){
		cout << aggregated[i].value << ",";
		cout << aggregated[i].sum/aggregated[i].count << endl;
	}
}

vector<distinct> aggregate(int position, double** xyValuesIn, int numLines){
	vector<distinct> distincts;

	for(int i = 0; i < numLines; i++){
		int index = findDistinct(distincts, xyValuesIn[i][0]);

		if(index != -1){
			distincts[index].sum += xyValuesIn[i][1];
			distincts[index].count++;
		}
		else{
			distinct newDistinct;

			newDistinct.value = xyValuesIn[i][0];
			newDistinct.sum = xyValuesIn[i][1];
			newDistinct.count = 1;
			distincts.push_back(newDistinct);
		}
	}

	return distincts;
}

int findDistinct(vector<distinct> distincts, double value){
	for(int i = 0; i < distincts.size(); i++){
		if(distincts[i].value == value) return i;
	}
	
	return -1;
}

double** getData(char* file, int numLines){
	double** xyValues;
	
	xyValues = (double**) malloc(sizeof(double*)*numLines);	
	for(int i = 0; i < numLines; i++){
		xyValues[i] = (double*) malloc(sizeof(int)*2);
	}

	ifstream dataFile(file);

	string line;

	int index = 0;

	if(dataFile.is_open()){
		while(getline(dataFile, line)){
			xyValues[index] = extract2DDatapoint(line, ',');
			index++;
		}
	}

	return xyValues;
}

double* extract2DDatapoint(string line, char separator){
	double* datapoint = (double*) malloc(sizeof(double)*2);
	string currSubstring = line;
	int currPosition;
	int index = 0;

	while(index != 2){
		currPosition = currSubstring.find(separator);
		datapoint[index] = strtod(currSubstring.substr(0, currPosition).c_str(), NULL);
		index++;
		currSubstring = currSubstring.substr(currPosition+1);
	}

	return datapoint;
}

int getNumLines(char* fileName){
	ifstream dataFile(fileName);
	string line;
	int numLines = 0;

	if(dataFile.is_open()){
		while(getline(dataFile, line)){
			numLines++;
		}
	}

	dataFile.close();

	return numLines;
}
