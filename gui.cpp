#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glut.h>
#include <unistd.h>
#include <ctime>
#include "game.h"
#include "ai.h"
#include "gui.h"

#ifndef M_PI
	const double M_PI = (double)3.1415926535;
#endif

int WIDTH = 800;
int HEIGHT = 600;
int insideBoardnum = -1;
int numberTile = -1;
int selectedBoard = -1;
unsigned int numturn = 1;
bool menu = true;
bool menuInGame = false;
bool hasChosenDirection = true;
bool hasPlayed = false;
bool winner = false;
bool isAIturn = false;
bool aivspmodeMotion = false;
bool pvpmodeMotion = false;
bool pvsaimodeMotion = false;
bool pvpmode = false;
bool pvsaimode = false;
bool aivspmode = false;
e_Color **pBoard;

e_Color player = BLACK;
e_Color AI_COLOR = WHITE;

std::string title = "Pentago";
std::string player1 = "J1";
std::string vs = "vs";
std::string player2 = "J2";
std::string str_ai = "IA";


void glInitialize(void)
{
	glClearColor(230.0/255.0f, 228.0/255.0f, 219.0f/255.0, 0.0f);
}

void reinitialize(void)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 9; j++)
					pBoard[i][j] = EMPTY;
	insideBoardnum = -1;
	numberTile = -1;
	selectedBoard = -1;
	numturn = 1;
	menuInGame = false;
	hasChosenDirection = true;
	player = BLACK;
	AI_COLOR = WHITE;
	hasPlayed = false;
	winner = false;
	isAIturn = false;
}


int isInsideBoard(double x, double y)
{
	if (x >= -0.62 && x <= -0.02)
	{
		if (y >= -0.62 && y <= -0.02)
			return (2);
		else if (y >= 0.02 && y <= 0.62)
			return (0);
	}
	else if (x >= 0.02 && x <= 0.62)
	{
		if (y >= -0.62 && y <= -0.02)
			return (3);
		else if (y >= 0.02 && y <= 0.62)
			return (1);
	}
	return (-1);
}

void drawModeBorders(void)
{
	double x1, x2, y1, y2;
	if (pvpmodeMotion || pvsaimodeMotion || aivspmodeMotion)
	{
		if (pvpmodeMotion)
		{
			x1 = -0.23;
			x2 = 0.23;
			y1 = 0.18;
			y2 = 0.32;
		}
		if (pvsaimodeMotion)
		{
			x1 = -0.23;
			x2 = 0.23;
			y1 = -0.02;
			y2 = 0.12;
		}
		if (aivspmodeMotion)
		{
			x1 = -0.23;
			x2 = 0.23;
			y1 = -0.22;
			y2 = -0.08;
		}
		glColor3f(0.0f, 0.0f, 0.0f);
		for (double index = 0.000; index <= 0.006; index += 0.001)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(x1 - index, y1 - index);
				glVertex2f(x2 + index, y1 - index);
				glVertex2f(x2 + index, y2 + index);
				glVertex2f(x1 - index, y2 + index);
			glEnd();
		}
	}
	glutPostRedisplay();
}

bool isInsidePVP(double x, double y)
{
	if (x >= -0.23 && x <= 0.23)
		if (y >= 0.18 && y <= 0.32)
			return (true);
	return (false);
}

bool isInsidePVSAI(double x, double y)
{
	if (x >= -0.23 && x <= 0.23)
		if (y <= 0.12 && y >= -0.02)
			return (true);
	return (false);
}

bool isInsideAIVSP(double x, double y)
{
	if (x >= -0.23 && x <= 0.23)
		if (y >= -0.22 && y <= -0.08)
			return (true);
	return (false);
}

bool isInsideMainMenu(double x, double y)
{
	if (x >= -0.9 && x <= -0.295)
		if (y >= -0.9 && y <= -0.76)
			return (true);
	return (false);
}

bool isInsideExit(double x, double y)
{
	if (x >= 0.64 && x <= 0.9)
		if (y >= -0.9 && y <= -0.76)
			return (true);
	return (false);
}

