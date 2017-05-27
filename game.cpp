#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "game.h"

extern int insideBoardnum;
extern int selectedBoard;
extern unsigned int numturn;
extern bool hasChosenDirection;
extern bool hasPlayed;
extern bool winner;
extern bool isAIturn;
extern bool pvpmode;
extern bool pvsaimode;
extern bool aivspmode;
extern e_Color **pBoard;

e_Color ** createpCopy(e_Color **copy)
{
	e_Color **res;

	res = initpBoard();
	for(size_t i = 0; i < 4; i++)
		for(size_t j = 0; j < 9; j++)
			res[i][j] = copy[i][j];
	return (res);
}

void copypBoard(e_Color **copy)
{
	for(size_t i = 0; i < 4; i++)
		for(size_t j = 0; j < 9; j++)
			copy[i][j] = pBoard[i][j];
}


void freeBoard(e_Color **tab)
{
	for(size_t i = 0; i < 4; i++)
		free(tab[i]);
	free(tab);
}

void saveBoardtoFile(void)
{
	std::ofstream outputfile(".save.txt", std::ios::out);
	int mode;

	if (pvpmode)
		mode = 0;
	else if (pvsaimode)
		mode = 1;
	else
		mode = 2; 
	if (outputfile)
	{
		insideBoardnum = -1;
		outputfile << mode << " " << numturn << " " << isAIturn << " " << hasChosenDirection << " " << hasPlayed << " " << winner << " ";
		outputfile << selectedBoard << std::endl;
		pvpmode = false;
		pvsaimode = false;
		aivspmode = false;
		switch(mode)
		{
			case 0:
				pvpmode = true;
				break;
			case 1:
				pvsaimode = true;
				break;
			case 2:
				aivspmode = true;
				break;
			default:
				break;
		}
		for (unsigned int j = 0; j < 7; j+=3)
		{
			for (unsigned int i = 0; i < 2; i++)
				outputfile << (unsigned int)pBoard[i][j] << " " << (unsigned int)pBoard[i][j + 1] << " " << (unsigned int)pBoard[i][j + 2] << " ";
			outputfile << std::endl;
		}
		for (unsigned int j = 0; j < 7; j+=3)
		{
			for (unsigned int i = 2; i < 4; i++)
				outputfile << (unsigned int)pBoard[i][j] << " " << (unsigned int)pBoard[i][j + 1] << " " << (unsigned int)pBoard[i][j + 2] << " ";
			outputfile << std::endl;
		}
		outputfile.close();
	}
	else
		std::cerr << "The file cannot be opened !" << std::endl;
}

void loadBoardFromFile(void)
{
	std::ifstream inputfile(".save.txt", std::ios::in);
	int mode;
	unsigned int tmp, tmp2, tmp3;

	if (inputfile)
	{
		insideBoardnum = -1;
		inputfile >> mode >> numturn >> isAIturn >> hasChosenDirection >> hasPlayed >> winner >> selectedBoard;
		pvpmode = false;
		pvsaimode = false;
		aivspmode = false;
		switch(mode)
		{
			case 0:
				pvpmode = true;
				break;
			case 1:
				pvsaimode = true;
				break;
			case 2:
				aivspmode = true;
				break;
			default:
				break;
		}
		for (unsigned int j = 0; j < 7; j+=3)
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				inputfile >> tmp >> tmp2 >> tmp3;
				pBoard[i][j] = (e_Color)tmp;
				pBoard[i][j + 1] = (e_Color)tmp2;
				pBoard[i][j + 2] = (e_Color)tmp3;
			}
		}
		for (unsigned int j = 0; j < 7; j+=3)
		{
			for (unsigned int i = 2; i < 4; i++)
			{
				inputfile >> tmp >> tmp2 >> tmp3;
				pBoard[i][j] = (e_Color)tmp;
				pBoard[i][j + 1] = (e_Color)tmp2;
				pBoard[i][j + 2] = (e_Color)tmp3;
			}
		}
		inputfile.close();
	}
	else
		std::cerr << "The file does not exist or cannot be opened !" << std::endl;
}

