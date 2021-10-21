/*
 * CheckerMove.cpp
 *
 *  Created on: Aug 2, 2021
 *      Author: theresesmith
 */

#include "CheckerMove.h"
#include <cctype>
#include <algorithm>

CheckerMove::CheckerMove() {
	// TODO Auto-generated constructor stub

}

CheckerMove::~CheckerMove() {
	// TODO Auto-generated destructor stub
}

double CheckerMove::negaMaxAI(Board* board, int d, bool t, double a, double b, int sF, int c) {
	//printf("a %d\n", a);
	//printf("b %d\n", b);
    bool done = false;
	double returnScore = -DBL_MAX;
    if (d <= 0 || board->isWin(c)) {
       float ev = Evaluate(c, board, t);
       returnScore = ev*sF;
       done = true;
    }
    Board newBoard = *board;
    LLNode* moveList = generateMoves(&newBoard, c, t);
    LLNode moveListTemp = *moveList;
    int count = countMoveList(&moveListTemp);
    if (count!=0) {    while (moveListTemp.next && !done) {
    	if (moveListTemp.payP) {
    		newBoard = *board;
			newBoard.copyBoard(board);
			CheckerLoc* checker = (CheckerLoc*) (moveListTemp.payP);
			newBoard.makeMove(checker);
			//printf("rS %f\n", returnScore);
			//printf("a %f\n", a);
			//printf("b %f\n", b);
			//printf("%d\n", d);
			//if (a >= b) printf("cutoff\n");
			//else printf("no\n");
			returnScore = std::max(returnScore, -negaMaxAI(&newBoard, d - 1, (!t), -b, -a, -sF, c));
			//printf("nS %f\n", newScore);
			//printf("a %f\n", a);
			a = std::max(a, returnScore);

			if(a >= b) {
				done = true;
			}

			if (returnScore == DBL_MAX) {
				done = true;
			}

			moveListTemp = *moveListTemp.next;
			//printf("Next Move Branch\n");
    	}
    }
	if (moveListTemp.payP && !done) {
		//printf("Final Move Branch\n");
		newBoard = *board;
		newBoard.copyBoard(board);
		CheckerLoc* checker = (CheckerLoc*) (moveListTemp.payP);
		newBoard.makeMove(checker);
        //newBoard.displayBoard();
		//printf("rS %f\n", returnScore);
		//printf("a %f\n", a);
		//printf("b %f\n", b);
		returnScore = std::max(returnScore, -negaMaxAI(&newBoard, d - 1, (!t), -b, -a, -sF, c));
		//printf("nS %f\n", newScore);
		//printf("a %f\n", a);
		a = std::max(a, returnScore);

		if(a >= b) {
			done = true;
		}

		if (returnScore == DBL_MAX) {
			done = true;
		}
	}
    }

    //printf("Return Score\n");
    //printf("rS %f\n", returnScore);
    return returnScore;
}


float CheckerMove::Evaluate(int c, Board* board, bool t) {
	int other = (c == 2) ? 1 : 2;
	char myChar = (c == 2) ? 'b' : 'r';
	float value = 0.0;
	int inverter = (c == 2) ? -1 : 1;
	bool win = board->isWin((t)? c : other);
	bool otherWin = board->isWin((t)? other : c);
	if (!win && !otherWin) {
		char** gameBoard = board->getBoard();
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				char space = *((char*)(gameBoard) + (row * 8) + col);
				float val = (space == 'r' || space == 'R' || space == 'b' || space == 'B') ? 1.0 : 0.0;
				//printf("val of board %d\n", val);
				float sign = (std::tolower(space) == myChar) ? 1.0 : -1.0;
				float mult = (std::isupper(space)) ? 2.0 : 1.0;
				val *= sign;
				val *= mult;
				value += val;
			}
		}
		//printf("%f\n", value);
	} else  if (win){
		//printf("%d (me) wins\n", c);
		value = DBL_MAX;
	} else if (otherWin) {
		value = -DBL_MAX;
	}
	return value;
}

LLNode* CheckerMove::generateMoves(Board* board, int color, bool t) {
	int otherColor = (color == 1) ? 2 : 1;
	int actualColor = (t) ? color : otherColor;
	//printf("Getting moves for %d\n", actualColor);
	return board->getPossibleMoves(actualColor);
}

CheckerLoc* CheckerMove::findBestMove(Board* board, int c, int d, LLNode* firstList) {
	//printf("Finding bestMove for %d\n", c);
	double bestValue = -DBL_MAX;
	CheckerLoc* bestMove;
    LLNode moveListTemp = *firstList;
    Board newBoard = *board;
    int count = countMoveList(&moveListTemp);
    double a = -128;
    double b = 128;
    bool done = false;
    if (count!=0) {
    while (moveListTemp.next && bestValue < DBL_MAX && !done) {
		newBoard.copyBoard(board);
		CheckerLoc* checker = (CheckerLoc*) (moveListTemp.payP);
		newBoard.makeMove(checker);
		if (!newBoard.isWin(c)) {
			//newBoard.displayBoard();
			//printf("We score {%d,%d} to {%d,%d}\n", checker->prevRow, checker->prevCol, checker->row, checker->col);
			double newVal = -negaMaxAI(&newBoard, d-1, false, -b, -a, -1, c);
			//printf("val %f\n", newVal);
			//printf("Move is {%d,%d} to {%d,%d} scored at %f\n", checker->prevRow, checker->prevCol, checker->row, checker->col, newVal);

			a = std::max(a, newVal);
			if (newVal >= bestValue) {
				bestValue = newVal;
				bestMove = checker;
			}

			if (a >= b) done = true;

			moveListTemp = *moveListTemp.next;
		} else {
			bestMove = checker;
			bestValue = DBL_MAX;
		}
    }
    if (bestValue < DBL_MAX && !done) {
    newBoard = *board;
    newBoard.copyBoard(board);
	CheckerLoc* checker = (CheckerLoc*) (moveListTemp.payP);
    newBoard.makeMove(checker);
	double newVal = -negaMaxAI(&newBoard, d-1, false,  -b, -a, -1, c);
    //printf("Move is {%d,%d} to {%d,%d} scored at %f\n", checker->prevRow, checker->prevCol, checker->row, checker->col, newVal);

	if (newVal >= bestValue) {
		bestValue = newVal;
		bestMove = checker;
	}
	if (a >= b) done = true;
    }
    }

    //printf("Chosen Move is {%d,%d} to {%d,%d} scored at %f\n", bestMove->prevRow, bestMove->prevCol, bestMove->row, bestMove->col, bestValue);
    //printf("count %d \n", count);
	return bestMove;
}

int CheckerMove::countMoveList(LLNode* list) {
	LLNode* countTemp = list;
	int count = 0;
	while (countTemp->next) {
		if (countTemp->payP) {
		count++;
		}
		countTemp = countTemp->next;
	}
	if (countTemp->payP) {
		count++;
	}
	return count;
}
