/*

 * main.cpp
 *
 *  Created on: 21 ����� 2017
 *      Author: michal
*/

#include "mainAux.h"

int main (int argc, char **argv){
	bool rotationEnabled = false, *missingCorners, hasMissingCornersOutOfFour = false;
	int inputFileIndex=1, outputFileIndex=2, solutionWidth, solutionHeight, ***solution;
	if (argc < 3){
		cout << "Usage: main was not given 2 arguments." << endl;
		return 1;
	}
	if (argc==4){ //check if rotate was entered and in which index the input and output files are
		if (isRotate(argv[1])){
			rotationEnabled = true;
			inputFileIndex++;
			outputFileIndex++;
		}
		else if (isRotate(argv[2])){
			rotationEnabled = true;
			outputFileIndex++;
		}
		else if (isRotate(argv[3]))
			rotationEnabled = true;
	}
	missingCorners = new bool[NUM_OF_CORNERS];
	fill_n(missingCorners, NUM_OF_CORNERS, true);
	ifstream fin(argv[inputFileIndex]);
	if (!fin.is_open()){
		cout << "Usage: could not open input file." << endl;
		return 1;
	}
	ofstream fout(argv[outputFileIndex]);

	jigsawPuzzle* puzzle = readFromInputFile(fin, fout, rotationEnabled);

	fin.close();
	if (puzzle == NULL){
		delete[] missingCorners;
		fout.close();
		return 1;
	}

	if (handleOneElement(fout, puzzle) || writeUnsolvableToOutputFile(fout, puzzle, missingCorners, hasMissingCornersOutOfFour)){
		delete puzzle;
		fout.close();
		return 0;
	}

	solution = puzzle->solvePuzzle(solutionHeight, solutionWidth);

	writeSolutionToOutputFile(fout, solution, solutionHeight, solutionWidth);
	delete puzzle;
	fout.close();
	return 0;
}

