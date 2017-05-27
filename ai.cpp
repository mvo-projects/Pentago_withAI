#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "game.h"
#include "ai.h"

extern unsigned int numturn;
const int MAX_TURN = 37;
extern bool isAIturn;
extern bool pvsaimode;
extern e_Color **pBoard;

extern e_Color player;
extern e_Color AI_COLOR;

bool canWin(e_Color **board, e_Color p, unsigned int *x, unsigned int *y, bool flagStop)
{
	e_Color **pBoardbis;
	bool win;
	size_t j, k;
	unsigned int tmp, tmp2;

	win = false;
	pBoardbis = createpCopy(board);
	for (size_t i = 0; i < 4; i++)
	{
		if (win)
			break;
		for (j = 0; j < 9; j++)
		{
			if (win)
				break;
			if (pBoardbis[i][j] == EMPTY)
			{
				pBoardbis[i][j] = p;
				if (hasWon(pBoardbis, p))
				{
					win = true;
				//	#pragma omp critical
				//	{
					*x = i;
					*y = j;
				//	}
					break;
				}
				pBoardbis[i][j] = EMPTY;
			}
		}
	}
	if (win)
	{
		freeBoard(pBoardbis);
		return (win);
	}	
		for (size_t i = 0; i < 4; i++)
		{
			if (win)
				break;
			for (j = 0; j < 9; j++)
			{
				if (win)
					break;
				if (pBoardbis[i][j] == EMPTY)
				{
					pBoardbis[i][j] = p;
					for (k = 0; k < 4; k++)
					{
						rotateLeft(pBoardbis, k);
						if (hasWon(pBoardbis, p))
						{
							win = true;
							*x = i;
							*y = j;
							if (!flagStop)
								break;
							if(!canWin(pBoardbis, p, &tmp, &tmp2, false))
							{
								*x = tmp;
								*y = tmp2;
								break;
							}
						}
						else
						{
							rotateRight(pBoardbis, k);
							rotateRight(pBoardbis, k);
							if (hasWon(pBoardbis, p))
							{
								win = true;
								*x = i;
								*y = j;
								if (!flagStop)
								{
									rotateLeft(pBoardbis, k);
									break;
								}
								if (!canWin(pBoardbis, p, &tmp, &tmp2, false))
								{
									*x = tmp;
									*y = tmp2;
									rotateLeft(pBoardbis, k);
									break;
								}
							}
							rotateLeft(pBoardbis, k);
						}
					}
					pBoardbis[i][j] = EMPTY;
				}
			}
		}
	freeBoard(pBoardbis);
	return (win);
}

void generatePlay(e_Color **board, e_Color player, unsigned int turn)
{
	int rnd;
	int nb_empty;

	nb_empty = 0;
	rnd = ((std::rand() % (MAX_TURN - turn)) + 1);
	for (size_t num_board = 0; num_board < 2; num_board++)
	{
		for (size_t num_tile = 0; num_tile < 3; num_tile++)
		{
			if (board[num_board][num_tile] == EMPTY)
				nb_empty++;
			if (nb_empty == rnd)
			{
				board[num_board][num_tile] = player;
				return;
			}
		}
	}
	for (size_t num_board = 0; num_board < 2; num_board++)
	{
		for (size_t num_tile = 3; num_tile < 6; num_tile++)
		{
			if (board[num_board][num_tile] == EMPTY)
				nb_empty++;
			if (nb_empty == rnd)
			{
				board[num_board][num_tile] = player;
				return;
			}
		}
	}
	for (size_t num_board = 0; num_board < 2; num_board++)
	{
		for (size_t num_tile = 6; num_tile < 9; num_tile++)
		{
			if (board[num_board][num_tile] == EMPTY)
				nb_empty++;
			if (nb_empty == rnd)
			{
				board[num_board][num_tile] = player;
				return;
			}
		}
	}
	for (size_t num_board = 2; num_board < 4; num_board++)
	{
		for (size_t num_tile = 0; num_tile < 3; num_tile++)
		{
			if (board[num_board][num_tile] == EMPTY)
				nb_empty++;
			if (nb_empty == rnd)
			{
				board[num_board][num_tile] = player;
				return;
			}
		}
	}
	for (size_t num_board = 2; num_board < 4; num_board++)
	{
		for (size_t num_tile = 3; num_tile < 6; num_tile++)
		{
			if (board[num_board][num_tile] == EMPTY)
				nb_empty++;
			if (nb_empty == rnd)
			{
				board[num_board][num_tile] = player;
				return;
			}
		}
	}
	for (size_t num_board = 2; num_board < 4; num_board++)
	{
		for (size_t num_tile = 6; num_tile < 9; num_tile++)
		{
			if (board[num_board][num_tile] == EMPTY)
				nb_empty++;
			if (nb_empty == rnd)
			{
				board[num_board][num_tile] = player;
				return;
			}
		}
	}
}


