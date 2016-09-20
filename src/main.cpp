#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <string>

using namespace std;

int** getGrid(int argc, char *argv[]);
void Print_Matrix(int** matrix);
void printGrid(int** grid);
bool Search_Row(int** matrix, int row, int key);
bool Search_Column(int** matrix, int column, int key);
bool Search_Square(int** matrix, int row, int column, int key);
bool anyEmptyBlock(int** matrix, int &row, int &column);
bool Solve(int** matrix);

int main (int argc, char *argv[]){
	int** matrix;
	int row = -1;
	int column = -1;
	
	matrix = new int*[9];
	for(int i = 0;i < 9;i++){
		matrix[i] = new int[9];
	}

	/*int matrix[9][9] = {{3, 0, 6, 5, 0, 8, 4, 0, 0},
		              {5, 2, 0, 0, 0, 0, 0, 0, 0},
		              {0, 8, 7, 0, 0, 0, 0, 3, 1},
		              {0, 0, 3, 0, 1, 0, 0, 8, 0},
		              {9, 0, 0, 8, 6, 3, 0, 0, 5},
		              {0, 5, 0, 0, 9, 0, 6, 0, 0},
		              {1, 3, 0, 0, 0, 0, 2, 5, 0},
		              {0, 0, 0, 0, 0, 0, 0, 7, 4},
		              {0, 0, 5, 2, 0, 6, 3, 0, 0}};*/

	matrix = getGrid(argc, argv);
	
	cout << "Initial matrix: " << endl;
	printGrid(matrix);

	if(Solve(matrix)){
		cout << "Solution matrix: " << endl;
		printGrid(matrix);
	}
	else
		cout << "No Solution" << endl;
}

int** getGrid(int argc, char *argv[]){   //Gets input from a txt file and converts to a matrix.Replaces spaces with 0's.
	int** matrix;
	ifstream infile;
	string str = "";

	matrix = new int*[9];
	for(int i = 0;i < 9;i++){
		matrix[i] = new int[9];
	}

	infile.open(argv[1]);
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
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			cout << grid[i][j];
			if ( (j+1) % 3 == 0 )
				cout << " | ";
		}

		cout << "\n";

		if ( (i+1) % 3 == 0 ){
			cout << "----------------- \n";
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
