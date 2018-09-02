/*
 * mainAux.h
 *
 *  Created on: 16 בדצמ× 2017
 *      Author: michal
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "jigsawPuzzle.h"

/**
 * checks if a string is a rotate argument
 *
 * @param - str
 * @return - true if string is -rotate, false otherwise
*/
bool isRotate(const string& str);

/**
 * checks if a string is a valid edge - meaning one of the numbers -1,0,1
 *
 * @param - str
 * @return - true if string is -1, 0 or 1, false otherwise
*/
bool isValidEdge(const string& str);

/**
 * checks if a string is a valid positive number
 *
 * @param - str
 * @return - true if string is a positive number, false otherwise
*/
bool isPositiveNum(const string& str);

/**
 * prints the puzzle solution to the output file
 *
 * @params - fout - the output file, solvedPuzzle - a matrix of the puzzle solution,
 * height, width - puzzle dimensions
 *
*/
void writeSolutionToOutputFile(ofstream& fout, int*** solvedPuzzle, int height, int width);

/**
 * prints an error to the output file in case of missing corner
 *
 * @params - fout - the output file, missingCorners - for each corner saves true if missing, otherwise false
 *
 * @return - true if there was a missing corner in array, false otherwise
 *
*/
bool checkMissingCorners(ofstream& fout, bool* missingCorners);

/**
 * prints a matching error to the output file in case of missing corner,
 * wrong number of straight edges, not enough straight edges for frame,
 * or sum of edges not zero
 *
 * @params - fout - the output file, missingCorners - for each corner saves true if missing, otherwise false,
 * puzzle - the puzzle elements, hasMissingCornersOutOfFour - sets to true if there weren't 4 corners but
 * there were 2 corners for a row/column solution
 *
 * @return - true if there was an error in the puzzle, false otherwise
 *
*/
bool writeUnsolvableToOutputFile(ofstream& fout, jigsawPuzzle* puzzle, bool* missingCorners, bool& hasMissingCornersOutOfFour);

/**
 * checks validity of the data in the input file
 * saves valid date to elements and creates puzzle if all elements were given correctly
 * or prints errors to output file if there were invalid arguments or missing elements
 *
 * @params - fin - the input file, fout - the output file, rotationEnabled - boolean to know
 * 			if we should create more elements for rotations
 *
 * @return - the puzzle that was given, NULL if there was invalid or missing data
 *
*/
jigsawPuzzle* readFromInputFile(ifstream& fin, ofstream& fout, bool rotationEnabled);

/**
 * checks if the puzzle was given only one element and
 * solves it and prints solution to output file
 * or prints error to output file if it's unsolvable
 *
 * @params - fout - the output file, puzzle - the puzzle elements
 *
 * @return - true if the puzzle has one element, false otherwise
 *
*/
bool handleOneElement(ofstream& fout, jigsawPuzzle* puzzle);



#endif /* MAINAUX_H_ */
