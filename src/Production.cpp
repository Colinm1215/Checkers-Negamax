/*
 * Production.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: Therese
 */

#include "Production.h"
#include "LinkedList.h"
#include "CheckerMove.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>


Production::Production() {
	// TODO Auto-generated constructor stub

}

Production::~Production() {
	// TODO Auto-generated destructor stub
}

bool Production::prod(int argc, char* argv[])
{
	printf("Starting Production\n");
	bool answer = false;

	if(argc <=1) //no interesting information
	{
		puts("Didn't find any arguments.");
		fflush(stdout);
		answer = false;
	}
	else //there is interesting information
	{
		printf("Found %d interesting arguments.\n", argc-1);
		fflush(stdout);
		char filename[FILENAMELENGTHALLOWANCE];
		char* eptr=(char*) malloc(sizeof(char*));
		long aL=-1L;
		int numTurns;
		int color;

		for(int i = 1; i<argc; i++) //don't want to read argv[0]
				{//argv[i] is a string

					switch(i)
					{
					case 1:
						//this is filename
						printf("The length of the filename is %d.\n",(int)strlen(argv[i]));
						printf("The proposed filename is %s.\n", argv[i]);
						if(strlen(argv[i])>=FILENAMELENGTHALLOWANCE)
						{
							puts("Filename is too long.");
							fflush(stdout);
							answer = false;
						}
						else
						{
							strcpy(filename, argv[i]);
							printf("Filename was %s.\n", filename);
							fflush(stdout);
						}
						break;
					case 2:
						aL = strtol(argv[i], &eptr, 10);
						color = (int) aL;
						switch (color) {
						case 1:
							printf("Color is red\n");fflush(stdout);
							break;
						case 2:
							printf("Color is black\n");fflush(stdout);
							break;
						}
						break;
					case 3:
						//this is maximum amount of treasure

						aL = strtol(argv[i], &eptr, 10);
						numTurns = (int) aL;
						printf("Number of turns is %d\n",numTurns);fflush(stdout);
						break;

					default:
						puts("Unexpected argument count."); fflush(stdout);
						answer = false;
						break;
					}//end of switch
				}//end of for loop on argument count
				puts("after reading arguments"); fflush(stdout);

		//we'll want to read the file
	    Board* theBoard = new Board();
	    CheckerMove* moverAI = (CheckerMove*) new CheckerMove();

		puts("Before read file"); fflush(stdout);
		answer = readFile(filename, theBoard); //read the file
		FILE* fd = fopen("boardPlay.txt", "w");
		theBoard->printToFileInitial(fd);
		puts("Back from read file"); fflush(stdout);
		printf("\n\n");
		int turnsPassed = 0;
		int whoseTurn = 2;
		int otherColor = (color == 1) ? 2 : 1;
		printf("Lets Begin\n");
		while (numTurns > 0) {
			turnsPassed++;
			printf("Board :\n");
			theBoard->displayBoard();
			printf("\n");
			LLNode* moveList;
			if (whoseTurn == color) {
				printf("My Turn!\n");
				moveList = moverAI->generateMoves(theBoard, color, true);
				if (theBoard->countMoveList(moveList) != 0) {
					CheckerLoc* move = moverAI->findBestMove(theBoard, color, 5, moveList);
					theBoard->makeMove(move);
					theBoard->printToFile(fd, moveList, move, color, turnsPassed);
					printf("Move was {%d,%d} to {%d,%d}\n", move->prevRow, move->prevCol, move->row, move->col);
				}
			} else {
				printf("Other Player's Turn!\n");
				moveList = moverAI->generateMoves(theBoard, otherColor, true);
				if (theBoard->countMoveList(moveList) != 0) {
					//CheckerLoc* move = moverAI->findBestMove(theBoard, otherColor, 5, moveList);
					CheckerLoc* move = getOtherPlayerMove(otherColor, moveList);
					theBoard->makeMove(move);
					theBoard->printToFile(fd, moveList, move, otherColor, turnsPassed);
				}
			}

			if (theBoard->isWin(1)) {
				printf("RED wins and BLACK loses!\n");
				numTurns = 0;
				fprintf(fd, "RED wins");
			} else if (theBoard->isWin(2)) {
				printf("BLACK wins and RED loses!\n");
				numTurns = 0;
				fprintf(fd, "BLACK wins");
			}
			numTurns--;
			whoseTurn = (whoseTurn == 1) ? 2 : 1;
		}

		if (numTurns <= 0) {
			if (!theBoard->isWin(1) && !theBoard->isWin(2)) {
				printf("No Turns Left! No one wins!");
				fprintf(fd, "The Game ended due to no turns left. No one won!");
			}
		}

		printf("Final Board : \n");
		theBoard->displayBoard();


		fclose(fd);

	}
	return answer;
}

