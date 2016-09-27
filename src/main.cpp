#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <string>
#include <omp.h>

using namespace std;

char files[12][16] = 	{
				{"Input/Easy1.txt"},
				{"Input/Easy2.txt"},
				{"Input/Easy3.txt"},
				{"Input/Medi1.txt"},
				{"Input/Medi2.txt"},
				{"Input/Medi3.txt"},
				{"Input/Hard1.txt"},
				{"Input/Hard2.txt"},
				{"Input/Hard3.txt"},
				{"Input/Evil1.txt"},
				{"Input/Evil2.txt"},
				{"Input/Evil3.txt"}
			};

int** getGrid(char* file);
void Print_Matrix(int** matrix);
void printGrid(int** grid);
bool Search_Row(int** matrix, int row, int key);
bool Search_Column(int** matrix, int column, int key);
bool Search_Square(int** matrix, int row, int column, int key);
bool anyEmptyBlock(int** matrix, int &row, int &column);
bool Solve(int** matrix);
void doAll();

//Difficulty metric
double getDifficulty(int** matrix);
int getNumEmpty(int** matrix);

int main (int argc, char *argv[]){
	std::string str1 ("all");
	if(str1.compare(argv[1])){
		int** matrix;
		int row = -1;
		int column = -1;

		matrix = new int*[9];
		for(int i = 0;i < 9;i++){
			matrix[i] = new int[9];
		}
	
		matrix = getGrid(argv[1]);

		cout << "Initial matrix: " << endl;
		printGrid(matrix);

		cout << "Difficulty metric: " << endl;
		cout << getDifficulty(matrix) << endl;
	
		double start = omp_get_wtime();
		if(Solve(matrix)){
			double end = omp_get_wtime();
			cout << "Solution matrix: " << endl;
			printGrid(matrix);
		}
		else
			cout << "No Solution" << endl;
	}
	else doAll();
}

void doAll(){
	for(int i = 0; i < 12; i++){
		int** matrix;
		int row = -1;
		int column = -1;

		matrix = new int*[9];
		for(int j = 0;j < 9;j++){
			matrix[i] = new int[9];
		}
	
		matrix = getGrid(files[i]);

		cout << getNumEmpty(matrix) << ",";
		
		double averageTime = 0;
		
		for(int j = 0; j < 100; j++){
			double start = omp_get_wtime();
			Solve(matrix);
			double end = omp_get_wtime();
			averageTime += (end-start)/100.0;
			matrix = getGrid(files[i]);
		}
		
		cout << averageTime << endl;
	}
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
   	else return numEmpty/maxFilled;	//Difficulty would be higher if numEmpty is higher, easier if maxFilled is higher.
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
