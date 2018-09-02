/*
 * edges.cpp
 *
 *  Created on: 14 Dec 2017
 *      Author: Sivan
 */

#include "edges.h"

edges :: edges(int left, int top, int right, int bottom, int rotationAngle){
	int edges[NUM_OF_EDGES];
	int result[NUM_OF_EDGES];
	int numOfRotations;
	edges[0] = left;
	edges[1] = top;
	edges[2] = right;
	edges[3] = bottom;

	numOfRotations = rotationAngle/90;

	result[(0+numOfRotations)%NUM_OF_EDGES] = edges[0];
	result[(1+numOfRotations)%NUM_OF_EDGES] = edges[1];
	result[(2+numOfRotations)%NUM_OF_EDGES] = edges[2];
	result[(3+numOfRotations)%NUM_OF_EDGES] = edges[3];

	leftEdge = result[0];
	topEdge = result[1];
	rightEdge = result[2];
	bottomEdge = result[3];
}

int edges :: getLeftEdge() const{
	return leftEdge;
}

int edges :: getTopEdge() const{
	return topEdge;
}
int edges :: getRightEdge() const{
	return rightEdge;
}
int edges :: getBottomEdge() const{
	return bottomEdge;
}

edges :: ~edges(){
}
