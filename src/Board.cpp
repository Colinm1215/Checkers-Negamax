/*
 * Board.cpp
 *
 *  Created on: Aug 2, 2021
 *      Author: theresesmith
 */

#include "Board.h"
#include "LinkedList.h"
#include <stdio.h>
#include <cmath>
#include <cctype>

Board::Board() {
	// TODO Auto-generated constructor stub
}

void Board::initBoard(char board[8][9]) {
	// TODO Auto-generated constructor stub
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 9; col++) {
			myBoard[row][col] = board[row][col];
			if (col == 8)
				myBoard[row][col] = '\n';
		}
	}
	//myBoard[7][8] = '\n';
}

Board::~Board() {
	// TODO Auto-generated destructor stub
}


bool Board::isJump(CheckerLoc* checker) {
	bool jump = false;
	return jump;
	//stub

}

bool Board::isWin(int color) {
	int lightCount = 0;
	int darkCount = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 9; col++) {
			switch(myBoard[row][col]) {
			case 'r':
				lightCount++;
				break;
			case 'b':
				darkCount++;
				break;
			case 'R':
				lightCount++;
				break;
			case 'B':
				darkCount++;
				break;
			}
		}
	}
	bool win = false;
	switch (color) {
	case 1:
		win = (darkCount == 0);
		if (countMoveList(getPossibleMoves(2)) == 0) win = true;
		break;
	case 2:
		win = (lightCount == 0);
		if (countMoveList(getPossibleMoves(1)) == 0) win = true;
		break;
	}

	return win;
}

void Board::displayBoard()
{
	//myBoard[row][col]
	for (int row = 0; row < 8; row++) {
	  for (int col = 0; col < 9; col++)
	    printf("%c", myBoard[row][col]);
	}
	printf("\n");
}
void Board::printToFile(FILE* fd, LLNode* moveList, Payload* chosenMove, int colorMoving, int gameStateNum)
{
	fprintf(fd, "Turn Number %d :\n", gameStateNum);
	switch(colorMoving) {
	case 1:
		fprintf(fd, "\tIt is RED's turn \n\n");
		break;
	case 2:
		fprintf(fd, "\tIt is BLACK's turn \n\n");
		break;
	}
	fprintf(fd, "\tPossible Moves Were : { ");
	LLNode* moveListTemp = moveList;
	if (moveListTemp->payP) {
		Payload* pay = moveListTemp->payP;
		fprintf(fd, "{ {%d,%d} to {%d,%d} } \n", pay->prevRow, pay->prevCol, pay->row, pay->col);
	}
	if (moveListTemp->next)moveListTemp = moveListTemp->next;
	while(moveListTemp->next) {
		Payload* pay = moveListTemp->payP;
		fprintf(fd, "\t\t\t\t\t\t\t{ {%d,%d} to {%d,%d} } \n", pay->prevRow, pay->prevCol, pay->row, pay->col);
		moveListTemp = moveListTemp->next;
	}
	if (moveListTemp->payP && moveListTemp->prev) {
		Payload* pay = moveListTemp->payP;
		fprintf(fd, "\t\t\t\t\t\t\t{ {%d,%d} to {%d,%d} } }\n", pay->prevRow, pay->prevCol, pay->row, pay->col);
	}
	fprintf(fd, "\n");
	fprintf(fd, "\tChosen Move Was : { {%d,%d} to {%d,%d} } \n\n", chosenMove->prevRow, chosenMove->prevCol, chosenMove->row, chosenMove->col);
	fprintf(fd, "\tBoard Configuration After Move :\n");
	for (int row = 0; row < 8; row++) {
		fprintf(fd, "\t\t");
		for (int col = 0; col < 9; col++)
			fprintf(fd, "%c", myBoard[row][col]);
	}
	fprintf(fd, "\n\n\n");
}

void Board::printToFileInitial(FILE* fd)
{
	fprintf(fd, "Intial Board Configuration:\n");
	fprintf(fd, "\tBoard Configuration :\n");
	for (int row = 0; row < 8; row++) {
		fprintf(fd, "\t\t");
		for (int col = 0; col < 9; col++)
			fprintf(fd, "%c", myBoard[row][col]);
	}
	fprintf(fd, "\n\n");
}

