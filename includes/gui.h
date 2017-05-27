#ifndef GUI_H
# define GUI_H
# include <string>

	typedef struct s_Color
	{
		double r;
		double g;
		double b;
	}t_Color;

	void glInitialize(void);
	void reinitialize(void);
	int isInsideBoard(double x, double y);
	void drawModeBorders(void);
	bool isInsidePVP(double x, double y);
	bool isInsidePVSAI(double x, double y);
	bool isInsideAIVSP(double x, double y);
	bool isInsideMainMenu(double x, double y);
	bool isInsideExit(double x, double y);
	void mouseMotion(int x, int y);
	int findTile(int boardnumber, double x, double y);
	void keyboard(unsigned char c, int x, int y);
	void specialInput(int key, int x, int y);
	bool clickedOnRightArrow(double x, double y);
	bool clickedOnUpArrow(double x, double y);
	bool clickedOnDownArrow(double x, double y);
	bool clickedOnLeftArrow(double x, double y);
	bool playAux(void);
	void mouseClicked(int button, int state, int x, int y);
	void findCenter(int numBoard, int numTile, double *cx, double *cy);
	void drawBorder(int selectedBoard);
	void writeText(t_Color text, double posX, double posY, double posZ, double sizeX, double sizeY, double sizeZ, std::string str);
	void drawArrow(int numBoard);
	void render(void);

#endif
