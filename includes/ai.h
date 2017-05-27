#ifndef AI_H
# define AI_H
# include "game.h"

	bool canWin(e_Color **board, e_Color p, unsigned int *x, unsigned int *y, bool flagStop);
	void generatePlay(e_Color **board, e_Color player, unsigned int turn);
	void AIstart(void);
	int simulateGame(e_Color **pentagoBoard, e_Color player, e_Color realPlayer, unsigned int turn);
	int AIcountStrategies(e_Color **board, e_Color p);
	bool canAlignFour(e_Color **board, e_Color p);
	unsigned int tryToSurvive(e_Color **board);
	bool canWinNextTurn(e_Color **board, e_Color p);
	unsigned int AIpickdirection(e_Color **board);
	void monteCarlo(e_Color opponent, unsigned int *x, unsigned int *y);
	unsigned int rotatenum(unsigned int rot, unsigned int index);
	void findplay(e_Color **grid, unsigned int *x, unsigned int *y);

#endif
