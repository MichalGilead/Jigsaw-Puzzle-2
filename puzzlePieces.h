/*
 * puzzlePieces.h
 *
 *  Created on: 14 Dec 2017
 *      Author: Sivan
 */

#ifndef PUZZLEPIECES_H_
#define PUZZLEPIECES_H_

#include "puzzleElement.h"
#include "edges.h"
#include <cmath>

using namespace std;

/**
 * The class represents a structure that manage a jigsaw puzzle elements.
 * The structure is an array in the range of all possible coding (NUM_OF_EDGE_TYPES base) for the elements
 * (that is NUM_OF_EDGE_TYPES^NUM_OF_EDGES), that holds in index i all the elements
 * that has suitable coding i.
 * Each element has 2^NUM_OF_EDGES coding options (the arrangement of the joker (*)).
 * The structure is responsible for finding the next piece that suits for the algorithm that solves the puzzle.
 * All coding are by this convention: 0 for straight edge, 1 for male edge, 2 for female edge, 3 for joker(*) edge
 *
 *
*/
class puzzlePieces{
private:
	int numOfElements; //number of elements in the jigsaw puzzle
	puzzleElement*** codedPieces; //holds in index i all elements with (NUM_OF_EDGE_TYPES base)i coding
	int* elementsNumberInIndices; //holds number of elements in each index of each coding
	bool* placedOnBoard; //holds true if the element i has been placed

	void addElementsToCodedPieces(puzzleElement** elements, int numOfPieces);
	void addElementSortedBuckets(puzzleElement* t, puzzleElement**** codedPiecesSorting, int** elementsCounting);
	int calc10BaseFromNBasedNum(int* numArray, int length, int base);
	void calcCodedArray(const edges* pieceEdges, int* pieceCoding);

public:
	puzzlePieces(puzzleElement** elements, int elementsNumber, bool rotationEnabled);
	puzzleElement* getPiece(const edges& pieceRequirement);
	puzzleElement* getNextPiece(const edges& pieceRequirement, puzzleElement* previousPieceToReturn);
	void calcNBasedNumFrom10Base(int num, int base, int* result, int resultLen);

	~puzzlePieces();

};


#endif /* PUZZLEPIECES_H_ */
