/*
 * jigsawPuzzle.cpp
 *
 *  Created on: 22 ����� 2017
 *      Author: michal
 */



#include "jigsawPuzzle.h"


jigsawPuzzle :: jigsawPuzzle(int elementsNum, puzzleElement** puzzleElements, bool rotation){
	numOfElements = elementsNum;
	numOfDivisors = findNumOfDivisors();
	divisors = findDivisors();
	elements = puzzleElements;
	rotationEnabled = rotation;
	numOfStraightEdges = 0;
	for (int i=0; i<numOfElements; i++)
		numOfStraightEdges += elements[i*(rotationEnabled ? NUM_OF_ROTATIONS : 1)]->getNumOfStraightEdgesInElement();
	wrongNumOfStraightEdges = checkWrongNumOfStraightEdges();
	puzzlePiecesManager = new puzzlePieces(puzzleElements, elementsNum, rotation);
}

int jigsawPuzzle :: findNumOfDivisors(){
	int root = floor(sqrt(numOfElements));
	int cnt=0;

	for(int i=1; i<=root; i++){
		if(numOfElements%i == 0)
			cnt++;
	}
	return cnt;
}

int** jigsawPuzzle :: findDivisors(){
	int root = floor(sqrt(numOfElements));
	int index;
	int** divisors;

	divisors = new int*[numOfDivisors];
	for(int i=0; i<numOfDivisors; i++)
		divisors[i] = new int[2];

	index=0;
	for(int i=1; i<=root; i++){
		if(numOfElements%i == 0){
			divisors[index][0] = i;
			divisors[index][1] = numOfElements/i;
			index++;
		}
	}
	return divisors;
}

int jigsawPuzzle :: getNumOfElements() const{
	return numOfElements;
}

int jigsawPuzzle :: getNumOfDivisors() const{
	return numOfDivisors;
}

int** jigsawPuzzle :: getDivisors() const{
	return divisors;
}

bool jigsawPuzzle :: getWrongNumOfStraightEdges(){
	return wrongNumOfStraightEdges;
}

puzzleElement** jigsawPuzzle :: getElements() const{
	return elements;
}

//solves the current jigsaw puzzle
int*** jigsawPuzzle :: solvePuzzle(int& height, int& width){
	int n, m;
	int res;
	int*** solvedPuzzle;

	//goes over all divisors pairs of numOfElements
	for(int i=0; i<numOfDivisors; i++){
		n = divisors[i][0];
		m = divisors[i][1];

		//checks if rotation enabled or if there is enough straight edges for n*m frame in the non-rotation option
		if (rotationEnabled || (n <= leftStraightEdges && n <= rightStraightEdges && m <= topStraightEdges && m <= bottomStraightEdges)){
			solvedPuzzle = createPuzzle(n, m);
			//checks if there is a solution for n*m frame
			res = solvePuzzleRec(0 ,0 , n, m, solvedPuzzle);
			if (res == 1){ //a solution was found
				height = n; //updates frame dimensions
				width = m;
				return solvedPuzzle;
			}
			destroyPuzzle(solvedPuzzle, n, m);
		}
		//checks if rotation enabled or if there is enough straight edges for m*n frame in the non-rotation option
		if (rotationEnabled || (m <= leftStraightEdges && m <= rightStraightEdges && n <= topStraightEdges && n <= bottomStraightEdges)){
			solvedPuzzle = createPuzzle(m, n);
			//checks if there is a solution for m*n frame
			res = solvePuzzleRec(0 ,0 , m, n, solvedPuzzle);
			if (res == 1){ //a solution was found
				height = m; //updates frame dimensions
				width = n;
				return solvedPuzzle;
			}
			destroyPuzzle(solvedPuzzle, m, n);
		}
	}

	return NULL; //no solution was found
}

