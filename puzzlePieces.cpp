/*
 * puzzlePieces.cpp
 *
 *  Created on: 14 Dec 2017
 *      Author: Sivan
 */

#include "puzzlePieces.h"

puzzlePieces :: puzzlePieces(puzzleElement** elements, int elementsNumber, bool rotationEnabled){
	int numOfPieces;

	numOfElements = elementsNumber;

	//updates numOfPieces according to rotationEnabled enabled
	numOfPieces = (rotationEnabled)? numOfElements*NUM_OF_EDGES : numOfElements;

	//creates codedPieces
	codedPieces = new puzzleElement**[NUM_OF_OPTIONS];
	for (int i=0; i<NUM_OF_OPTIONS; i++)
		codedPieces[i] = new puzzleElement*[numOfPieces];

	//creates elementsNumberInIndices
	elementsNumberInIndices = new int[NUM_OF_OPTIONS];
	for(int i=0; i<NUM_OF_OPTIONS; i++) //initializes to zeros
		elementsNumberInIndices[i] = 0;

	//adds elements to codedPieces
	addElementsToCodedPieces(elements, numOfPieces);

	//creates placedOnBoard
	placedOnBoard = new bool[numOfElements];
	for(int i=0; i<numOfElements; i++) //initializes to false
		placedOnBoard[i] = false;

}

//adds the puzzle elements to codedPieces array.
//each coding array in codedPieces holds the elements sorted by their original coding (without joker(*))
//the sorting is done by a bucket sort method.
void puzzlePieces :: addElementsToCodedPieces(puzzleElement** elements, int numOfPieces){
	//an array for storing the elements in their original coding order.
	//It's an array of all coding options arrays. Each coding option i holds an array of original coding size-
	//(NUM_OF_EDGE_TYPES-1)^NUM_OF_EDGES. So codedPiecesSorting holds in index [i][j] all the
	//elements with an i-coding option and an j-original coding.
	puzzleElement**** codedPiecesSorting;
	int** elementsCounting; //holds in index [i][j] the number of elements with an i-coding option and an j-original coding.
	int originalCodingSize = pow(NUM_OF_EDGE_TYPES-1,NUM_OF_EDGES); //number of original coding options

	codedPiecesSorting = new puzzleElement***[NUM_OF_OPTIONS];
	for (int i=0; i<NUM_OF_OPTIONS; i++){
		codedPiecesSorting[i] = new puzzleElement**[originalCodingSize];
		for (int j=0; j<originalCodingSize; j++)
			codedPiecesSorting[i][j] = new puzzleElement*[numOfPieces];
	}

	elementsCounting = new int*[NUM_OF_OPTIONS];
	for (int i=0; i<NUM_OF_OPTIONS; i++){
		elementsCounting[i] = new int[originalCodingSize];
		for (int j=0; j<originalCodingSize; j++)
			elementsCounting[i][j] = 0;
	}

	//adds elements to codedPiecesSorting.
	for (int i=0; i<numOfPieces; i++)
		addElementSortedBuckets(elements[i], codedPiecesSorting, elementsCounting);

	//adds elements to codedPieces according to the sorting order.
	for (int i=0; i<NUM_OF_OPTIONS; i++){
		for (int j=0; j<originalCodingSize; j++)
			for (int k=0; k<elementsCounting[i][j]; k++){
				codedPieces[i][elementsNumberInIndices[i]] = codedPiecesSorting[i][j][k];
				codedPieces[i][elementsNumberInIndices[i]]->setElementIndexInArray(i, elementsNumberInIndices[i]); //updates indicesInArrays of element to hold the right index
				elementsNumberInIndices[i]++; //increases elementsNumberInIndices in index i by 1
			}
	}

	//deletes codedPiecesSorting
	for (int i=0; i<NUM_OF_OPTIONS; i++){
		for (int j=0; j<originalCodingSize; j++)
			delete[] codedPiecesSorting[i][j];
		delete[] codedPiecesSorting[i];

	}
	delete[] codedPiecesSorting;

	//deletes elementsCounting
	for (int i=0; i<NUM_OF_OPTIONS; i++)
		delete[] elementsCounting[i];
	delete[] elementsCounting;


}

