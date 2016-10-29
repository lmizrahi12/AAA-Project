/*
	Implements the backtracking algorithm.
	======================================

	*Input:
	./backtracking <char*> filepath <char> metric
	
		filepath - path to .txt file storing incomplete sudokus.
			File Structure:
				One sudoku per line (81 integers per line)

				eg:	400759008780010094000460500047825931018000005059000820194576200500003149803100056
					054270003012050090709381000503192086000840000281706940100060032036420018028013009
					023586410090743265050021307900610020030209608006800701000102000082497036007008052
					694000100800000034003490860318650200020708600706129080460907358080360002035042096
					307104208025390104009780563008009650954017802073820000580003020006001005002068410
	
		metric - 'E', 'D' or 'V' (E for numEmpty, D for numEmpty/maxFilled) higher=harder V for verification of algorithm
	
	*Output:
	<int> numEmpty,<double> timeTaken(s)
OR	<double> difficulty,<double> timeTaken(s)
	
		eg:	36,1.6144e-06
			36,1.3968e-06
			36,1.4526e-06
			36,1.4646e-06
			36,2.6592e-06

	 
*/

#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <string>
#include <omp.h>

using namespace std;

int NUM_IN_FILE = 0;	//Number of sudokus in file (Should be changed to local...)
const int NUM_RUNS = 10;	//Number of refinements per sudoku

//Utility
int** getGrid(char* file);
int** copySudoku(int** sudoku);	//Used to ensure no memory latency when accessing sudoku during solving.
int*** getSudokus(char *file);	//Similar to getGrid, but for multiple sudokus in one text file on one line.
int getNumLines(char* fileName); //Get number of lines in text file

//Printing
void Print_Matrix(int** matrix);
void printGrid(int** grid);

//Solving
bool Solve(int** matrix);
	bool Search_Row(int** matrix, int row, int key);
	bool Search_Column(int** matrix, int column, int key);
	bool Search_Square(int** matrix, int row, int column, int key);
	bool anyEmptyBlock(int** matrix, int &row, int &column);



void doAll(int*** sudokus, char metric);

//Difficulty metric (Combines measurements below)
double getDifficulty(int** matrix);
	int getNumEmpty(int** matrix);
	int getMaxFilled(int** matrix);
		int findMax(int boxes[3][3], int rows[9], int cols[9]);

int main (int argc, char *argv[]){
	NUM_IN_FILE = getNumLines(argv[1]);
	int*** sudokus = getSudokus(argv[1]);

	doAll(sudokus, argv[2][0]);
}

void doAll(int*** sudokus, char metric){
	if(toupper(metric) == 'V'){	//For verifying that the algorithm works
		for(int i = 0; i < NUM_IN_FILE; i++){
			int** currSudoku = copySudoku(sudokus[i]);
			cout << "Initial sudoku:" << endl;
			printGrid(currSudoku);

			cout << "Completed sudoku:" << endl;
			Solve(currSudoku);
			printGrid(currSudoku);
		}
	}
	else{	//For empirical analysis
	
		for(int i = 0; i < NUM_IN_FILE; i++){
			int** currSudoku;
		
			if(toupper(metric) == 'E')	cout << getNumEmpty(sudokus[i]) << ",";
			else	cout << getDifficulty(sudokus[i]) << ",";
			
			double averageTime = 0;
			for(int j = 0; j < NUM_RUNS; j++){
				currSudoku = copySudoku(sudokus[i]);
				double start = omp_get_wtime();
				Solve(currSudoku);
				double end = omp_get_wtime();
				averageTime += (end-start)/(double) NUM_RUNS;
			}
			
			cout << averageTime << endl;
		}
	}
}

int** copySudoku(int** sudoku){
	int** copy = (int**) malloc(sizeof(int*)*9);
	
	for(int i = 0; i < 9; i++){
		copy[i] = (int*) malloc(sizeof(int)*9);
	}
	
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			copy[i][j] = sudoku[i][j];
		}
	}
	
	return copy;
}


int findMax(int boxes[3][3], int rows[9], int cols[9]){	//Used in getMaxFilled to determine the maximum, can be modified to extract more info
	int currMax = 0;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(boxes[i][j] > currMax) currMax = boxes[i][j];
		}
	}

	for(int i = 0; i < 9; i++){
		if( (rows[i] > currMax) && (rows[i] > cols[i]) ) currMax = rows[i];
		if(cols[i] > currMax) currMax = cols[i];
	}

	return currMax;
}

int getMaxFilled(int** matrix){	//Finds the maximum number of cells filled in for any block/row/column
	int boxes[3][3] = {	{0,0,0},
					    {0,0,0},
                        {0,0,0}};
	int rows[9] = {0,0,0,0,0,0,0,0,0};
	int cols[9] = {0,0,0,0,0,0,0,0,0};

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(matrix[i][j] != 0){
				rows[i]++;
				cols[j]++;
				boxes[i/3][j/3]++;
			}
		}
	}

	return findMax(boxes, rows, cols);
}

