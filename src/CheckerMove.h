/*
 * CheckerMove.h
 *
 *  Created on: Aug 2, 2021
 *      Author: theresesmith
 */

#ifndef CHECKERMOVE_H_
#define CHECKERMOVE_H_
#include <cfloat>
#include <algorithm>
#include "Board.h"
#define MAXDEPTH 50

class CheckerMove {
public:
	CheckerMove();
	virtual ~CheckerMove();
	double negaMaxAI(Board* board, int d, bool t, double a, double b, int sF, int c);
	CheckerLoc* findBestMove(Board* board, int c, int d, LLNode* firstList);
	float Evaluate(int c, Board* board, bool t);
	LLNode* generateMoves(Board* board, int color, bool t);
	int countMoveList(LLNode* list);
};

#endif /* CHECKERMOVE_H_ */