char** Board::getBoard() {
	return (char**) myBoard;
}


bool Board::makeMove(CheckerLoc* checker) {
	bool isDiagonal = (std::abs(checker->prevRow - checker->row)) == (std::abs(checker->prevCol - checker->col));
	int newRow = checker->row;
	int newCol = checker->col;
	int curRow = checker->prevRow;
	int curCol = checker->prevCol;
	if (isDiagonal) {
		if (myBoard[newRow][newCol] == '-') {
			myBoard[newRow][newCol] = myBoard[curRow][curCol];
			myBoard[curRow][curCol] = '-';
			if (std::abs(checker->prevRow - checker->row) == 2) {
				int rB = (checker->row - checker->prevRow)/std::abs(checker->prevRow - checker->row);
				int cB = (checker->col - checker->prevCol)/std::abs(checker->prevCol - checker->col);
				if ((curRow+rB <= 8 && curRow+rB >= 0) && (curCol+cB <= 8 && curCol+cB >= 0)) {
					myBoard[curRow+rB][curCol+cB] = '-';
				}
			}
		}

		if(myBoard[newRow][newCol] == 'r' && newRow == 7) {
			myBoard[newRow][newCol] = 'R';
		}

		if(myBoard[newRow][newCol] == 'b' && newRow == 0) {
			myBoard[newRow][newCol] = 'B';
		}
	}

	return isDiagonal;
}

void Board::copyBoard(Board* boardToCopy) {
	char** boardCopy = boardToCopy->getBoard();
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 9; col++) {
			myBoard[row][col] = *((char*)(boardCopy) + (row * 9) + col);
		}
	}
}

LLNode* Board::getPossibleMoves(int color) {
	int co = 0;
	int inverter = (color % 2 == 0) ? 1 : -1;
	int possibleMoves[4][2] = {{1, 1}, {1,-1}, {-1, 1}, {-1, -1}};
	LLNode* moveList = ll->makeEmptyLinkedList();
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			bool properColor = false;
			char c = myBoard[row][col];
			char oC = 'a';
			char oCK = '-';
			bool isKing = std::isupper(c);
			switch (color) {
			case 1:
				properColor = (c=='r' || c=='R');
				oC = 'b';
				oCK = 'B';
				break;
			case 2:
				properColor = (c=='b' || c=='B');
				oC = 'r';
				oCK = 'R';
				break;
			}

			if (properColor) {
				for (auto move : possibleMoves) {
					if (((move == possibleMoves[0] || move == possibleMoves[1]) && isKing) ^ (!isKing && !((move == possibleMoves[0] || move == possibleMoves[1])))) {
						int newRow = row+(move[0]*inverter);
						int newCol = col+(move[1]*inverter);

						bool validRow = (row < 8 && row >= 0);
						bool validCol = (row < 8 && row >= 0);
						if (validRow && validCol) {
							char otherSpace = myBoard[newRow][newCol];
							if (otherSpace == oC || otherSpace == oCK) {
								newRow = row+(2*move[0]*inverter);
								newCol = col+(2*move[1]*inverter);
								otherSpace = myBoard[newRow][newCol];
								if (otherSpace == '-') {
									co++;
									//printf("co %d\n",co);
									Payload* pay = (Payload*) new Payload;
									pay->color = color;
									pay->king = isKing;
									pay->prevCol = col;
									pay->prevRow = row;
									pay->col = newCol;
									pay->row = newRow;
									ll->savePayload(moveList, pay);
								}
							} else if (otherSpace == '-') {
								co++;
								//printf("co %d\n",co);
								Payload* pay = (Payload*) new Payload;
								pay->color = color;
								pay->king = isKing;
								pay->prevCol = col;
								pay->prevRow = row;
								pay->col = newCol;
								pay->row = newRow;
								ll->savePayload(moveList, pay);
							}
						}
					}
				}
			}
		}
	}
	return moveList;
}

int Board::countMoveList(LLNode* list) {
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