int simulateGame(e_Color **pentagoBoard, e_Color player, e_Color realPlayer, unsigned int turn) 
{
	e_Color opponent;

	if (hasWon(pentagoBoard, realPlayer))
		return (10000);
	opponent = (e_Color)((int)(player + 1) % 2);
	if (hasWon(pentagoBoard, (e_Color)((int)(realPlayer + 1) % 2)))
		return (-10000);
	if (turn == 37)
		return (1);
	else
	{
		generatePlay(pentagoBoard, player, turn);
		return (simulateGame(pentagoBoard, opponent, realPlayer, turn + 1));
	}
}

int AIcountStrategies(e_Color **board, e_Color p)
{
	int result;

	result = 48;

	for (unsigned int i = 0; i < 3; i+=2)
	{	
		for (unsigned int k = 0; k < 3; k+=2)
		{	
			if (board[i][1] == p || board[i][2] == p || board[i + 1][0 + k] == p || board[i + 1][3 + k] == p || board[i + 1][6 + k] == p)
				--result;
			if (board[i][7] == p || board[i][8] == p || board[i + 1][0 + k] == p || board[i + 1][3 + k] == p || board[i + 1][6 + k] == p)
				--result;
			if (board[i + 1][0] == p || board[i + 1][1] == p || board[i + 1][0 + k] == p || board[i + 1][3 + k] == p || board[i + 1][6 + k] == p)
				--result;
			if (board[i + 1][6] == p || board[i + 1][7] == p || board[i + 1][0 + k] == p || board[i + 1][3 + k] == p || board[i + 1][6 + k] == p)
				--result;
		}
		if (board[i][4] == p || board[i][5] == p || board[i + 1][1] == p || board[i + 1][4] == p || board[i + 1][7] == p)
			--result;
		if (board[i + 1][3] == p || board[i + 1][4] == p || board[i][1] == p || board[i][4] == p || board[i][7] == p)
			--result;
	}
	
	for (unsigned int i = 0; i < 2; i++)
	{	
		for (unsigned int k = 0; k < 7; k+=6)
		{	
			if (board[i + 2][0] == p || board[i + 2][3] == p || board[i][0 + k] == p || board[i][1 + k] == p || board[i][2 + k] == p)
				--result;
			if (board[i + 2][2] == p || board[i + 2][5] == p || board[i][0 + k] == p || board[i][1 + k] == p || board[i][2 + k] == p)
				--result;
			if (board[i][3] == p || board[i][6] == p || board[i + 2][0 + k] == p || board[i + 2][1 + k] == p || board[i + 2][2 + k] == p)
				--result;
			if (board[i][5] == p || board[i][8] == p || board[i + 2][0 + k] == p || board[i + 2][1 + k] == p || board[i + 2][2 + k] == p)
				--result;
		}
		if (board[i + 2][1] == p || board[i + 2][4] == p || board[i][0] == p || board[i][4] == p || board[i][5] == p)
			--result;
		if (board[i][4] == p || board[i][7] == p || board[i + 2][3] == p || board[i + 2][4] == p || board[i + 2][5] == p)
			--result;
	}
	if (board[0][4] == p || board[0][8] == p || board[3][0] == p || board[3][4] == p)
		result = result - 2;
	else
	{
		if (board[0][0] == p)
			--result;
		if (board[3][8] == p)
			--result;
	}
	if (board[1][4] == p || board[1][6] == p || board[2][2] == p || board[2][4] == p)
		result = result - 2;
	else
	{
		if (board[1][2] == p)
			--result;
		if (board[2][6] == p)
			--result;
	}
	if (board[0][8] == p || board[1][1] == p || board[1][3] == p || board[2][1] == p || board[2][3] == p)
		--result;
	if (board[3][0] == p || board[1][5] == p || board[1][8] == p || board[2][5] == p || board[2][8] == p)
		--result;
	if (board[0][1] == p || board[0][5] == p || board[1][6] == p || board[3][1] == p || board[3][5] == p)
		--result;
	if (board[0][3] == p || board[0][7] == p || board[2][2] == p || board[3][3] == p || board[3][7] == p)
		--result;
	return (result);
}

