/*
 * Tests.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: Therese
 */

#include "Tests.h"
#include "CheckerMove.h"
#include <stdbool.h>
#include <iostream>

Tests::Tests() {
	// TODO Auto-generated constructor stub

}

Tests::~Tests() {
	// TODO Auto-generated destructor stub
}

bool Tests::tests()
{
	bool answer = true;
	bool ok1 = testReadFile();
	bool ok5 = testMakeMove();
	bool ok3 = testMakeLList();
	bool ok2 = testFileOutput();
	bool ok4 = testEnqueue();
	//pedagogical bool ok5 = testRemoveFromList();
	bool ok6 = testPrintHistory();
	answer = ok1 && ok3 && ok4  && ok6 && ok5;
	return answer;
}

bool Tests::testReadFile()
{
	puts("starting testReadFile"); fflush(stdout);
	bool ok = true;
	//the file tells how many rooms there are
	int answer = -1;
	int rightAnswer = 8;


	Board* theBoard = new Board();


	Production* pP = new Production();


	ok = pP->readFile("gameState.txt", theBoard); //read the file
	theBoard->displayBoard();

	ok = pP->getYesNo("Does the board look right?");
	if(ok)
	{
		puts("testReadfile did pass");
	}
	else
	{
		puts("testReadfile did not pass.");
	}

	return ok;
}
bool Tests::testEnqueue()
{
	bool ok = true;
	if(ok)
	{
		puts("testEnqueue did pass");
	}
	else
	{
		puts("testEnqueue did not pass.");
	}
	return ok;
}
bool Tests::testGotAdjacencyMatrix()
{
	bool ok = true;
	if(ok)
	{
		puts("testGotAdjacencyMatrix did pass");
	}
	else
	{
		puts("testGotAdjacencyMatrix did not pass.");
	}
	return ok;
}

bool Tests::testMakeLList()
{
	bool ok = true;
	LinkedList* ll = new LinkedList();
	LLNode* node = ll->makeEmptyLinkedList();
	Payload* pay = (Payload*) malloc(sizeof(Payload));
	pay->col = 1;
	pay->row = 1;
	ll->savePayload(node, pay);
	Payload* pay2 = (Payload*) malloc(sizeof(Payload));
	pay2->col = 2;
	pay2->row = 2;

	if (node->payP) ok = true;

	ll->savePayload(node, pay2);

	if (node->next->payP) ok = true;

	if(ok)
	{
		puts("testMakeLList did pass");
	}
	else
	{
		puts("testMakeLList did not pass.");
	}
	return ok;
}
bool Tests::testPrintHistory()
{
	bool ok = true;
	if(ok)
	{
		puts("testPrintHistory did pass");
	}
	else
	{
		puts("testPrintHistory did not pass.");
	}
	return ok;
}
bool Tests::testFileOutput()
{
	bool ok1 = true;
	bool ok2 = true;

	FILE* fd = fopen("boardPlay.txt", "w");

	Board* theBoard = new Board();


	Production* pP = new Production();


	pP->readFile("gameState.txt", theBoard); //read the file
	printf("We have opened and read our input file, and opened our output file\n");
	printf("The Board from our input file is :\n");
	theBoard->displayBoard();
	printf("Red moves first\n");
	theBoard->printToFileInitial(fd);
	LLNode* moveList = theBoard->getPossibleMoves(1);
	Payload* chosenMove = testAI(theBoard, moveList, 1);
	theBoard->makeMove(chosenMove);
	printf("After red moves our new board is :\n");
	theBoard->displayBoard();
	theBoard->printToFile(fd, moveList, chosenMove, 1, 1);

    ok1 = pP->getYesNo("Was the output file successfully and correctly updated?");
	printf("Now black moves\n");
	moveList = theBoard->getPossibleMoves(2);
	chosenMove = testAI(theBoard, moveList, 2);
	theBoard->makeMove(chosenMove);
	printf("After black moves our new board is :\n");
	theBoard->displayBoard();
	theBoard->printToFile(fd, moveList, chosenMove, 2, 2);
	ok2 = pP->getYesNo("Was the output file successfully and correctly updated?");
    fclose(fd);
	if(ok1 && ok2)
	{
		puts("testFileOutput did pass");
	}
	else
	{
		puts("testFileOutput did not pass.");
	}
	return ok1 && ok2;
}

Payload* Tests::testAI(Board* theBoard, LLNode* moveList, int c) {


	Production* pP = new Production();

	Payload* returnMove = (Payload*)0;
	Board newBoard = *theBoard;

	LLNode* moveListTemp = moveList;
	moveListTemp = moveListTemp->next;
	while (moveListTemp->next) {
		if (moveListTemp->payP) {
			Payload* p = moveListTemp->payP;
			printf("We can move from {%d,%d} to {%d,%d}\n", p->prevRow, p->prevCol, p->row, p->col);
		}
		moveListTemp = moveListTemp->next;
	}
	if (moveListTemp->payP) {
		Payload* p = moveListTemp->payP;
		printf("We can move from {%d,%d} to {%d,%d}\n", p->prevRow, p->prevCol, p->row, p->col);
	}
	CheckerMove* moverAI = (CheckerMove*) new CheckerMove();
	printf("Now we find the best move from the AI\n");
	moveListTemp = newBoard.getPossibleMoves(c);
	printf("first Count %d \n", moverAI->countMoveList(moveListTemp));
	CheckerLoc* move;
	move = moverAI->findBestMove(&newBoard, c, 5, moveListTemp);

	if (move->prevRow && move->prevCol && move->row && move->col)
		printf("Our Chosen move is {%d,%d} to {%d,%d}\n", move->prevRow, move->prevCol, move->row, move->col);
	theBoard->makeMove(move);
	printf("Our board after the move\n");
	theBoard->displayBoard();
	returnMove = move;
	if (!pP->getYesNo("Was a move succesfully chosen, and the board correctly updated? :")) returnMove = (Payload*)0;

	if(returnMove)
	{
		puts("testAI did pass");
	}
	else
	{
		puts("testAI did not pass.");
	}
	return returnMove;
}

bool Tests::testMakeMove() {
	printf("Testing the ability to generate all possible valid moves, and then move a piece\n");
	Production* pP = new Production();
	Board* board = new Board();
	pP->readFile("gameState.txt", board);

	printf("Initial Board \n\n");
	board->displayBoard();
	printf("We get a move and perform it \n");

	LLNode* moves = board->getPossibleMoves(1);
	board->makeMove(moves->payP);
	printf("Board after move \n");
	board->displayBoard();

	bool ok = pP->getYesNo("Was a valid move found and performed?");

	if (ok) printf("testMakeMove passed\n");
	else printf("testMakeMove failed\n");

	return ok;
}