int getNumEmpty(int** matrix){	//Returns total number of empty cells in matrix
	int numEmpty = 0;

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(matrix[i][j] == 0) numEmpty++;
		}
	}

	return numEmpty;
}

double getDifficulty(int** matrix){	//A prototype metric for evaluating difficulty (Higher = harder)
	int numEmpty = getNumEmpty(matrix);		//Number of empty cells
	int maxFilled = getMaxFilled(matrix);	//Highest number of cells filled in for a block/row/column

	if(maxFilled == 0) return 0;	//Nothing filled in, hence difficulty is set to 0, since it is easy to choose any valid numbers
   	else return numEmpty/(double)maxFilled;	//Difficulty would be higher if numEmpty is higher, easier if maxFilled is higher.
}



int*** getSudokus(char *file){   //Gets multiple sudokus from text file and stores them in 3D array (Different format to getGrid())
	int*** matrix;
	ifstream infile;
	string str = "";

	matrix = (int***) malloc(sizeof(int**)*NUM_IN_FILE);
	for(int i = 0; i < NUM_IN_FILE; i++){
		matrix[i] = (int**) malloc(sizeof(int*)*9);
		for(int j = 0; j < 9; j++){
			matrix[i][j] = (int*) malloc(sizeof(int)*9);
		}
	}

	infile.open(file);
	
	for(int i = 0; i < NUM_IN_FILE; i++){
		getline(infile, str);

		for(int j = 0;j < 9*9; j++){
			if(!isspace(str[j])){
				matrix[i][j/9][j%9] = str[j] % 48;  // '% 48' converts ascii value to actual int value
			}
		}
	}
	infile.close();

	return matrix;
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

int** getGrid(char *file){   //Gets input from a txt file and converts to a matrix.Replaces spaces with 0's.
	int** matrix;
	ifstream infile;
	string str = "";

	matrix = new int*[9];
	for(int i = 0;i < 9;i++){
		matrix[i] = new int[9];
	}

	infile.open(file);
		for(int i = 0;i < 9;i++){
			getline(infile, str);
			for(int j = 0;j < 9;j++){
				if(!isspace(str[j])){
					matrix[i][j] = str[j] % 48;  // '% 48' converts ascii value to actual int value
				}
			}
		}
	infile.close();

	return matrix;
}

//Prints in a nicer format
void printGrid(int** grid){
	cout << "------------------- \n";
	for (int i = 0; i < 9; i++){
		cout << "| ";
		for (int j = 0; j < 9; j++){
			cout << grid[i][j];
			if ( (j+1) % 3 == 0 )
				cout << " | ";
		}

		cout << "\n";

		if ( (i+1) % 3 == 0 ){
			cout << "------------------- \n";
		}

	}

}


void Print_Matrix(int** matrix){
	for(int i = 0;i < 9;i++){
		for(int j = 0;j < 9;j++){
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


bool Search_Row(int** matrix, int row, int key){
	for(int i = 0;i < 9;i++){
		if(matrix[row][i] == key){
			return true;
		}
	}
	return false;
}

bool Search_Column(int** matrix, int column, int key){
	for(int i = 0;i < 9;i++){
		if(matrix[i][column] == key){
			return true;
		}
	}
	return false;
}

bool Search_Square(int** matrix, int row, int column, int key){
	int square_row = row/3;
	int square_column = column/3;

	for(int i = 0;i < 3;i++){
		for(int j = 0;j < 3;j++){
			if(matrix[ (square_row * 3) + i ][ (square_column * 3) + j ] == key){
				return true;
			}
		}
	}
	return false;
}

bool anyEmptyBlock(int** matrix, int &row, int &column){
	for(row = 0;row < 9;row++){
		for(column = 0;column < 9;column++){
			if(matrix[row][column] == 0)
				return true;
		}
	}
	return false;
}


bool Solve(int** matrix){
	int row ;
	int column;

	if(!anyEmptyBlock(matrix, row, column)){  //if there are no empty blocks then the sudoku is complete
		return true;
	}

	for(int value = 0;value <= 9;value++){
		if(!Search_Row(matrix, row, value)  &&  !Search_Column(matrix, column, value) && !Search_Square(matrix, row, column, value) ){   //chaecks is value can fit at empty block
			matrix[row][column] = value;

			if(Solve(matrix)){   //Recursively call for next available block
				return true;
			}

			matrix[row][column] = 0;   //if Solve fails, rewrite block and try again
		}
	}
	return false;   //invokes backtracking
}