bool Production::readFile(char* filename, Board* theBoard)
{
	bool ok = true;
	char temp = '-';
	FILE* fp = fopen(filename, "r"); //read the file

	if (fp == NULL)
	{
		puts("Error! opening file");

	}
	else
	{
		LinkedList* ll = new LinkedList();
		char newBoard[8][9];
		LLNode* makeBoard = ll->makeEmptyLinkedList();
		//TODO read the board from the file,
		//discover checkers
		//make instances of class checker as needed
		//board needs to keep track of instances of checkers
		//checkers might also know where they are...
		for (int row = 0; row <= 8; row++) {
			//printf("r %d \n", row);
			for (int col = 0; col <= 8; col++) {
				fscanf(fp,"%c", &temp); //red checker, black, none?
				//notice, this reads only one character, and <CR> is a character
				newBoard[row][col] = temp;
			}
		}

		theBoard->initBoard(newBoard);

	}
	fclose(fp);

	return ok;
}

bool Production::getYesNo(char* query)
{
   bool answer = true; //so far
   char said = 'x';
   do
   {
	 printf("%s (y/n):",query);
     fflush(stdout);
     fflush(stdin);
     scanf("%c",&said);
   }while((said!= 'y')&&(said!='n'));
   if(said=='n')
   {
	   answer=false;
   }


   return answer;
}


Payload* Production::getOtherPlayerMove(int color, LLNode* list) {
	int moveSet[2][2];
	switch (color) {
	case 1:
		printf("What move does RED make?\n");
		break;
	case 2:
		printf("What move does BLACK make?\n");
		break;
	}
	Payload* move;
	bool moveValid = false;
	do {
		printf("What is the initial row of the piece you wish to move? : ");
		fflush(stdout);
		fflush(stdin);
		scanf("%d",&moveSet[0][0]);
		printf("What is the initial col of the piece you wish to move? : ");
		fflush(stdout);
		fflush(stdin);
		scanf("%d",&moveSet[0][1]);
		printf("What row do you wish to move it to? : ");
		fflush(stdout);
		fflush(stdin);
		scanf("%d",&moveSet[1][0]);
		printf("What col do you wish to move it to? : ");
		fflush(stdout);
		fflush(stdin);
		scanf("%d",&moveSet[1][1]);
		printf("Move chosen is {%d,%d} to {%d,%d}\n", moveSet[0][0], moveSet[0][1], moveSet[1][0], moveSet[1][1]);
		moveValid = moveInList(list, moveSet);
		if (!moveValid) printf("Invalid Move! Please try again \n");
	} while (!moveValid);
	move = getMoveFromList(list, moveSet);
    return move;
}

Payload* getMoveFromList(LLNode* list, int moveSet[2][2]) {
	Payload* move;
	LLNode* listTemp = list;
	while (listTemp->next) {
		if (listTemp->payP) {
			Payload* pay = listTemp->payP;
			if (pay->prevRow == moveSet[0][0] && pay->prevCol==moveSet[0][1])
				if (pay->row == moveSet[1][0] && pay->col==moveSet[1][1])
					move = pay;
		}
		listTemp = listTemp->next;
	}
	if (listTemp->payP) {
		Payload* pay = listTemp->payP;
		if (pay->prevRow == moveSet[0][0] && pay->prevCol==moveSet[0][1])
			if (pay->row == moveSet[1][0] && pay->col==moveSet[1][1])
				move = pay;
	}

	return move;
}

bool moveInList(LLNode* list, int moveSet[2][2]) {
	bool answer = false;
	LLNode* listTemp = list;
	while (listTemp->next) {
		if (listTemp->payP) {
			Payload* pay = listTemp->payP;
			if (pay->prevRow == moveSet[0][0] && pay->prevCol==moveSet[0][1])
				if (pay->row == moveSet[1][0] && pay->col==moveSet[1][1])
					answer = true;
		}
		listTemp = listTemp->next;
	}
	if (listTemp->payP) {
		Payload* pay = listTemp->payP;
		if (pay->prevRow == moveSet[0][0] && pay->prevCol==moveSet[0][1])
			if (pay->row == moveSet[1][0] && pay->col==moveSet[1][1])
				answer = true;
	}

	return answer;
}