//adds element t to codedPiecesSorting array.
void puzzlePieces :: addElementSortedBuckets(puzzleElement* t, puzzleElement**** codedPiecesSorting, int** elementsCounting){
	int* elementCoding; //holds the element original coding-  NUM_OF_EDGE_TYPES-1 base
	int* elementJokerCoding; //holds an element joker coding-  NUM_OF_EDGE_TYPES base
	int* binaryBaseNum;
	int numOfJokerChoices;
	int JokerCodingIndex, originalCodingIndex;

	elementCoding = new int[NUM_OF_EDGES];
	elementJokerCoding = new int[NUM_OF_EDGES];

	//initializes arrays to the original NUM_OF_EDGE_TYPES-1 base coding
	calcCodedArray(t->getElementEdges(), elementCoding);
	calcCodedArray(t->getElementEdges(), elementJokerCoding);

	//calculates the element original coding- 10 base
	originalCodingIndex = calc10BaseFromNBasedNum(elementCoding, NUM_OF_EDGES, NUM_OF_EDGE_TYPES-1);

	binaryBaseNum = new int[NUM_OF_EDGES];
	numOfJokerChoices = pow(2,NUM_OF_EDGES); //calculates number of jokerChoices


	//calculates coding for all joker options
	for (int i=0; i<numOfJokerChoices; i++){
		calcNBasedNumFrom10Base(i, 2, binaryBaseNum, NUM_OF_EDGES); //converts 10-base i to binary-base number

		//for each j that binaryBaseNum[j] == 1, replace the elementJokerCoding[j] to "3" (joker- *)
		for (int j=0; j<NUM_OF_EDGES; j++){
			if (binaryBaseNum[j] == 1)
				elementJokerCoding[j] = 3;
		}

		//converts element coding number to 10-base number
		JokerCodingIndex = calc10BaseFromNBasedNum(elementJokerCoding, NUM_OF_EDGES, NUM_OF_EDGE_TYPES);

		//adds element to codedPiecesSorting according to its JokerCodingIndex and originalCodingIndex
		codedPiecesSorting[JokerCodingIndex][originalCodingIndex][elementsCounting[JokerCodingIndex][originalCodingIndex]] = t;
		elementsCounting[JokerCodingIndex][originalCodingIndex]++; //increases elementsNumberInIndices in the relevant index by 1

		//restores elementJokerCoding the original coding
		for (int j=0; j<NUM_OF_EDGES; j++){
			elementJokerCoding[j] = elementCoding[j];
		}

	}

	delete[] binaryBaseNum;
	delete[] elementCoding;
	delete[] elementJokerCoding;

}

//converts parameter num from 10-base to base parameter-base
//stores the result in result
void puzzlePieces :: calcNBasedNumFrom10Base(int num, int base, int* result, int resultLen){
	int index=0;

	while (num > 0){
		result[index] = num%base;
		num /= base;
		index++;
	}

	for (int i=index; i<resultLen; i++)
		result[i] =0;
}

//converts parameter numArray from base parameter-base to 10-base.
int puzzlePieces :: calc10BaseFromNBasedNum(int* numArray, int length, int base){
	int result=0;
	for (int i=0; i< length; i++)
		result += numArray[i]*pow(base, i);
	return result;
}

//returns the first piece that suits the pieceRequirement parameter, or NULL if there is none.
puzzleElement* puzzlePieces :: getPiece(const edges& pieceRequirement){
	int* pieceRequirementCoding;
	int i;
	int codingIndex;
	puzzleElement* piece=NULL;

	pieceRequirementCoding = new int[NUM_OF_EDGES];

	//initializes array- NUM_OF_EDGE_TYPES-base coding for pieceRequirement
	calcCodedArray(&pieceRequirement, pieceRequirementCoding);

	//converts pieceRequirementCoding number to 10-base number
	codingIndex = calc10BaseFromNBasedNum(pieceRequirementCoding, NUM_OF_EDGES, NUM_OF_EDGE_TYPES);

	i=0;
	//passes over all elements that has been placed for the current solution
	while (i<elementsNumberInIndices[codingIndex] && placedOnBoard[codedPieces[codingIndex][i]->getElementId()-1]){
		i++;
	}

	//a matching piece for pieceRequirement was found
	if (i<elementsNumberInIndices[codingIndex]){
		piece = codedPieces[codingIndex][i];
		placedOnBoard[piece->getElementId()-1] = true; //updates placedOnBoard to be true for the found piece
	}

	delete[] pieceRequirementCoding;

	return piece;
}