bool canAlignFour(e_Color **board, e_Color p)
{
	int pawn;
	e_Color p2;

	p2 = (e_Color)((int)(p + 1) % 2);
	for (unsigned int j = 0; j < 3; j+=2)
	{
		for (unsigned int k = 0; k < 7; k+=3)
		{
			if (board[0 + j][0 + k] == EMPTY && board[1 + j][2 + k] == EMPTY)
			{
				pawn = 0;
				if (board[0 + j][1 + k] == p2)
					pawn--;
				else if (board[0 + j][1 + k] == p)
					pawn++;
				if (board[0 + j][2 + k] == p2)
					pawn--;
				else if (board[0 + j][2 + k] == p)
					pawn++;
				if (board[1 + j][0 + k] == p2)
					pawn--;
				else if (board[1 + j][0 + k] == p)
					pawn++;
				if (board[1 + j][1 + k] == p2)
					pawn--;
				else if (board[1 + j][1 + k] == p)
					pawn++;
				if (pawn >= 3)
					return (true);
			}
		}
	}
	for (unsigned int j = 0; j < 2; j++)
	{
		for (unsigned int k = 0; k < 3; k++)
		{
			if (board[0 + j][0 + k] == EMPTY && board[2 + j][6 + k] == EMPTY)
			{
				pawn = 0;
				if (board[0 + j][3 + k] == p2)
					pawn--;
				else if (board[0 + j][3 + k] == p)
					pawn++;
				if (board[0 + j][6 + k] == p2)
					pawn--;
				else if (board[0 + j][6 + k] == p)
					pawn++;
				if (board[2 + j][0 + k] == p2)
					pawn--;
				else if (board[2 + j][0 + k] == p)
					pawn++;
				if (board[2 + j][3 + k] == p2)
					pawn--;
				else if (board[2 + j][3 + k] == p)
					pawn++;
				if (pawn >= 3)
					return (true);
			}
		}
	}
	return (false);
}

unsigned int tryToSurvive(e_Color **board)
{
	unsigned int x, y;
	e_Color p;
	e_Color **pBoardbis;

	pBoardbis = createpCopy(board);
	p = (e_Color)((int)(AI_COLOR + 1) % 2);
	for (unsigned int i = 0; i < 4; i++)
	{
		rotateLeft(pBoardbis, i);
		if (!canWin(pBoardbis, p, &x, &y, false))
		{
			freeBoard(pBoardbis);
			return (i * 2);
		}
		else
		{
			rotateRight(pBoardbis, i);
			rotateRight(pBoardbis, i);
			if (!canWin(pBoardbis, p, &x, &y, false))
			{
				freeBoard(pBoardbis);
				return (i * 2 + 1);
			}
			rotateLeft(pBoardbis, i);
		}
	}
	freeBoard(pBoardbis);
	return (8);
}

bool canWinNextTurn(e_Color **board, e_Color p)
{
	bool res = false;
	unsigned int x, y;
	e_Color **pBoardbis;

	pBoardbis = createpCopy(board);
	for (unsigned int i = 0; i < 4; i++)
	{
		rotateLeft(pBoardbis, i);
		res |= canWin(pBoardbis, p, &x, &y, false);
		res |= canAlignFour(pBoardbis, p);
		if (res)
		{
			freeBoard(pBoardbis);
			return (res);
		}
		rotateRight(pBoardbis, i);
		rotateRight(pBoardbis, i);
		res |= canWin(pBoardbis, p, &x, &y, false);
		res |= canAlignFour(pBoardbis, p);
		if (res)
		{
			freeBoard(pBoardbis);
			return (res);
		}
		rotateLeft(pBoardbis, i);
	}
	return (res);
}

