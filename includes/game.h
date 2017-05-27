#ifndef GAME_H
# define GAME_H

	enum e_Color { BLACK, WHITE, EMPTY };

	void freeBoard(e_Color **tab);
	void saveBoardtoFile(void);
	void loadBoardFromFile(void);
	bool winDiag(e_Color **board, e_Color c);
	bool winLines(e_Color **board, e_Color c);
	bool hasWon(e_Color **board, e_Color color);
	void rotateLeft(e_Color **board, int number);
	void rotateRight(e_Color **board, int number);
	void rotateBoard(unsigned int rotnum);
	e_Color ** initpBoard(void);
	e_Color ** createpCopy(e_Color **copy);
	void copypBoard(e_Color **copy);
	void printBoard(e_Color **board);

#endif
