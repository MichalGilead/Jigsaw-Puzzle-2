/*
 * jigsawPuzzle.h
 *
 *  Created on: 22 ����� 2017
 *      Author: michal
 */

#ifndef JIGSAWPUZZLE_H_
#define JIGSAWPUZZLE_H_

#include "puzzlePieces.h"
#include <iostream>
#include <cmath>
using namespace std;
#include <iostream>

#define NUM_OF_ROTATIONS 4

/**
 * This class represents a jigsaw puzzle.
 *
*/
class jigsawPuzzle{
private:
	int numOfElements; //number of pieces in the puzzle
	int** divisors; //all divisors pairs of numOfElements
	int numOfDivisors; //number of divisors pairs
	puzzleElement** elements; //array of all puzzle pieces
	bool rotationEnabled; //true if there is a rotation option for the pieces, false otherwise
	puzzlePieces* puzzlePiecesManager; //manager for the puzzle pieces
	int leftStraightEdges;
	int topStraightEdges;
	int rightStraightEdges;
	int bottomStraightEdges;
	int numOfStraightEdges;
	bool wrongNumOfStraightEdges;

	int findNumOfDivisors();
	int** findDivisors();

	bool* missingCornerElementForRowOrColumnShapedPuzzle();
	void missingCornerElementForRegularPuzzle(bool* missingCorners);
	void missingCornerElementForRegularPuzzleWithRotation(bool* missingCorners);
	bool* missingCornerElementForRowOrColumnShapedPuzzleWithRotation();
	int* fillCornerArrays(int* TLCorners, int* TRCorners, int* BRCorners, int* BLCorners);

	/**
	 * counts number of straight edges by edge and saves to matching fields
	 *
	 * @return - false if number of left straight edges is equal to number of right straight edges
	 * and number of top straight edges is equal to number of bottom straight edges, true otherwise
	 * if rotations are enabled checks if there's an arrangement that fulfills the condition
	 */
	bool checkWrongNumOfStraightEdges();


public:
	jigsawPuzzle(int elementsNum, puzzleElement **puzzleElements, bool rotation);
	bool getWrongNumOfStraightEdges();
	/**
	 * @return - false if number of straight edges is at least the size of the largest frame, true otherwise
	 */
	bool notEnoughStraightEdgesForFrame();

	/**
	 * checks if there's a matching element for each corner
	 * @params -
	 * missingCorners - a pointer to a boolean array that saves for
	 *  each corner true if it is missing and false otherwise
	 *  hasMissingCornersOutOfFour - sets to true if there are missing corners out of 4 but
	 *  there are 2 corners for row/column solution
	 *
	 * @return - a pointer to a boolean array to handle the case where the puzzle
	 * doesn't have 4 corners but does have 2 corners for row/column shaped puzzle
	 * so we can reach the algorithm and search for a row/column solution but still have
	 * the missing corners out of the 4 if there's no such solution in the pointer we gave as parameter
	 *
	 */
	bool* missingCornerElement(bool* missingCorners, bool& hasMissingCornersOutOfFour);
	bool sumOfEdgesNotZero();
	int*** solvePuzzle(int& height, int& width);
	int  solvePuzzleRec(int row, int col, int numOfRows, int numOfColumns, int*** solvedPuzzle);

	int getNumOfDivisors() const;
	int** getDivisors() const;
	int getNumOfElements() const;
	puzzleElement** getElements() const;


	~jigsawPuzzle();

};

int*** createPuzzle(int numOfRows, int numOfColumns);
void destroyPuzzle(int*** puzzle, int numOfRows, int numOfColumns);


#endif /* JIGSAWPUZZLE_H_ */
