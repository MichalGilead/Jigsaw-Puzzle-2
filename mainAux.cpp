/*
 * mainAux.cpp
 *
 *  Created on: 16 ����� 2017
 *      Author: michal
 */

#include "mainAux.h"

bool isRotate(const string& str){
	if (str=="-rotate")
		return true;
	return false;
}

bool isValidEdge(const string& str){
	if (str=="0"||str=="1"||str=="-1")
		return true;
	return false;
}

bool isPositiveNum(const string& str){
	for(unsigned int i=0; i< str.size(); i++)
		if ((i==0 && str[i]==48) || (str[i]<48||str[i]>57))
			return false;
	return true;
}

void writeSolutionToOutputFile(ofstream& fout, int*** solvedPuzzle, int height, int width){
	if (solvedPuzzle==NULL)
		fout << "Cannot solve puzzle: it seems that there is no proper solution" << endl;
	else{
		for (int i=0; i<height; i++){
			for (int j=0; j<width; j++){
				fout << solvedPuzzle[i][j][0];//print element id
				if (solvedPuzzle[i][j][1]!=0)
					fout << " [" << solvedPuzzle[i][j][1] << "]";//print element rotation angle
				if (j!=width-1)
					fout << " ";
			}
			fout << endl;
		}
	}
}

bool checkMissingCorners(ofstream& fout, bool* missingCorners){
	bool res = false;
	string corners[NUM_OF_CORNERS] = {"TL", "TR", "BR", "BL"};
	if (missingCorners!=NULL){
		for (int i=0; i<NUM_OF_CORNERS; i++){
			if (missingCorners[i]){
				fout << "Cannot solve puzzle: missing corner element: " << corners[i] << endl;
				res = true;
			}
		}
		delete[] missingCorners;
	}
	return res;
}

bool writeUnsolvableToOutputFile(ofstream& fout, jigsawPuzzle* puzzle, bool* missingCorners, bool& hasMissingCornersOutOfFour){
	bool unsolvable = false, *solution;
	if (puzzle->getWrongNumOfStraightEdges()){//top straight edges don't equal to bottom straight edges or left aren't equal to right
		fout << "Cannot solve puzzle: wrong number of straight edges" << endl;
		unsolvable = true;
	}

	if (puzzle->notEnoughStraightEdgesForFrame()){//number of straight edges is lower than minimal frame
		fout << "Cannot solve puzzle: not enough straight edges for frame" << endl;
		unsolvable = true;
	}

	solution = puzzle->missingCornerElement(missingCorners, hasMissingCornersOutOfFour);//gets a boolean array of the missing corners

	if (checkMissingCorners(fout, solution))
		unsolvable = true;

	if (puzzle->sumOfEdgesNotZero()){
		if (hasMissingCornersOutOfFour)//we tried to continue to the algorithm to check if there's a row/column solution because there are no 4 corners
			checkMissingCorners(fout, missingCorners);//write the missing corners out of the four
		fout << "Cannot solve puzzle: sum of edges is not zero" << endl;
		unsolvable = true;
	}

	return unsolvable;
}


bool checkValidFirstLine(ofstream& fout, int& numOfElements, string line){
	string tmp;
	stringstream ss, ss2;
	int i=0, wordsCnt=0;
	ss << line;
	while (getline(ss, tmp, '=')){
		ss2.str("");
		ss2.clear();
		ss2 << tmp;
		ss2 >> tmp;
		while (ss2>>tmp)//check the only words are NumElements, =, and a number
			wordsCnt++;

		if ((i==0 && tmp!="NumElements") || (i==1 && !isPositiveNum(tmp)) || i>1 || wordsCnt>0){
			fout << "Wrong format of first line of arguments" << endl;
			return false;
		}
		if (i==1)
			numOfElements = stoi(tmp);
		i++;
	}
	return true;
}