//checks if there is a solution for n*m frame recursively (each column in each row from 0,0).
//stores the solution (is exists) in solvedPuzzle. solvedPuzzle[i][j][0] holds the ID of the
//piece that was placed in the [i,j] location and solvedPuzzle[i][j][1] holds the rotation angle of that piece.
//returns 1 if a solution was found, 0 otherwise.
int jigsawPuzzle :: solvePuzzleRec(int row, int col, int numOfRows, int numOfColumns, int*** solvedPuzzle){
	int lefttAdj, topAdj, rightAdj, bottomAdj;
	puzzleElement* pieceToPlace=NULL;
	int i ,res;
	int leftPieceIndex=0, upperPieceIndex=0;

	if (row == numOfRows) //reached the end of the puzzle - a solution was found
		return 1;

	//updates leftPieceIndex and upperPieceIndex according to solvedPuzzle and rotationEnabled
	if (col != 0)
		leftPieceIndex = rotationEnabled? (solvedPuzzle[row][col-1][0]-1)*NUM_OF_EDGES + solvedPuzzle[row][col-1][1]/(360/NUM_OF_EDGES) :
				solvedPuzzle[row][col-1][0]-1;
	if (row != 0)
		upperPieceIndex = rotationEnabled? (solvedPuzzle[row-1][col][0]-1)*NUM_OF_EDGES + solvedPuzzle[row-1][col][1]/(360/NUM_OF_EDGES) :
				solvedPuzzle[row-1][col][0]-1;

	//updates piece requirement edges according to the pieces that has been placed in solvedPuzzle
	//and the frame requirements.
	lefttAdj = (col == 0) ? 0 :  (elements[leftPieceIndex]->getElementEdges()->getRightEdge())*(-1);
	topAdj = (row == 0) ? 0 : (elements[upperPieceIndex]->getElementEdges()->getBottomEdge())*(-1);
	rightAdj = (col == numOfColumns-1) ? 0 : 3;
	bottomAdj = (row == numOfRows-1) ? 0 : 3;

	edges pieceRequirement(lefttAdj, topAdj, rightAdj, bottomAdj, 0);

	res=0; //no solution yet
	i=0;
	pieceToPlace = puzzlePiecesManager->getPiece(pieceRequirement); //gets the first suitable piece, or NULL if there in none.

	//goes over all suitable pieces until a solution was found for one of them, or there are no more
	//suitable pieces.
	while (pieceToPlace!=NULL && res == 0){
		//place piece ID and rotation angle in solvedPuzzle "board"
		solvedPuzzle[row][col][0] = pieceToPlace->getElementId();
		solvedPuzzle[row][col][1] = pieceToPlace->getElementRotationAngle();

		//invokes solvePuzzleRec for the next piece
		if (col == numOfColumns-1) //next row, column 0
			res = solvePuzzleRec(row+1, 0, numOfRows, numOfColumns, solvedPuzzle);
		else //same row, next column
			res = solvePuzzleRec(row, col+1, numOfRows, numOfColumns, solvedPuzzle);

		//gets the next suitable piece, or NULL if there in none.
		pieceToPlace = puzzlePiecesManager->getNextPiece(pieceRequirement, pieceToPlace);
		i++;
	}

	return res;
}

//creates a puzzle matrix of numOfRows*numOfColumns*2.
int*** createPuzzle(int numOfRows, int numOfColumns){
	int*** solvedPuzzle;

	solvedPuzzle = new int**[numOfRows];
	for(int i=0; i<numOfRows; i++){
		solvedPuzzle[i] = new int*[numOfColumns];
		for (int j=0; j<numOfColumns; ++j)
			solvedPuzzle[i][j] = new int[2];
	}

	for(int i=0; i<numOfRows; i++){
		for (int j=0; j<numOfColumns; j++)
			for (int k=0; k<2; k++)
				solvedPuzzle[i][j][k] = 0;
	}

	return solvedPuzzle;
}

void destroyPuzzle(int*** puzzle, int numOfRows, int numOfColumns){
	for(int i = 0; i < numOfRows; i++){
		for (int j=0; j<numOfColumns; ++j)
			delete[] puzzle[i][j];
		delete[] puzzle[i];
	}

	delete[] puzzle;
}