unsigned int AIpickdirection(e_Color **board)
{
	e_Color **pBoardbis;
	int scoreH = 0;
	int scoreH2 = 0;
	int scoreMax;
	int scoreStrategy = 0;
	unsigned int bestDirection = 8;
	unsigned int x, y;
	e_Color p;

	scoreMax = 100;
	pBoardbis = createpCopy(board);
	p = (e_Color)((int)(AI_COLOR + 1) % 2);
	for (unsigned int i = 0; i < 4; i++)
	{
		rotateLeft(pBoardbis, i);
		if (hasWon(pBoardbis, AI_COLOR))
		{
			freeBoard(pBoardbis);
			return (i * 2);
		}
		rotateRight(pBoardbis, i);
		rotateRight(pBoardbis, i);
		if (hasWon(pBoardbis, AI_COLOR))
		{
			freeBoard(pBoardbis);
			return (i * 2 + 1);
		}
		rotateLeft(pBoardbis, i);
	}
	for (unsigned int i = 0; i < 4; i++)
	{
		rotateLeft(pBoardbis, i);
		if (!canWin(pBoardbis, p, &x, &y, true))
		{
			if (canWin(pBoardbis, AI_COLOR, &x, &y, false))
			{
				freeBoard(pBoardbis);
				return (i * 2);
			}
			if (canAlignFour(pBoardbis, p) || canWinNextTurn(pBoardbis, p))
				scoreH = -1;
			else
				scoreH = AIcountStrategies(pBoardbis, p);
			if (scoreH != -1)
			{
				if (scoreH == scoreMax)
				{
					scoreH2 = AIcountStrategies(pBoardbis, AI_COLOR);
					if (scoreH2 > scoreStrategy)
					{
						scoreMax = scoreH;
						scoreStrategy = scoreH2;
						bestDirection = i * 2;
					}
				}
				else if (scoreH < scoreMax)
				{
					scoreMax = scoreH;
					scoreStrategy = AIcountStrategies(pBoardbis, AI_COLOR);
					bestDirection = i * 2;
				}
			}
		}
		rotateRight(pBoardbis, i);
		rotateRight(pBoardbis, i);
		if (!canWin(pBoardbis, p, &x, &y, true))
		{
			if (canWin(pBoardbis, AI_COLOR, &x, &y, false))
			{
				freeBoard(pBoardbis);
				return (i * 2 + 1);
			}
			if (canAlignFour(pBoardbis, p) || canWinNextTurn(pBoardbis, p))
				scoreH = -1;
			else
				scoreH = AIcountStrategies(pBoardbis, p);
			if (scoreH != -1)
			{
				if (scoreH == scoreMax)
				{
					scoreH2 = AIcountStrategies(pBoardbis, AI_COLOR);
					if (scoreH2 > scoreStrategy)
					{
						scoreMax = scoreH;
						scoreStrategy = scoreH2;
						bestDirection = i * 2 + 1;
					}
				}
				else if (scoreH < scoreMax)
				{
					scoreMax = scoreH;
					scoreStrategy = AIcountStrategies(pBoardbis, AI_COLOR);
					bestDirection = i * 2 + 1;
				}
			}
		}
		rotateLeft(pBoardbis, i);
	}
	freeBoard(pBoardbis);
	if (bestDirection == 8)
		bestDirection = tryToSurvive(board);
	return (bestDirection);
}

void monteCarlo(e_Color opponent, unsigned int *x, unsigned int *y)
{
	e_Color **pBoardbis;
	long bestscore;
	long score;
	int num_board, num_tile;

	bestscore = -200000000;
	pBoardbis = initpBoard();
	copypBoard(pBoardbis);
	for (num_board = 0; num_board < 4; num_board++)
	{
		for (num_tile = 0; num_tile < 9; num_tile++)
		{
			if (pBoardbis[num_board][num_tile] == EMPTY)
			{
				score = 0;
				for (unsigned index = 0; index < 6000; index++)
				{
					pBoardbis[num_board][num_tile] = opponent;
					score += simulateGame(pBoardbis, (e_Color)((int)(opponent + 1) % 2), opponent, numturn + 1);
					copypBoard(pBoardbis);
				}
				if (score > bestscore)
				{
					bestscore = score;
					*x = num_board;
					*y = num_tile;
				}
			}
		}
	}
	freeBoard(pBoardbis);
}


