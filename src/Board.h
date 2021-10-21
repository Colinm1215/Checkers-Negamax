/*
 * Board.h
 *
 *  Created on: Aug 2, 2021
 *      Author: theresesmith
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <stdio.h>
#include "LinkedList.h"

class Board {
public:
	Board();
	void initBoard(char board[9][9]);
	virtual ~Board();
	void displayBoard();
	void printToFile(FILE* fd, LLNode* moveList, Payload* chosenMove, int colorMoving, int gameStateNum);
	char** getBoard();
	bool makeMove(CheckerLoc* checker);
	void copyBoard(Board* newBoard);
	bool isJump(CheckerLoc* checker);
	bool isWin(int color);
	LLNode* getPossibleMoves(int color);
	int countMoveList(LLNode* list);
	void printToFileInitial(FILE* fd);
private:
	char myBoard[8][9];
	LinkedList* ll = new LinkedList();
};

#endif /* BOARD_H_ */