bool jigsawPuzzle :: checkWrongNumOfStraightEdges(){
	int index, *baseFourInt ,baseFour = 4, numOfRotations;
	long numOfIterations;
	numOfRotations = rotationEnabled ? NUM_OF_ROTATIONS : 1;
	numOfIterations = pow(numOfRotations, numOfElements);

	baseFourInt = new int[numOfElements];

	if (numOfStraightEdges%2 != 0){
		return true;
	}
	for (int i=0; i<numOfIterations; i++){//for each element
		puzzlePiecesManager->calcNBasedNumFrom10Base(i, baseFour, baseFourInt, numOfElements);

		leftStraightEdges = 0, topStraightEdges = 0, rightStraightEdges = 0, bottomStraightEdges = 0;
		for (int j=0; j<numOfElements; j++){//for each combination of elements rotations
			index = j*numOfRotations+baseFourInt[j];
			leftStraightEdges += elements[index]->getElementEdges()->getLeftEdge()==0 ? 1 : 0;
			topStraightEdges += elements[index]->getElementEdges()->getTopEdge()==0 ? 1 : 0;
			rightStraightEdges += elements[index]->getElementEdges()->getRightEdge()==0 ? 1 : 0;
			bottomStraightEdges += elements[index]->getElementEdges()->getBottomEdge()==0 ? 1 : 0;
		}

		if (i==0 && !rotationEnabled){//if no rotations - return first calculation answer
			delete[] baseFourInt;
			return (leftStraightEdges!=rightStraightEdges)||(topStraightEdges!=bottomStraightEdges);
		}//return false if we found a good combination, otherwise continue
		if ((leftStraightEdges==rightStraightEdges)&&(topStraightEdges==bottomStraightEdges)){
			delete[] baseFourInt;
			return false;
		}
	}
	delete[] baseFourInt;
	return true;
}

bool jigsawPuzzle :: notEnoughStraightEdgesForFrame(){
	if ((divisors[numOfDivisors-1][0] + divisors[numOfDivisors-1][1])*2 <= (numOfStraightEdges))
		return false;
	return true;
}

bool* jigsawPuzzle :: missingCornerElementForRowOrColumnShapedPuzzle(){
	bool* missingCorners = new bool[NUM_OF_CORNERS];
	int numOfSquares, numOfTops, numOfLefts, numOfRights, numOfBottoms;
		fill_n(missingCorners, NUM_OF_CORNERS, true);
		numOfSquares = 0, numOfTops = 0, numOfLefts = 0, numOfRights = 0, numOfBottoms = 0;
		//counting square elements and row/column possible corners separately
		for (int i=0; i<numOfElements; i++){
			if (elements[i]->isSquare())
				numOfSquares++;
			else if (elements[i]->getGoodForCorner()[0]&&elements[i]->getGoodForCorner()[3]){
				numOfLefts++;
				missingCorners[0] = missingCorners[3] = false;
			}
			else if (elements[i]->getGoodForCorner()[0]&&elements[i]->getGoodForCorner()[1]){
				numOfTops++;
				missingCorners[0] = missingCorners[1] = false;
			}
			else if (elements[i]->getGoodForCorner()[2]&&elements[i]->getGoodForCorner()[3]){
				numOfBottoms++;
				missingCorners[2] = missingCorners[3] = false;
			}
			else if (elements[i]->getGoodForCorner()[1]&&elements[i]->getGoodForCorner()[2]){
				numOfRights++;
				missingCorners[1] = missingCorners[2] = false;
			}
		}
		//check we have at least 2 row/column corners
		if ((numOfLefts!=0 && numOfRights!=0) || (numOfTops!=0 && numOfBottoms!=0))
			return missingCorners;
		if (numOfSquares>=2 || (numOfSquares==1 && (numOfLefts!=0 || numOfRights!=0 || numOfTops!=0 || numOfBottoms!=0))){
			fill_n(missingCorners, NUM_OF_CORNERS, false);
			return missingCorners;
		}
		missingCorners[1] = missingCorners[2] = missingCorners[3] = false;
	return 	missingCorners;
}

void checkEmptyCornerArray(int& size, int* arr){
	if (size==0){
		arr[0] = 0;
		size++;
	}
}

int* jigsawPuzzle :: fillCornerArrays(int* TLCorners, int* TRCorners, int* BRCorners, int* BLCorners){
	int i, TLCornersIndex = 0, TRCornersIndex = 0, BRCornersIndex = 0, BLCornersIndex = 0, *cornerArraysIndices;

	for (i=0; i<numOfElements; i++){
		if (elements[i]->getGoodForCorner()[0]){
			TLCorners[TLCornersIndex] = elements[i]->getElementId();
			TLCornersIndex++;
		}
		if (elements[i]->getGoodForCorner()[1]){
			TRCorners[TRCornersIndex] = elements[i]->getElementId();
			TRCornersIndex++;
		}
		if (elements[i]->getGoodForCorner()[2]){
			BRCorners[BRCornersIndex] = elements[i]->getElementId();
			BRCornersIndex++;
		}
		if (elements[i]->getGoodForCorner()[3]){
			BLCorners[BLCornersIndex] = elements[i]->getElementId();
			BLCornersIndex++;
		}
	}
	checkEmptyCornerArray(TLCornersIndex, TLCorners);
	checkEmptyCornerArray(TRCornersIndex, TRCorners);
	checkEmptyCornerArray(BRCornersIndex, BRCorners);
	checkEmptyCornerArray(BLCornersIndex, BLCorners);

	cornerArraysIndices = new int[NUM_OF_CORNERS];
	cornerArraysIndices[0] = TLCornersIndex;
	cornerArraysIndices[1] = TRCornersIndex;
	cornerArraysIndices[2] = BRCornersIndex;
	cornerArraysIndices[3] = BLCornersIndex;
	return cornerArraysIndices;
}


