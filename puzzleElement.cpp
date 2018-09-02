/*
 * puzzlePiece.cpp
 *
 *  Created on: 21 ����� 2017
 *      Author: michal
 */

#include "puzzleElement.h"

puzzleElement :: puzzleElement(int elementNumber, int left, int top, int right, int bottom, int rotation){
	elementId = elementNumber;
	rotationAngle = rotation*90;
	elementEdges = new edges(left, top, right, bottom, rotationAngle);
	goodForCorner = new bool[NUM_OF_CORNERS];
	goodForCorner[0] = (elementEdges->getTopEdge()==0&&elementEdges->getLeftEdge()==0) ? true : false;
	goodForCorner[1] = (elementEdges->getTopEdge()==0&&elementEdges->getRightEdge()==0) ? true : false;
	goodForCorner[2] = (elementEdges->getBottomEdge()==0&&elementEdges->getRightEdge()==0) ? true : false;
	goodForCorner[3] = (elementEdges->getBottomEdge()==0&&elementEdges->getLeftEdge()==0) ? true : false;
	indicesInArrays = new int[NUM_OF_OPTIONS];
}

int puzzleElement :: getElementId() const{
	return elementId;
}

int puzzleElement :: getElementRotationAngle() const{
	return rotationAngle;
}

const edges* puzzleElement :: getElementEdges() const{
	return elementEdges;
}

const bool* puzzleElement :: getGoodForCorner() const{
	return goodForCorner;
}

bool puzzleElement :: isSquare(){
	return (goodForCorner[0] && goodForCorner[1] && goodForCorner[2] && goodForCorner[3]);
}

int puzzleElement :: getNumOfStraightEdgesInElement() const{
	int numOfStraihtEdges = 0;
	numOfStraihtEdges += elementEdges->getLeftEdge()==0 ? 1 : 0;
	numOfStraihtEdges += elementEdges->getTopEdge()==0 ? 1 : 0;
	numOfStraihtEdges += elementEdges->getRightEdge()==0 ? 1 : 0;
	numOfStraihtEdges += elementEdges->getBottomEdge()==0 ? 1 : 0;
	return numOfStraihtEdges;
}

int puzzleElement :: getElementIndexInArray(int index) const{
	if (index>=0&&index<NUM_OF_OPTIONS)
		return indicesInArrays[index];
	return -1;
}

void puzzleElement :: setElementIndexInArray(int index, int value){
	if (index>=0&&index<NUM_OF_OPTIONS)
		indicesInArrays[index] = value;
}

puzzleElement :: ~puzzleElement(){
	delete[] goodForCorner;
	delete elementEdges;
	delete[]indicesInArrays;
}