//returns the next piece that suits the pieceRequirement parameter, or NULL if there is none.
//doesn't return a piece that has the same edges as a piece that already been checked for the current solution
puzzleElement* puzzlePieces :: getNextPiece(const edges& pieceRequirement, puzzleElement* previousPieceToReturn){
	int* pieceRequirementCoding, *previousPieceCoding, *candidatePieceCoding;
	int i;
	int codingIndex;
	int elementIndexInArray, previousPieceCodingInt;
	puzzleElement* nextPiece=NULL;

	pieceRequirementCoding = new int[NUM_OF_EDGES];
	previousPieceCoding = new int[NUM_OF_EDGES];
	candidatePieceCoding = new int[NUM_OF_EDGES];

	//initializes array NUM_OF_EDGE_TYPES-base coding for pieceRequirementCoding
	calcCodedArray(&pieceRequirement, pieceRequirementCoding);
	//initializes array (NUM_OF_EDGE_TYPES-1)-base coding for previousPieceCoding
	calcCodedArray(previousPieceToReturn->getElementEdges(), previousPieceCoding);

	//converts pieceRequirementCoding number to 10-base number
	codingIndex = calc10BaseFromNBasedNum(pieceRequirementCoding, NUM_OF_EDGES, NUM_OF_EDGE_TYPES);

	//converts previousPieceCoding number to 10-base number
	previousPieceCodingInt = calc10BaseFromNBasedNum(previousPieceCoding, NUM_OF_EDGES, NUM_OF_EDGE_TYPES-1);

	//updates placedOnBoard to be false for the previous piece
	placedOnBoard[previousPieceToReturn->getElementId()-1] = false;

	elementIndexInArray = previousPieceToReturn->getElementIndexInArray(codingIndex);
	i=elementIndexInArray+1;

	//starts finding the next piece from the previous piece index in codingIndex array
	//passes over all elements that has been placed for the current solution,
	//or that has the same edges as the previous piece
	if (i<elementsNumberInIndices[codingIndex])
		calcCodedArray(codedPieces[codingIndex][i]->getElementEdges(), candidatePieceCoding);
	while (i<elementsNumberInIndices[codingIndex] && (placedOnBoard[codedPieces[codingIndex][i]->getElementId()-1]
				  || previousPieceCodingInt == calc10BaseFromNBasedNum(candidatePieceCoding, NUM_OF_EDGES, NUM_OF_EDGE_TYPES-1))){
		i++;
		if (i<elementsNumberInIndices[codingIndex])
			calcCodedArray(codedPieces[codingIndex][i]->getElementEdges(), candidatePieceCoding);
	}

	//the next matching piece for pieceRequirement was found
	if (i<elementsNumberInIndices[codingIndex]){
		nextPiece = codedPieces[codingIndex][i];
		placedOnBoard[nextPiece->getElementId()-1] = true; //updates placedOnBoard to be true for the found piece
	}

	delete[] pieceRequirementCoding;
	delete[] previousPieceCoding;
	delete[] candidatePieceCoding;

	return nextPiece;
}

//Calculates pieceCoding according to pieceEdges. Fills in indices 0,1,2 and 3 the edges left, top, right
//and bottom, respectively.
//0 for straight edge, 1 for male edge, 2 for female edge
void puzzlePieces :: calcCodedArray(const edges* pieceEdges, int* pieceCoding){
	pieceCoding[0] = (pieceEdges->getLeftEdge() == -1)? 2: pieceEdges->getLeftEdge();
	pieceCoding[1] = (pieceEdges->getTopEdge() == -1)? 2: pieceEdges->getTopEdge();
	pieceCoding[2] = (pieceEdges->getRightEdge() == -1)? 2: pieceEdges->getRightEdge();
	pieceCoding[3] = (pieceEdges->getBottomEdge() == -1)? 2: pieceEdges->getBottomEdge();
}


puzzlePieces :: ~puzzlePieces(){
	for (int i=0; i<NUM_OF_OPTIONS; i++)
		delete[] codedPieces[i];
	delete[] codedPieces;

	delete[] elementsNumberInIndices;

	delete[] placedOnBoard;

}