void jigsawPuzzle :: missingCornerElementForRegularPuzzle(bool* missingCorners){
	int i, j, k, l, m, cnt, *cornerArraysIndices;
	bool updatedCorners = false;
	bool** missingCornersNumber = new bool*[NUM_OF_CORNERS];
	for (m=0; m<NUM_OF_CORNERS; m++){
		missingCornersNumber[m] = NULL;
	}
	int *TLCorners = new int[numOfElements];
	int *TRCorners = new int[numOfElements];
	int *BRCorners = new int[numOfElements];
	int *BLCorners = new int[numOfElements];
	//create array for each corner and put fitting elements in matching array
	cornerArraysIndices = fillCornerArrays(TLCorners, TRCorners, BRCorners, BLCorners);

	//check an arrangement of four different elements from the different arrays
	for (i=0; i<cornerArraysIndices[0]; i++){
		if (TLCorners[i]!=0)
			missingCorners[0] = false;
		else
			missingCorners[0] = true;
		for (j=0; j<cornerArraysIndices[1]; j++){
			if (TRCorners[j]!=0 && TRCorners[j]!=TLCorners[i])
				missingCorners[1] = false;
			else
				missingCorners[1] = true;
			for (k=0; k<cornerArraysIndices[2]; k++){
				if (BRCorners[k]!=0 && BRCorners[k]!=TRCorners[j] && BRCorners[k]!=TLCorners[i])
					missingCorners[2] = false;
				else
					missingCorners[2] = true;
				for (l=0; l<cornerArraysIndices[3]; l++){
					if (BLCorners[l]!=0 && BLCorners[l]!=BRCorners[k] && BLCorners[l]!=TRCorners[j] && BLCorners[l]!=TLCorners[i])
						missingCorners[3] = false;
					else
						missingCorners[3] = true;

					//if we found an arrangement that satisfies all corners - return that we have all corners
					cnt = (missingCorners[0]==false) + (missingCorners[1]==false) + (missingCorners[2]==false) + (missingCorners[3]==false);
					if (cnt==NUM_OF_CORNERS){
						for (m=0; m<NUM_OF_CORNERS; m++){
							if (missingCornersNumber[m]!=NULL)
								delete[] missingCornersNumber[m];
						}
						delete[] missingCornersNumber;
						delete[] TLCorners;
						delete[] TRCorners;
						delete[] BRCorners;
						delete[] BLCorners;
						delete[] cornerArraysIndices;
						return;
					}
					//if we haven't found four corners yet - save the biggest arrangement of corners wev'e found
					if (missingCornersNumber[cnt]==NULL){
						missingCornersNumber[cnt] = new bool[NUM_OF_CORNERS];
						for (m=0; m<NUM_OF_CORNERS; m++)
							missingCornersNumber[cnt][m] = missingCorners[m];
					}
				}
			}
		}
	}
	//if we haven't found four corners - return answer according to the biggest arrangement of corners wev'e found
	for (m=NUM_OF_CORNERS; m>0; m--){
		if (missingCornersNumber[m-1]!=NULL){
			if (!updatedCorners){
				for (i=0; i<NUM_OF_CORNERS; i++)
					missingCorners[i] = missingCornersNumber[m-1][i];
				updatedCorners = true;
			}
			delete[] missingCornersNumber[m-1];
		}
	}
	delete[] missingCornersNumber;
	delete[] TLCorners;
	delete[] TRCorners;
	delete[] BRCorners;
	delete[] BLCorners;
	delete[] cornerArraysIndices;
}