unsigned int rotatenum(unsigned int rot, unsigned int index)
{
	if ((rot % 2) == 0)
	{
		switch(index)
		{
			case 0:
				return 6;
			case 1:
				return 3;
			case 2:
				return 0;
			case 3:
				return 7;
			case 4:
				return 4;
			case 5:
				return 1;
			case 6:
				return 8;
			case 7:
				return 5;
			case 8:
				return 2;
			default:
				return (index);
		}
	}
	else
	{
		switch(index)
		{
			case 0:
				return 2;
			case 1:
				return 5;
			case 2:
				return 8;
			case 3:
				return 1;
			case 4:
				return 4;
			case 5:
				return 7;
			case 6:
				return 0;
			case 7:
				return 3;
			case 8:
				return 6;
			default:
				return (index);
		}
	}
	return (index);
}

void findplay(e_Color **grid, unsigned int *x, unsigned int *y)
{
	bool ok = false;
	e_Color **board;
	unsigned int i, j, k;

	board = createpCopy(grid);
	for (i = 0; i < 2; i++)
	{
		for (k = 0; k < 4; k+=3)
		{
			for (j = 4; j < 6; j++)
			{
				if (board[i][j + k - i] == EMPTY)
				{
					board[i][j + k - i] = player;
					if (!canAlignFour(board, AI_COLOR))
					{
						*x = i;
						*y = j + k - i;
						ok = true;
						break;
					} 	
					board[i][j + k - i] = EMPTY;
				}
				if (ok)
					break;
			}
			if (ok)
				break;
		}
		if (ok)
			break;
	}
	if (!ok)
	{
		for (i = 2; i < 4; i++)
		{
			for (k = 0; k < 4; k+=3)
			{
				for (j = 1; j < 3; j++)
				{
					if (board[i][j + k - (i - 2)] == EMPTY)
					{
						board[i][j + k - (i - 2)] = player;
						if (!canAlignFour(board, AI_COLOR))
						{
							*x = i;
							*y = j + k - (i - 2);
							ok = true;
							break;
						} 	
						board[i][j + k - (i - 2)] = EMPTY;
					}
					if (ok)
						break;
				}
				if (ok)
					break;
			}
			if (ok)
				break;
		}
	}
	if (!ok)
	{
		for (i = 0; i < 2; i++)
		{
			for (j = 1; j < 3; j++)
			{
				if (board[i][j + k - i] == EMPTY)
				{
					board[i][j + k - i] = player;
					if (!canAlignFour(board, AI_COLOR))
					{
						*x = i;
						*y = j + k - i;
						ok = true;
						break;
					} 	
					board[i][j + k - i] = EMPTY;
				}
				if (ok)
					break;
			}
			if (ok)
				break;
		}
	}
	if (!ok)
	{
		for (i = 2; i < 4; i++)
		{
			for (j = 7; j < 8; j++)
			{
				if (board[i][j + k - (i - 2)] == EMPTY)
				{
					board[i][j + k - (i - 2)] = player;
					if (!canAlignFour(board, AI_COLOR))
					{
						*x = i;
						*y = j + k - (i - 2);
						ok = true;
						break;
					}
					board[i][j + k - (i - 2)] = EMPTY;
				}
				if (ok)
					break;
			}
			if (ok)
				break;
		}
	}
	if (!ok)
	{
		if (board[0][3] == EMPTY)
		{
			board[0][3] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 0;
				*y = 3;
				ok = true;
			}
			board[0][3] = EMPTY;
		}
		if (!ok && board[0][6] == EMPTY)
		{
			board[0][6] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 0;
				*y = 6;
				ok = true;
			}
			board[0][6] = EMPTY;
		}
		if (!ok && board[2][0] == EMPTY)
		{
			board[2][0] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 2;
				*y = 0;
				ok = true;
			}
			board[2][0] = EMPTY;
		}
		if (!ok && board[2][3] == EMPTY)
		{
			board[2][3] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 2;
				*y = 3;
				ok = true;
			}
			board[2][3] = EMPTY;
		}
		if (!ok && board[1][5] == EMPTY)
		{
			board[1][5] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 1;
				*y = 5;
				ok = true;
			}
			board[1][5] = EMPTY;
		}
		if (!ok && board[1][8] == EMPTY)
		{
			board[1][8] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 1;
				*y = 8;
				ok = true;
			}
			board[1][8] = EMPTY;
		}
		if (!ok && board[3][2] == EMPTY)
		{
			board[3][2] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 3;
				*y = 2;
				ok = true;
			}
			board[3][2] = EMPTY;
		}
		if (!ok && board[3][5] == EMPTY)
		{
			board[3][5] = player;
			if (!canAlignFour(board, AI_COLOR))
			{
				*x = 3;
				*y = 5;
				ok = true;
			}
			board[3][5] = EMPTY;
		}
	}
	freeBoard(board);
}

