/*
 * puzzlePiece.h
 *
 *  Created on: 21 ����� 2017
 *      Author: michal
 */
#include "edges.h"

#ifndef PUZZLEELEMENT_H_
#define PUZZLEELEMENT_H_
#include <iostream>

using namespace std;

#define NUM_OF_CORNERS 4
#define NUM_OF_OPTIONS 256
#define NUM_OF_EDGE_TYPES 4

/**
 * a class saving a jigsaw puzzle element, it's id, edges,
 * rotation angle, and corners it may occupy
*/
class puzzleElement{
private:
	int elementId;
	int rotationAngle;//0, 90, 180 or 270 degrees
	edges* elementEdges;
	int* indicesInArrays; //holds in index i the index of the element in the i-coded array
	bool* goodForCorner; //0-TL, 1-TR, 2-BR, 3-BL

public:
	puzzleElement(int elementNumber, int left, int top, int right, int bottom, int rotationAngle);
	bool isSquare();
	int getElementId() const;
	int getElementRotationAngle() const;
	int getNumOfStraightEdgesInElement() const;
	int getElementIndexInArray(int index) const;
	void setElementIndexInArray(int index, int value);
	const edges* getElementEdges() const;
	const bool* getGoodForCorner() const;

	~puzzleElement();

};




#endif /* PUZZLEELEMENT_H_ */