void handleInvalidId(string tmp, bool hasInvalidId, string& invalidIdsError){
	if (hasInvalidId){//append to string of invalid ids if already created
		invalidIdsError.append(", ");
		invalidIdsError.append(tmp);
	}
	else{//create a string of invalid ids and append to it
		hasInvalidId = true;
		invalidIdsError = "Puzzle cannot have the following IDs: ";
		invalidIdsError.append(tmp);
	}
}

void handleOutOfRangeId(string tmp, bool hasIdsOutOfRange, string& IdsOutOfRangeError, int numOfElements){
	if (hasIdsOutOfRange){//append to string of out of range ids if already created
		IdsOutOfRangeError.append(", ");
		IdsOutOfRangeError.append(tmp);
	}
	else{//create a string of out of range ids and append to it
		hasIdsOutOfRange = true;
		IdsOutOfRangeError = "Puzzle of size ";
		IdsOutOfRangeError.append(to_string(numOfElements));
		IdsOutOfRangeError.append(" cannot have the following IDs: ");
		IdsOutOfRangeError.append(tmp);
	}
}

bool checkWrongElementId(string tmp, int numOfElements, bool hasInvalidId, string& invalidIdsError, bool hasIdsOutOfRange, string& IdsOutOfRangeError){
	if (!isPositiveNum(tmp)){//invalid id
		handleInvalidId(tmp, hasInvalidId, invalidIdsError);
		return true;
	}
	if (stoi(tmp)>numOfElements){//id is out of range
		handleOutOfRangeId(tmp, hasIdsOutOfRange, IdsOutOfRangeError, numOfElements);
		return true;
	}
	return false;
}

bool checkValidEdges(int i, string tmp, int& left, int& top, int& right, int& bottom){
	if (i>NUM_OF_EDGES || !isValidEdge(tmp)){//if we already have 4 edges of this element or edge is invalid
		return false;
	}
	//save valid edges
	left = (i==1) ? stoi(tmp) : left;
	top = (i==2) ? stoi(tmp) : top;
	right = (i==3) ? stoi(tmp) : right;
	bottom = (i==4) ? stoi(tmp) : bottom;
	return true;
}