void AIstart(void)
{
	unsigned int x, y, rotnum, rotnum2;
	unsigned int rndboard;
	bool ok = false;
	unsigned int x1, y1;
	unsigned int i, j;
	unsigned int maxt;

	if (pvsaimode)
		maxt = 5;
	else
		maxt = 4;
	if (numturn < maxt)
	{
		do
		{
			rndboard = (std::rand() % 4);
			for (i = 0; i < 9; i++)
			{
				if (pBoard[rndboard][i] != EMPTY)
					break;
				else if (i == 8)
					ok = true;
			}
		}while(!ok);
		pBoard[rndboard][4] = AI_COLOR;
	}
	else if (canWin(pBoard, AI_COLOR, &x, &y, false))
		pBoard[x][y] = AI_COLOR;
	else if (canWin(pBoard, player, &x, &y, true))
	{
		pBoard[x][y] = AI_COLOR;
		if (canWin(pBoard, player, &x1, &y1, true))
		{
			pBoard[x][y] = EMPTY;
			ok = false;
			if (canAlignFour(pBoard, player))
			{
				for (i = 0; i < 4; i++)
				{
					for (j = 0; j < 9; j++)
					{
						if (pBoard[i][j] == EMPTY)
						{
							pBoard[i][j] = AI_COLOR;
							if (!canAlignFour(pBoard, player))
							{
								ok = true;
								break;
							}
							pBoard[i][j] = EMPTY;
						}
					}
					if (ok)
						break;
				}
			}
			if (!ok)
			{
				monteCarlo(AI_COLOR, &x, &y);
				pBoard[x][y] = AI_COLOR;
			}
		}
	}
	else if (canAlignFour(pBoard, AI_COLOR))
	{
		findplay(pBoard, &x, &y);
		pBoard[x][y] = AI_COLOR;
	}
	else
	{
		ok = false;
		monteCarlo(AI_COLOR, &x, &y);
		pBoard[x][y] = AI_COLOR;
	}
	if (hasWon(pBoard, AI_COLOR))
		return ;
	rotnum = AIpickdirection(pBoard);
	rotnum2 = tryToSurvive(pBoard);
	rotateBoard(rotnum);
	if (hasWon(pBoard, AI_COLOR))
		return ;
	if (rotnum2 != 8 && rotnum != rotnum2 && (canWin(pBoard, player, &x, &y, false) || canAlignFour(pBoard, player)))
	{
		switch(rotnum)
		{
			case 0:
				rotateRight(pBoard, 0);
				break;
			case 1:
				rotateLeft(pBoard, 0);
				break;
			case 2:
				rotateRight(pBoard, 1);
				break;
			case 3:
				rotateLeft(pBoard, 1);
				break;
			case 4:
				rotateRight(pBoard, 2);
				break;
			case 5:
				rotateLeft(pBoard, 2);
				break;
			case 6:
				rotateRight(pBoard, 3);
				break;
			case 7:
				rotateLeft(pBoard, 3);
				break;
			default:
				break;
		}
		rotateBoard(rotnum2);
	}
	else if (rotnum == 8 && rotnum2 == 8)
		rotateBoard(rand() % 8);
	isAIturn = false;
}