bool winDiag(e_Color **board, e_Color c)
{
	if (board[0][8] == c && board[1][1] == c && board[1][3] == c && board[2][1] == c && board[2][3] == c)
		return (true);
	if ((board[1][2] == c || board[2][6] == c) && board[1][4] == c && board[1][6] == c && board[2][2] == c && board[2][4] == c)
		return (true);
	if (board[1][5] == c && board[1][7] == c && board[2][5] == c && board[2][7] == c && board[3][0] == c)
		return (true);
	if (board[0][3] == c && board[0][7] == c && board[3][3] == c && board[3][7] == c && board[2][2] == c)
		return (true);
	if ((board[0][0] == c || board[3][8] == c) && board[0][4] == c && board[0][8] == c && board[3][0] == c && board[3][4] == c)
		return (true);
	if (board[0][1] == c && board[0][5] == c && board[1][6] == c && board[3][1] == c && board[3][5] == c)
		return (true);
	return (false);
}

bool winLines(e_Color **board, e_Color c)
{
	int k;
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if ((board[j][i] == c || board[2 + j][6 + i] == c) && board[j][3 + i] == c && board[j][6 + i] == c && board[2 + j][i] == c && board[2 + j][3 + i] == c)
				return (true);
		}
	}
	for (int j = 0; j < 3; j+=2)
	{
		for (int i = 0; i < 3; i++)
		{
			k = i * 3;
			if ((board[0 + j][0 + k] == c || board[1 + j][2 + k] == c) && board[0 + j][1 + k] == c && board[0 + j][2 + k] == c && board[1 + j][0 + k] == c && board[1 + j][1 + k] == c)
				return (true);
		}
	}
	return (false);
}

bool hasWon(e_Color **board, e_Color color)
{
	if (!winLines(board, color))
		return (winDiag(board, color));
	return (true);
}

void rotateLeft(e_Color **board, int number)
{
	e_Color tmp, tmp2;
	
	tmp = board[number][6];
	board[number][6] = board[number][0];
	tmp2 = board[number][8];
	board[number][8] = tmp;
	tmp = board[number][2];
	board[number][2] = tmp2;
	board[number][0] = tmp;
	
	tmp = board[number][1];
	board[number][1] = board[number][5];
	tmp2 = board[number][3];
	board[number][3] = tmp;
	tmp = board[number][7];
	board[number][7] = tmp2;
	board[number][5] = tmp;	

}

void rotateRight(e_Color **board, int number)
{
	e_Color tmp, tmp2;
	
	tmp = board[number][2];
	board[number][2] = board[number][0];
	tmp2 = board[number][8];
	board[number][8] = tmp;
	tmp = board[number][6];
	board[number][6] = tmp2;
	board[number][0] = tmp;
	
	tmp = board[number][1];
	board[number][1] = board[number][3];
	tmp2 = board[number][5];
	board[number][5] = tmp;
	tmp = board[number][7];
	board[number][7] = tmp2;
	board[number][3] = tmp;	

}

void rotateBoard(unsigned int rotnum)
{
	switch(rotnum)
	{
		case 0:
			rotateLeft(pBoard, 0);
			break;
		case 1:
			rotateRight(pBoard, 0);
			break;
		case 2:
			rotateLeft(pBoard, 1);
			break;
		case 3:
			rotateRight(pBoard, 1);
			break;
		case 4:
			rotateLeft(pBoard, 2);
			break;
		case 5:
			rotateRight(pBoard, 2);
			break;
		case 6:
			rotateLeft(pBoard, 3);
			break;
		case 7:
			rotateRight(pBoard, 3);
			break;
		default:
			break;
	}
}

e_Color ** initpBoard(void)
{
	e_Color **board;

	if ((board = (e_Color **)malloc(sizeof(e_Color *) * 4)) == NULL)
	{
		perror("malloc failed : ");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < 4; i++)
	{
		if ((board[i] = (e_Color *)malloc(sizeof(e_Color) * 9)) == NULL)
		{
			perror("malloc failed : ");
			exit(EXIT_FAILURE);
		}
	}
	for (size_t j = 0; j < 4; j++)
		for (size_t i = 0; i < 9; i++)
			board[j][i] = EMPTY;
	return (board);
}

void printBoard(e_Color **board)
{
	for (unsigned int k = 0; k < 7; k += 3)
	{
		for (unsigned int j = 0; j < 2; j++)
		{ 
			for (unsigned int i = 0; i < 3; i++)
				fprintf(stderr, "%d ", board[j][i + k]);
		}
		fprintf(stderr, "\n");
	}
	for (unsigned int k = 0; k < 7; k += 3)
	{
		for (unsigned int j = 2; j < 4; j++)
		{ 
			for (unsigned int i = 0; i < 3; i++)
				fprintf(stderr, "%d ", board[j][i + k]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