jigsawPuzzle* readFromInputFile(ifstream& fin, ofstream& fout, bool rotationEnabled){
	string line, tmp, tmp2, IdsOutOfRangeError, invalidIdsError;
	bool hasIdsOutOfRange = false, hasInvalidId = false, invalidRead = false;
	stringstream ss, ss2;
	int i, numOfElements = 0, elementId = 0, left, top, right, bottom, validElements = 0, numOfGivenElements = 0, rotationEnabledInt = rotationEnabled ? NUM_OF_ROTATIONS : 1;
	getline(fin, line);
	checkValidFirstLine(fout, numOfElements, line);

	//creating an elements array - if rotation is enabled the number of elements is multiplied by 4
	puzzleElement **elements = new puzzleElement*[numOfElements*rotationEnabledInt];
	for (i=0; i<numOfElements*rotationEnabledInt; i++)
		elements[i] = NULL;
	bool *givenElements = new bool[numOfElements];
	fill_n(givenElements, numOfElements, false);
	string *wrongElements = new string[numOfElements];
	fill_n(wrongElements, numOfElements, "");

	while (getline(fin, line)){//read elements
		ss.str("");
		ss.clear();
		ss << line;
		invalidRead = false;
		i=0;
		while (getline(ss, tmp, ' ')){//read specific element
			if (i==0){//check valid element id
				if (checkWrongElementId(tmp, numOfElements, hasInvalidId, invalidIdsError, hasIdsOutOfRange, IdsOutOfRangeError)){
					invalidRead = true;
					break;
				}
				else{//valid id
					elementId = stoi(tmp);
					if (!givenElements[elementId-1]){//add to given elements if not already given
						givenElements[elementId-1]= true;
						numOfGivenElements++;
					}
					else{//print error - id was given more than once
						fout << "Puzzle ID " << elementId << " was given more than once" << endl;
						invalidRead = true;
						break;
					}
				}
			}
			else if (!checkValidEdges(i, tmp, left, top, right, bottom)){//check element's edges
					wrongElements[elementId-1].append(line);//append the whole line to the wrong formatted elements array
					invalidRead = true;
					break;
			}
			i++;
			if (invalidRead){//stop reading line if it's already invalid
				break;
			}
		}

		if (i>0 && !invalidRead){
			if (i<NUM_OF_EDGES+1)//if we received less than 4 edges of this element
				wrongElements[elementId-1].append(line);//append the whole line to the wrong formatted elements array
			else{//valid element
				if (rotationEnabled){//create an element for each rotation of the piece
					for (i=0; i<NUM_OF_ROTATIONS; i++)
						elements[(elementId-1)*rotationEnabledInt + i] = new puzzleElement(elementId, left, top, right, bottom, i);
				}
				else //create element and add to elements array
					elements[elementId-1] = new puzzleElement(elementId, left, top, right, bottom, 0);
				validElements++;
			}
		}
	}
	if (numOfGivenElements==numOfElements && !hasIdsOutOfRange && !hasInvalidId && validElements==numOfElements){//if we got all elements correctly
		delete[] givenElements;
		delete[] wrongElements;
		jigsawPuzzle* puzzle = new jigsawPuzzle(numOfElements, elements, rotationEnabled);//create puzzle from elements
		return puzzle;
	}

	if (numOfGivenElements < numOfElements){//we didn't get all elements mentioned in the first line
		fout << "Missing puzzle element(s) with the following IDs: ";
		for (i=0; i<numOfElements; i++){ //print to output file all missing ids
			if (!givenElements[i]){
				numOfGivenElements++;
				if (numOfGivenElements < numOfElements){
					fout << i+1 << ", ";
				}
				else
					fout << i+1 << endl;
			}
		}
	}
	if (hasIdsOutOfRange)//print to output file out of range ids if were found
		fout << IdsOutOfRangeError << endl;
	if (hasInvalidId)//print to output file invalid ids if were found
		fout << invalidIdsError << endl;
	if (validElements!=numOfElements){//print to output file the line of invalid elements if were found
		for (i=0; i<numOfElements; i++){
			if (wrongElements[i]!=""){
				fout << "Puzzle ID " << i+1 << " has wrong data: " << wrongElements[i] << endl;
			}
		}
	}

	delete[] givenElements;
	delete[] wrongElements;
	for (i=0; i<numOfElements; i++){
		if (elements[i]!=NULL)
			delete elements[i];
	}
	delete[] elements;
	return NULL;//no puzzle was made
}

bool handleOneElement(ofstream& fout, jigsawPuzzle* puzzle){
	if (puzzle->getNumOfElements() > 1)
		return false;

	if (puzzle->getElements()[0]->isSquare())
		fout << "1" << endl;

	else {
		if (puzzle->getWrongNumOfStraightEdges())
			fout << "Cannot solve puzzle: wrong number of straight edges" << endl;
		if (puzzle->notEnoughStraightEdgesForFrame())
			fout << "Cannot solve puzzle: not enough straight edges for frame" << endl;


		if (!puzzle->getElements()[0]->getGoodForCorner()[0])
			fout << "Cannot solve puzzle: missing corner element: TL" << endl;
		else if (!puzzle->getElements()[0]->getGoodForCorner()[1])
			fout << "Cannot solve puzzle: missing corner element: TR" << endl;
		else if (!puzzle->getElements()[0]->getGoodForCorner()[2])
			fout << "Cannot solve puzzle: missing corner element: BR" << endl;
		else if (!puzzle->getElements()[0]->getGoodForCorner()[3])
			fout << "Cannot solve puzzle: missing corner element: BL" << endl;
		if (puzzle->sumOfEdgesNotZero())
			fout << "Cannot solve puzzle: sum of edges is not zero" << endl;
	}
	return true;


}