//counts different elements that are fit to be corners and returns accordingly (if there are more than 4 we have all corners)
void jigsawPuzzle :: missingCornerElementForRegularPuzzleWithRotation(bool* missingCorners){
	int i = 0, countCorners = 0;
	while (i<numOfElements && countCorners<NUM_OF_CORNERS){
		if (elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[0]||elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[1]||
				elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[2]||elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[3])
			countCorners++;
		i++;
	}
	for (i=0; i<countCorners; i++)
		missingCorners[i] = false;
}

//counts different elements that can be corners of row/column puzzle and counts regular corner elements and returns accordingly
bool* jigsawPuzzle :: missingCornerElementForRowOrColumnShapedPuzzleWithRotation(){
	int i = 0, countRegularCorners = 0, countRowOrColumnCorners = 0;
	bool* missingCorners = new bool[NUM_OF_CORNERS];
	fill_n(missingCorners, NUM_OF_CORNERS, true);
	while (i<numOfElements && countRowOrColumnCorners<2){
		if ((elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[0]&&elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[1])||
				(elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[1]&&elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[2])||
				(elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[2]&&elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[3])||
				(elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[3]&&elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[0]))
			countRowOrColumnCorners++;
		else if (elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[0]||elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[1]||
				elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[2]||elements[i*NUM_OF_ROTATIONS]->getGoodForCorner()[3])
			if (countRegularCorners<2)
				countRegularCorners++;

		i++;
	}
	if (countRowOrColumnCorners>=2)//has 2 row/column corners - meaning 4 corners
		for (i=0; i<NUM_OF_CORNERS; i++)
			missingCorners[i] = false;
	else if (countRowOrColumnCorners==1&&countRegularCorners>=1)//has only 3 corners
		for (i=0; i<NUM_OF_CORNERS-1; i++)
			missingCorners[i] = false;
	else if (countRegularCorners>=2)//has only 2 corners
		for (i=0; i<NUM_OF_CORNERS-2; i++)
			missingCorners[i] = false;
	return missingCorners;
}

bool* jigsawPuzzle :: missingCornerElement(bool* missingCorners, bool& hasMissingCornersOutOfFour){
	bool* missingCornersRowOrColumn;
	if (numOfDivisors==1)
		return 	rotationEnabled ? missingCornerElementForRowOrColumnShapedPuzzleWithRotation() : missingCornerElementForRowOrColumnShapedPuzzle();
	else{
		rotationEnabled ? missingCornerElementForRegularPuzzleWithRotation(missingCorners) : missingCornerElementForRegularPuzzle(missingCorners);
		if (missingCorners[0]==false && missingCorners[1]==false && missingCorners[2]==false && missingCorners[3]==false)
			return missingCorners;
		else{
			missingCornersRowOrColumn = rotationEnabled ? missingCornerElementForRowOrColumnShapedPuzzleWithRotation() : missingCornerElementForRowOrColumnShapedPuzzle();
			if (missingCornersRowOrColumn[0]==false && missingCornersRowOrColumn[1]==false && missingCornersRowOrColumn[2]==false && missingCornersRowOrColumn[3]==false){
				hasMissingCornersOutOfFour = true;
				return missingCornersRowOrColumn;
			}
			else
				delete[] missingCornersRowOrColumn;
		}
	}
	return 	missingCorners;
}

bool jigsawPuzzle :: sumOfEdgesNotZero(){
	int i, j, sum = 0;
	j = rotationEnabled ? NUM_OF_ROTATIONS : 1;
	for (i=0; i<numOfElements; i++){
		sum += elements[i*j]->getElementEdges()->getLeftEdge();
		sum += elements[i*j]->getElementEdges()->getTopEdge();
		sum += elements[i*j]->getElementEdges()->getRightEdge();
		sum += elements[i*j]->getElementEdges()->getBottomEdge();
	}
	return sum!=0;
}

jigsawPuzzle :: ~jigsawPuzzle(){
	int numOfPieces = (rotationEnabled)? numOfElements*NUM_OF_EDGES : numOfElements;	//deleting divisors

	for(int i = 0; i < numOfDivisors; i++)
		delete[] divisors[i];
	delete[] divisors;

	//deleting elements
	for(int i = 0; i < numOfPieces; i++)
		elements[i]->~puzzleElement();
	delete[] elements;

	delete puzzlePiecesManager;
}

