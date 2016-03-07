#ifndef INPUT_H
#define INPUT_H

#undef APIENTRY

#include <GLFW\glfw3.h>

/*
GLFW is written for a US keyboard. This means that most special character keys are different. To amend this, special buttons with such exceptions will get a translation name here.
I may choose to extend this to apply to all special characters, including those that correspond. We'll see what happens.
*/
#define SWE_KEY_PLUS GLFW_KEY_MINUS
#define SWE_KEY_APOSTROPHE GLFW_KEY_BACKSLASH
#define SWE_KEY_DASH GLFW_KEY_SLASH

#include <string>
#include <algorithm>

using namespace std;

class Input
{
private:
	static Input* singleton;
	double scrollLastFrame = 0.0;
	Input();

	static bool Q,W,E,R,T,Y,U,I,O,P,A,S,D,F,G,H,J,K,L,Z,X,C,V,B,N,M,
		SHIFT,CTRL,ALT,SPACE,ESC,ENTER,
		NUM1,NUM2,NUM3,NUM4,NUM5,NUM6,NUM7,NUM8,NUM9,NUM0,
		DOT,COMMA, BACKSPACE, PLUS, APOSTROPHE, DASH, TAB,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12; //Button is pressed right now
	static bool pQ, pW, pE, pR, pT, pY, pU, pI, pO, pP, pA, pS, pD, pF, pG, pH, pJ, pK, pL, pZ, pX, pC, pV, pB, pN, pM,
		pSHIFT, pCTRL, pALT, pSPACE, pESC, pENTER,
		pNUM1, pNUM2, pNUM3, pNUM4, pNUM5, pNUM6, pNUM7, pNUM8, pNUM9, pNUM0,
		pDOT, pCOMMA, pBACKSPACE, pPLUS, pAPOSTROPHE, pDASH, pTAB,
		pF1, pF2, pF3, pF4, pF5, pF6, pF7, pF8, pF9, pF10, pF11, pF12; //Button was pressed this frame

	static bool keyPressedThisFrame; //Checks if at least one key was pressed this frame, if false then no clear is done and justPressed returns false automatically
	static void callbackKeyboard(GLFWwindow* wnd, int key, int scancode, int action, int modkeys);

	static bool LMB, RMB, MMB; //Mouse button is pressed right now
	static bool pLMB, pRMB, pMMB; //Mouse button was pressed this frame
	static double cursorX, cursorY; //Current cursor position
	static void callbackMouseButton(GLFWwindow* wnd, int button, int action, int mods);
	static void callbackCursorPos(GLFWwindow* wnd, double x, double y);

	static double scrollVal; //Scroll up = Positive, scroll down = Negative. Value = Speed of scrolling (useless?)
	static void callbackScroll(GLFWwindow* wnd, double xOffset, double yOffset);

	GLFWwindow* window;

public:
	static Input* getInput();
	void setupCallbacks(GLFWwindow* wnd); //Must call this when window is created to make GLFW callback functions work!

	bool getKeyInfo(int key); //Used for both keyboard and mouse, returns whether a key is currently active
	bool justPressed(int key); //Used for both keyboard and mouse, returns whether a key was pressed this frame

	void getCursor(double &x, double &y);
	void centerCursor(int xcenter, int ycenter);

	double getScrollValue();


	void clearOnPress(); //Must be called at the end of each frame!

	static void release(); //Must be called at the termination of the game

	char keyToChar(int key); //Translates a key's assigned character to an actual char
	int getGLFWkeyFromString(string n);
	

};

#endif