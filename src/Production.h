/*
 * Production.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Therese
 */

#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#include <stdio.h>
#include <stdbool.h>
#include <string.h>//strncpy
#include <stdlib.h>//strtol
#include "Board.h"
#include "LinkedList.h"


#define FILENAMELENGTHALLOWANCE 50

class Production {
public:
	Production();
	virtual ~Production();
	bool prod(int argc, char* argv[]);
	bool readFile(char*, Board*);
	bool getYesNo(char* query);
	Payload* getOtherPlayerMove(int color, LLNode* list);

private:
	LinkedList* ll = new LinkedList();
	int myColor;
};
bool moveInList(LLNode* list, int moveSet[2][2]);
Payload* getMoveFromList(LLNode* list, int moveSet[2][2]);
#endif /* PRODUCTION_H_ */