void mouseMotion(int x, int y)
{
	double normalizedX = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
	double normalizedY = -((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1);

	if (menu)
	{
		pvpmodeMotion = isInsidePVP(normalizedX, normalizedY);
		pvsaimodeMotion = isInsidePVSAI(normalizedX, normalizedY);
		aivspmodeMotion = isInsideAIVSP(normalizedX, normalizedY);
		/*std::cerr << "pvpMode : " << pvpmodeMotion << std::endl;
		std::cerr << "pvsaiMode : " << pvsaimodeMotion << std::endl;
		std::cerr << "aivspMode : " << aivspmodeMotion << std::endl;
		if (menuInGame)
		{
			mainmenuMotion = isInsideMainMenu(normalizedX, normalizedY);
			exitMotion = isInsideExit(normalizedX, normalizedY);
		}*/
		if (pvpmodeMotion || pvsaimodeMotion || aivspmodeMotion)
			drawModeBorders();
	}
	else
		insideBoardnum = isInsideBoard(normalizedX, normalizedY);
	glutPostRedisplay();
}

int findTile(int boardnumber, double x, double y)
{
	int numTile = 0;

	switch (boardnumber)
	{
		case 0:
			if (y <= 0.62 && y >= 0.42)
				numTile = 0;
			else if (y <= 0.42 && y >= 0.22)
				numTile = 1;
			else if (y <= 0.22 && y >= 0.02)
				numTile = 2;
			if (x >= -0.62 && x <= -0.42)
				return (0 + (numTile * 3));
			if (x >= -0.42 && x <= -0.22)
				return (1 + (numTile * 3));
			if (x >= -0.22 && x <= -0.02)
				return (2 + (numTile * 3));
			break;
		case 1:
			if (y <= 0.62 && y >= 0.42)
				numTile = 0;
			else if (y <= 0.42 && y >= 0.22)
				numTile = 1;
			else if (y <= 0.22 && y >= 0.02)
				numTile = 2;
			if (x <= 0.62 && x >= 0.42)
				return (2 + (numTile * 3));
			if (x <= 0.42 && x >= 0.22)
				return (1 + (numTile * 3));
			if (x <= 0.22 && x >= 0.02)
				return (0 + (numTile * 3));
			break;
		case 2:
			if (y >= -0.62 && y <= -0.42)
				numTile = 2;
			else if (y >= -0.42 && y <= -0.22)
				numTile = 1;
			else if (y >= -0.22 && y <= -0.02)
				numTile = 0;
			if (x >= -0.62 && x <= -0.42)
				return (0 + (numTile * 3));
			if (x >= -0.42 && x <= -0.22)
				return (1 + (numTile * 3));
			if (x >= -0.22 && x <= -0.02)
				return (2 + (numTile * 3));
			break;
		case 3:
			if (y >= -0.62 && y <= -0.42)
				numTile = 2;
			else if (y >= -0.42 && y <= -0.22)
				numTile = 1;
			else if (y >= -0.22 && y <= -0.02)
				numTile = 0;
			if (x <= 0.62 && x >= 0.42)
				return (2 + (numTile * 3));
			if (x <= 0.42 && x >= 0.22)
				return (1 + (numTile * 3));
			if (x <= 0.22 && x >= 0.02)
				return (0 + (numTile * 3));
			break;
		default:
			break;
	}
	return (-1);
}

void keyboard(unsigned char c, int x, int y)
{
	if (!menu)
	{
		if (c == 27)
		{
			if (menuInGame)
				menuInGame = false;
			else
				menuInGame = true;
		}
	}
	else
	{
		if (c == 27)
		{
			freeBoard(pBoard);
			exit(EXIT_SUCCESS);
		}
	}
}

void specialInput(int key, int x, int y)
{
	if (!hasChosenDirection)
	{
		switch (key)
		{
			case GLUT_KEY_LEFT:
				rotateLeft(pBoard, selectedBoard);
				hasChosenDirection = true;
				hasPlayed = false;
				glutPostRedisplay();
				break;
			case GLUT_KEY_RIGHT:
				rotateRight(pBoard, selectedBoard);
				hasChosenDirection = true;
				hasPlayed = false;
				glutPostRedisplay();
				break;
			default:
				return;
		}
	}
}

bool clickedOnRightArrow(double x, double y)
{
	double xgap, ygap;
	double p1x, p1y, p2x, p2y, p3x, p3y;
	double alpha, beta, gamma;

	xgap = ygap = 0.0f;
	if (selectedBoard == 1 || selectedBoard == 3)
		xgap = 0.64;
	if (selectedBoard == 2 || selectedBoard == 3)
		ygap = -1.37;

	// check inside rectangle of arrow
	if ((x >= -0.62 + xgap) && (x <= -0.58 + xgap) && (y >= 0.67 + ygap) && (y <= 0.7 + ygap))
		return (true);
	
	// check inside triangle (using barycenter theorem)
	p1x = -0.58f + xgap;
	p1y = 0.655f + ygap;
	p2x = -0.56f + xgap;
	p2y = 0.685f + ygap;
	p3x = -0.58f + xgap;
	p3y = 0.715f + ygap;

	alpha = ((p2y - p3y) * (x - p3x) + (p3x - p2x) * (y - p3y))/((p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y));
	beta = ((p3y - p1y)*(x-p3x)+(p1x-p3x)*(y-p3y))/((p2y-p3y)*(p1x-p3x)+(p3x-p2x)*(p1y - p3y));
	gamma = 1.0f - alpha - beta;

	if (alpha > 0 && beta > 0 && gamma > 0)
		return (true);
	return (false);
}

bool clickedOnUpArrow(double x, double y)
{
	double xgap;
	double p1x, p1y, p2x, p2y, p3x, p3y;
	double alpha, beta, gamma;

	xgap = 0.0f;
	if (selectedBoard == 3)
		xgap = 1.33;

	// check inside rectangle of arrow
	if ((x >= -0.68 + xgap) && (x <= -0.65 + xgap) && (y <= -0.58) && (y >= -0.62))
		return (true);
	// check inside triangle (using barycenter theorem)
	p1x = -0.695f + xgap;
	p1y = -0.58f;
	p2x = -0.665f + xgap;
	p2y = -0.55f;
	p3x = -0.635f + xgap;
	p3y = -0.58f;
	
	alpha = ((p2y - p3y) * (x - p3x) + (p3x - p2x) * (y - p3y))/((p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y));
	beta = ((p3y - p1y)*(x-p3x)+(p1x-p3x)*(y-p3y))/((p2y-p3y)*(p1x-p3x)+(p3x-p2x)*(p1y - p3y));
	gamma = 1.0f - alpha - beta;

	if (alpha > 0 && beta > 0 && gamma > 0)
		return (true);
	return (false);
}

bool clickedOnDownArrow(double x, double y)
{
	double xgap;
	double p1x, p1y, p2x, p2y, p3x, p3y;
	double alpha, beta, gamma;

	xgap = 0.0f;
	if (selectedBoard == 1)
		xgap = 1.33;

	// check inside rectangle of arrow
	if ((x >= -0.68 + xgap) && (x <= -0.65 + xgap) && (y >= 0.58) && (y <= 0.62))
		return (true);
	// check inside triangle (using barycenter theorem)
	p1x = -0.695f + xgap;
	p1y = 0.58f;
	p2x = -0.665f + xgap;
	p2y = 0.55f;
	p3x = -0.635f + xgap;
	p3y = 0.58f;
	
	alpha = ((p2y - p3y) * (x - p3x) + (p3x - p2x) * (y - p3y))/((p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y));
	beta = ((p3y - p1y)*(x-p3x)+(p1x-p3x)*(y-p3y))/((p2y-p3y)*(p1x-p3x)+(p3x-p2x)*(p1y - p3y));
	gamma = 1.0f - alpha - beta;

	if (alpha > 0 && beta > 0 && gamma > 0)
		return (true);
	return (false);

}

bool clickedOnLeftArrow(double x, double y)
{
	double xgap, ygap;
	double p1x, p1y, p2x, p2y, p3x, p3y;
	double alpha, beta, gamma;

	xgap = ygap = 0.0f;
	if (selectedBoard == 1 || selectedBoard == 3)
		xgap = 0.64;
	if (selectedBoard == 2 || selectedBoard == 3)
		ygap = -1.37;

	// check inside rectangle of arrow
	if ((x >= -0.06 + xgap) && (x <= -0.02 + xgap) && (y >= 0.67 + ygap) && (y <= 0.7 + ygap))
		return (true);
	// check inside triangle (using barycenter theorem)
	p1x = -0.06f + xgap;
	p1y = 0.655f + ygap;
	p2x = -0.08f + xgap;
	p2y = 0.685f + ygap;
	p3x = -0.06f + xgap;
	p3y = 0.715f + ygap;
	
	alpha = ((p2y - p3y) * (x - p3x) + (p3x - p2x) * (y - p3y))/((p2y - p3y)*(p1x - p3x) + (p3x - p2x)*(p1y - p3y));
	beta = ((p3y - p1y)*(x-p3x)+(p1x-p3x)*(y-p3y))/((p2y-p3y)*(p1x-p3x)+(p3x-p2x)*(p1y - p3y));
	gamma = 1.0f - alpha - beta;

	if (alpha > 0 && beta > 0 && gamma > 0)
		return (true);
	return (false);

}

bool playAux(void)
{
	hasPlayed = false;
	hasChosenDirection = true;
	if (!pvpmode)
		isAIturn = true;
	selectedBoard = -1;
	numturn++;
	glutPostRedisplay();
	if (hasWon(pBoard, player))
		return (true);
	if (hasWon(pBoard, AI_COLOR))
		return (true);
/*	if (!pvpmode)
	{
		if (isAIturn)
		{
			AIstart();
			numturn++;
			isAIturn = false;
		}
	}*/
	return (false);
}

void mouseClicked(int button, int state, int x, int y)
{
	double normalizedX = (double)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
	double normalizedY = -((double)y / glutGet(GLUT_WINDOW_HEIGHT) * 2 - 1);
	if (menuInGame)
	{
		if (isInsideExit(normalizedX, normalizedY) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			freeBoard(pBoard);
			exit(EXIT_SUCCESS);
		}
		if (isInsideMainMenu(normalizedX, normalizedY) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			reinitialize();
			menu = true;
			pvpmode = false;
			pvsaimode = false;
			aivspmode = false;
		}
		if (isInsidePVP(normalizedX, normalizedY) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			loadBoardFromFile();
			menuInGame = false;
		}
		if (isInsidePVSAI(normalizedX, normalizedY) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			saveBoardtoFile();
			menuInGame = false;
		}
		if (isInsideAIVSP(normalizedX, normalizedY) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			reinitialize();
			if (aivspmode)
			{
				AI_COLOR = BLACK;
				player = WHITE;
				isAIturn = true;
			}
		}
		
	}
	else if (menu)
	{
		if (pvpmodeMotion && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			pvpmode = true;
			menu = false;
		}
		if (pvsaimodeMotion && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			pvsaimode = true;
			menu = false;
		}
		if (aivspmodeMotion && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			aivspmode = true;
			menu = false;
			AI_COLOR = (e_Color)((int)(AI_COLOR + 1) % 2);
			player = (e_Color)((int)(player + 1) % 2);
			isAIturn = true;
			//AIstart();
		}
		glutPostRedisplay();
		
	}
	else
	{
		insideBoardnum = isInsideBoard(normalizedX, normalizedY);
		glutPostRedisplay();
		if (!winner && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			if (selectedBoard != -1)
			{
				if (selectedBoard == 0 && clickedOnRightArrow(normalizedX, normalizedY))
				{
					rotateRight(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 0 && clickedOnDownArrow(normalizedX, normalizedY))
				{
					rotateLeft(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 1 && clickedOnDownArrow(normalizedX, normalizedY))
				{
					rotateRight(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 1 && clickedOnLeftArrow(normalizedX, normalizedY))
				{
					rotateLeft(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 2 && clickedOnUpArrow(normalizedX, normalizedY))
				{
					rotateRight(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 2 && clickedOnRightArrow(normalizedX, normalizedY))
				{
					rotateLeft(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 3 && clickedOnUpArrow(normalizedX, normalizedY))
				{
					rotateLeft(pBoard, selectedBoard);
					if (playAux())
						return;
				}
				else if (selectedBoard == 3 && clickedOnLeftArrow(normalizedX, normalizedY))
				{
					rotateRight(pBoard, selectedBoard);
					if (playAux())
						return;
				}
			}
			selectedBoard = insideBoardnum;
			if (insideBoardnum != -1)
			{
				if (hasPlayed)
					hasChosenDirection = false;
				else
				{
					if (hasChosenDirection)
					{
						numberTile = findTile(insideBoardnum, normalizedX, normalizedY);
						if (pBoard[insideBoardnum][numberTile] != EMPTY)
							return;
						pBoard[insideBoardnum][numberTile] = player;
						if (pvpmode)
							player = (e_Color)(((int)player + 1) % 2);
						hasPlayed = true;
						glutPostRedisplay();
					}
				}
			}
		
		}
	}
}
	
void findCenter(int numBoard, int numTile, double *cx, double *cy)
{
	if (numBoard == 0 || numBoard == 2)
	{
		switch (numTile)
		{
			case 0:
				*cx = -0.52;
				break;
			case 1:
				*cx = -0.32;
				break;
			case 2:
				*cx = -0.12;
				break;
			case 3:
				*cx = -0.52;
				break;
			case 4:
				*cx = -0.32;
				break;
			case 5:
				*cx = -0.12;
				break;
			case 6:
				*cx = -0.52;
				break;
			case 7:
				*cx = -0.32;
				break;
			case 8:
				*cx = -0.12;
				break;
			default:
				break;
		}
	}
	if (numBoard == 1 || numBoard == 3)
	{
		switch (numTile)
		{
			case 0:
				*cx = 0.12;
				break;
			case 1:
				*cx = 0.32;
				break;
			case 2:
				*cx = 0.52;
				break;
			case 3:
				*cx = 0.12;
				break;
			case 4:
				*cx = 0.32;
				break;
			case 5:
				*cx = 0.52;
				break;
			case 6:
				*cx = 0.12;
				break;
			case 7:
				*cx = 0.32;
				break;
			case 8:
				*cx = 0.52;
				break;
			default:
				break;
		}
	}
	if (numBoard == 0 || numBoard == 1)
	{
		switch (numTile)
		{
			case 0:
				*cy = 0.52;
				break;
			case 1:
				*cy = 0.52;
				break;
			case 2:
				*cy = 0.52;
				break;
			case 3:
				*cy = 0.32;
				break;
			case 4:
				*cy = 0.32;
				break;
			case 5:
				*cy = 0.32;
				break;
			case 6:
				*cy = 0.12;
				break;
			case 7:
				*cy = 0.12;
				break;
			case 8:
				*cy = 0.12;
				break;
			default:
				break;
		}
	}
	if (numBoard == 2 || numBoard == 3)
	{
		switch (numTile)
		{
			case 0:
				*cy = -0.12;
				break;
			case 1:
				*cy = -0.12;
				break;
			case 2:
				*cy = -0.12;
				break;
			case 3:
				*cy = -0.32;
				break;
			case 4:
				*cy = -0.32;
				break;
			case 5:
				*cy = -0.32;
				break;
			case 6:
				*cy = -0.52;
				break;
			case 7:
				*cy = -0.52;
				break;
			case 8:
				*cy = -0.52;
				break;
			default:
				break;
		}
	}
}

void drawBorder(int selectedBoard)
{
	float x1, x2;
	float y1, y2;

	if (selectedBoard == 0 || selectedBoard == 1)
	{
		y1 = 0.62;
		y2 = 0.02;
	}
	else
	{
		y1 = -0.02;
		y2 = -0.62;
	}
	if (selectedBoard == 0 || selectedBoard == 2)
	{
		x1 = -0.62;
		x2 = -0.02;
	}
	else
	{
		x1 = 0.02;
		x2 = 0.62;
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	for (double index = 0.000; index <= 0.006; index += 0.001)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2f(x1 + index, y1 + index);
			glVertex2f(x2 - index, y1 + index);
			glVertex2f(x2 - index, y2 - index);
			glVertex2f(x1 + index, y2 - index);
		glEnd();
	}
}

void writeText(t_Color text, double posX, double posY, double posZ, double sizeX, double sizeY, double sizeZ, std::string str)
{
	glPushMatrix();
	glColor3d(text.r, text.g, text.b);
	glTranslatef(posX, posY, posZ);
	glScalef(sizeX, sizeY, sizeZ);
	for (size_t i = 0; i < str.length(); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	glPopMatrix();
}

void drawArrow(int numBoard)
{
	double xgap, ygap;
	double xgap2;

	glColor3f(0.0f, 0.0f, 0.0f);
	xgap = ygap = 0.0f;
	xgap2 = 0.0f;
	if (numBoard != -1)
	{
		if (numBoard == 1 || numBoard == 3)
		{
			xgap = 0.64;
			xgap2 = 1.33;
		}
		if (numBoard == 2 || numBoard == 3)
			ygap = -1.37;
		if (numBoard == 0 || numBoard == 2)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(-0.62 + xgap, 0.67 + ygap);
				glVertex2f(-0.62 + xgap, 0.70 + ygap);
				glVertex2f(-0.58 + xgap, 0.70 + ygap);
				glVertex2f(-0.58 + xgap, 0.715 + ygap);
				glVertex2f(-0.56 + xgap, 0.685 + ygap);
				glVertex2f(-0.58 + xgap, 0.655 + ygap);
				glVertex2f(-0.58 + xgap, 0.67 + ygap);
			glEnd();
		}
		if (numBoard == 1 || numBoard == 3)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(-0.02 + xgap, 0.67 + ygap);
				glVertex2f(-0.06 + xgap, 0.67 + ygap);
				glVertex2f(-0.06 + xgap, 0.655 + ygap);
				glVertex2f(-0.08 + xgap, 0.685 + ygap);
				glVertex2f(-0.06 + xgap, 0.715 + ygap);
				glVertex2f(-0.06 + xgap, 0.7 + ygap);
				glVertex2f(-0.02 + xgap, 0.7 + ygap);
			glEnd();
		}
		if (numBoard == 0 || numBoard == 1)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(-0.65f + xgap2, 0.62f);
				glVertex2f(-0.68f + xgap2, 0.62f);
				glVertex2f(-0.68f + xgap2, 0.58f);
				glVertex2f(-0.695f + xgap2, 0.58f);
				glVertex2f(-0.665f + xgap2, 0.55f);
				glVertex2f(-0.635f + xgap2, 0.58f);
				glVertex2f(-0.65f + xgap2, 0.58f);
			glEnd();
		}
		if (numBoard == 2 || numBoard == 3)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(-0.65f + xgap2, -0.62f);
				glVertex2f(-0.68f + xgap2, -0.62f);
				glVertex2f(-0.68f + xgap2, -0.58f);
				glVertex2f(-0.695f + xgap2, -0.58f);
				glVertex2f(-0.665f + xgap2, -0.55f);
				glVertex2f(-0.635f + xgap2, -0.58f);
				glVertex2f(-0.65f + xgap2, -0.58f);
			glEnd();
		}
	}
}


void render(void)
{
	t_Color pickDirection;
	t_Color loadsavetext;
	double centerY;
	double centerX;

	pickDirection.r = pickDirection.g = pickDirection.b = 0.0f;
	loadsavetext.r = loadsavetext.g = loadsavetext.b = 1.0f;
	glInitialize();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (menuInGame)
	{
		glColor3f(163.0/255.0f, 200.0/255.0f, 226.0/255.0f);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, 1.0f);
			glVertex2f(-1.0, 0.6f);
			glVertex2f(1.0, 0.6f);
			glVertex2f(1.0, 1.0f);
		glEnd();
		glColor3f(0.6f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-0.23, 0.18f);
			glVertex2f(0.23, 0.18f);
			glVertex2f(0.23, 0.32f);
			glVertex2f(-0.23, 0.32f);
			
			glVertex2f(-0.23, -0.02f);
			glVertex2f(0.23, -0.02f);
			glVertex2f(0.23, 0.12f);
			glVertex2f(-0.23, 0.12f);

			glVertex2f(-0.23, -0.22f);
			glVertex2f(0.23, -0.22f);
			glVertex2f(0.23, -0.08f);
			glVertex2f(-0.23, -0.08f);
			
			glVertex2f(-0.9, -0.9f);
			glVertex2f(-0.295, -0.9f);
			glVertex2f(-0.295, -0.76f);
			glVertex2f(-0.9, -0.76f);	
			
			glVertex2f(0.9, -0.9f);
			glVertex2f(0.64, -0.9f);
			glVertex2f(0.64, -0.76f);
			glVertex2f(0.9, -0.76f);	
		glEnd();
		for (double index = -0.85; index <= -0.84; index += 0.001)
			writeText(pickDirection, index, 0.75, 0.0, 0.0015, 0.0015, 0.0, title);
		for (double index = -0.17; index <= -0.16; index += 0.001)
			writeText(pickDirection, index, 0.2, 0.0, 0.0011, 0.0010, 0.0, "Load");
		for (double index = -0.17; index <= -0.16; index += 0.001)
			writeText(pickDirection, index, 0.0, 0.0, 0.0011, 0.0010, 0.0, "Save");
		for (double index = -0.17; index <= -0.16; index += 0.001)
			writeText(pickDirection, index, -0.20, 0.0, 0.0010, 0.0010, 0.0, "Reset");
		for (double index = -0.88; index <= -0.87; index += 0.001)
			writeText(pickDirection, index, -0.88, 0.0, 0.0009, 0.0010, 0.0, "Main");
		for (double index = -0.61; index <= -0.6; index += 0.001)
			writeText(pickDirection, index, -0.88, 0.0, 0.0009, 0.0010, 0.0, "Menu");
		for (double index = 0.66; index <= 0.67; index += 0.001)
			writeText(pickDirection, index, -0.88, 0.0, 0.0009, 0.0010, 0.0, "Exit");
	}
	else if (menu)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-1.0f, 1.0f);
			glVertex2f(-1.0f, -1.0f);
			glVertex2f(1.0f, -1.0f);
			glVertex2f(1.0f, 1.0f);
		glEnd();
		glColor3f(163.0/255.0f, 200.0/255.0f, 226.0/255.0f);
		glBegin(GL_QUADS);
			glVertex2f(-0.6, 1.0f);
			glVertex2f(-0.6, -1.0f);
			glVertex2f(0.6, -1.0f);
			glVertex2f(0.6, 1.0f);
		glEnd();
	//	for (double index = -0.55; index <= -0.54; index += 0.001)
		for (double index = -0.35; index <= -0.34; index += 0.001)
			writeText(pickDirection, index, 0.75, 0.0, 0.0015, 0.0015, 0.0, title);
		glColor3f(0.6f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-0.23, 0.18f);
			glVertex2f(0.23, 0.18f);
			glVertex2f(0.23, 0.32f);
			glVertex2f(-0.23, 0.32f);
			
			glVertex2f(-0.23, -0.02f);
			glVertex2f(0.23, -0.02f);
			glVertex2f(0.23, 0.12f);
			glVertex2f(-0.23, 0.12f);

			glVertex2f(-0.23, -0.22f);
			glVertex2f(0.23, -0.22f);
			glVertex2f(0.23, -0.08f);
			glVertex2f(-0.23, -0.08f);
		glEnd();
		for (double index = -0.21; index <= -0.2; index += 0.001)
			writeText(pickDirection, index, 0.2, 0.0, 0.0010, 0.0010, 0.0, player1);
		for (double index = -0.08; index <= -0.07; index += 0.001)
			writeText(pickDirection, index, 0.2, 0.0, 0.0010, 0.0010, 0.0, vs);
		for (double index = 0.07; index <= 0.08; index += 0.001)
			writeText(pickDirection, index, 0.2, 0.0, 0.0010, 0.0010, 0.0, player2);
		
		for (double index = -0.21; index <= -0.2; index += 0.001)
			writeText(pickDirection, index, 0.0, 0.0, 0.0010, 0.0010, 0.0, player1);
		for (double index = -0.08; index <= -0.07; index += 0.001)
			writeText(pickDirection, index, 0.0, 0.0, 0.0010, 0.0010, 0.0, vs);
		for (double index = 0.07; index <= 0.08; index += 0.001)
			writeText(pickDirection, index, 0.0, 0.0, 0.0010, 0.0010, 0.0, str_ai);
		
		for (double index = -0.21; index <= -0.2; index += 0.001)
			writeText(pickDirection, index, -0.20, 0.0, 0.0010, 0.0010, 0.0, str_ai);
		for (double index = -0.08; index <= -0.07; index += 0.001)
			writeText(pickDirection, index, -0.20, 0.0, 0.0010, 0.0010, 0.0, vs);
		for (double index = 0.07; index <= 0.08; index += 0.001)
			writeText(pickDirection, index, -0.20, 0.0, 0.0010, 0.0010, 0.0, player1);
		drawModeBorders();
	}
	else
	{
	for (double index = -0.85; index <= -0.84; index += 0.001)
		writeText(pickDirection, index, 0.75, 0.0, 0.0015, 0.0015, 0.0, title);
	glColor3f(0.6f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f(0.62, -0.62f);
		glVertex2f(0.62, -0.02f);
		glVertex2f(0.02, -0.02f);
		glVertex2f(0.02, -0.62f);
		
		glVertex2f(-0.62, -0.62f);
		glVertex2f(-0.62, -0.02f);
		glVertex2f(-0.02, -0.02f);
		glVertex2f(-0.02, -0.62f);

		glVertex2f(-0.62, 0.62f);
		glVertex2f(-0.02, 0.62f);
		glVertex2f(-0.02, 0.02f);
		glVertex2f(-0.62, 0.02f);

		glVertex2f(0.62, 0.62f);
		glVertex2f(0.02, 0.62f);
		glVertex2f(0.02, 0.02f);
		glVertex2f(0.62, 0.02f);
	glEnd();
	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(-0.62 + (i * 0.2f), -0.62f);
			glVertex2f(-0.62 + (i * 0.2f), -0.02f);
			glVertex2f(-0.62 + (i * 0.2f), 0.62f);
			glVertex2f(-0.62 + (i * 0.2f), 0.02f);
			
			glVertex2f(0.02 + (i * 0.2f), -0.62f);
			glVertex2f(0.02 + (i * 0.2f), -0.02f);	
			glVertex2f(0.02 + (i * 0.2f), 0.02f);
			glVertex2f(0.02 + (i * 0.2f), 0.62f);
			glVertex2f(-0.62, 0.02 + (i * 0.2f));
			glVertex2f(-0.02, 0.02 + (i * 0.2f));
			glVertex2f(0.02, 0.02 + (i * 0.2f));
			glVertex2f(0.62, 0.02 + (i * 0.2f));
			glVertex2f(-0.62f, -0.02 - (i * 0.2f));
			glVertex2f(-0.02f, -0.02 - (i * 0.2f));
			glVertex2f(0.02f, -0.02 - (i * 0.2f));
			glVertex2f(0.62f, -0.02 - (i * 0.2f));
		glEnd();
	}
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (pBoard[j][i] != EMPTY)
			{
				findCenter(j, i, &centerX, &centerY);
				if (pBoard[j][i] == BLACK)
					glColor3f(0.0f, 0.0f, 0.0f);
				else if (pBoard[j][i] == WHITE)
					glColor3f(1.0f, 1.0f, 1.0f);
				glBegin(GL_TRIANGLE_FAN);
					glVertex2f(centerX, centerY);
					for (unsigned int i = 0; i <= 100; i++)
					{
						glVertex2f(
						(centerX + (0.075 * cos(i * 2 * M_PI / 100))), (centerY + (0.075 * sin(i * 2 * M_PI / 100)))
						);
					}
				glEnd();
			}
		}
	}
	if (!hasChosenDirection && selectedBoard != -1)
	{
		drawBorder(selectedBoard);
		drawArrow(selectedBoard);
	}
	if ((winner = hasWon(pBoard, BLACK)))
	{
		if (hasWon(pBoard, WHITE))
		{
			for (double index = -0.60; index <= -0.59; index += 0.001)
				writeText(pickDirection, index, -0.9, 0.0, 0.0015, 0.0015, 0.0, "IT'S A DRAW");
		}
		else
			for (double index = -0.60; index <= -0.59; index += 0.001)
				writeText(pickDirection, index, -0.9, 0.0, 0.0015, 0.0015, 0.0, "BLACK WON");
	}
	else if ((winner = hasWon(pBoard, WHITE)))
	{
		if (hasWon(pBoard, BLACK))
		{
			for (double index = -0.60; index <= -0.59; index += 0.001)
				writeText(pickDirection, index, -0.9, 0.0, 0.0015, 0.0015, 0.0, "IT'S A DRAW");
		}
		else
			for (double index = -0.60; index <= -0.59; index += 0.001)
				writeText(pickDirection, index, -0.9, 0.0, 0.0015, 0.0015, 0.0, "WHITE WON");
	}
	else if (hasPlayed && !winner)
		for (double index = -0.8; index <= -0.793; index += 0.001)
			writeText(pickDirection, index, -0.9, 0.0, 0.0010, 0.0010, 0.0, "Select a board and turn");
	else if (numturn == 37 && !winner)
		for (double index = -0.60; index <= -0.59; index += 0.001)
			writeText(pickDirection, index, -0.9, 0.0, 0.0015, 0.0015, 0.0, "IT'S A DRAW");
	else if (isAIturn)
	{
		for (double index = -0.60; index <= -0.59; index += 0.001)
			writeText(pickDirection, index, -0.9, 0.0, 0.0015, 0.0015, 0.0, "Please wait");
	}
}
	glutSwapBuffers();			
}

void ifunc(void)
{
	glutPostRedisplay();
	if (!pvpmode && !winner && numturn != 37)
	{
		if (isAIturn)
		{
			AIstart();
			numturn++;
		}
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	std::srand(std::time(0));
	pBoard = initpBoard();
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Pentago");

	glutSpecialFunc(specialInput);
	//glutTimerFunc(600, updateTimer, 600);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMotion);
	glutMouseFunc(mouseClicked);
	//glutReshapeFunc(resize);
	glutDisplayFunc(render);
	glutIdleFunc(ifunc);
	glutMainLoop();
	return (1);	
}
