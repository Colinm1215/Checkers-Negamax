/*
 * Tests.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Therese
 */

#ifndef TESTS_H_
#define TESTS_H_
#include "Production.h"
#include "CheckerMove.h"
#include <stdio.h>


class Tests {
public:
	Tests();
	virtual ~Tests();
	bool tests();

private:
	bool testReadFile();
	bool testEnqueue();
	bool testGotAdjacencyMatrix();
	bool testMakeLList();
	bool testMakeMove();
	//pedagogical bool testRemoveFromList();
	bool testPrintHistory();
	bool testFileOutput();
	Payload* testAI(Board* theBoard, LLNode* moveList, int c);
};

#endif /* TESTS_H_ */
