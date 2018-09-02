/*
 * edges.h
 *
 *  Created on: 14 Dec 2017
 *      Author: Sivan
 */

#ifndef EDGES_H_
#define EDGES_H_

#define NUM_OF_EDGES 4

/**
 * a model that saves 4 edges that describe a puzzle element
*/
class edges{
private:
	int leftEdge;
	int topEdge;
	int rightEdge;
	int bottomEdge;

public:
	edges(int left, int top, int right, int bottom, int rotationAngle);
	int getLeftEdge() const;
	int getTopEdge() const;
	int getRightEdge() const;
	int getBottomEdge() const;
	~edges();
};

#endif /* EDGES_H_ */
