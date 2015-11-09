#ifndef INPUT_H
#define INPUT_H

#include <GLFW\glfw3.h>

class Input
{
private:
	static Input* singleton;
	Input();

	static bool Q,W,E,R,T,Y,U,I,O,P,A,S,D,F,G,H,J,K,L,Z,X,C,V,B,N,M,
		SHIFT,CTRL,ALT,SPACE,ESC, ENTER,
		NUM1,NUM2,NUM3,NUM4,NUM5,NUM6,NUM7,NUM8,NUM9,NUM0,
		DOT,COMMA;
	static bool rQ, rW, rE, rR, rT, rY, rU, rI, rO, rP, rA, rS, rD, rF, rG, rH, rJ, rK, rL, rZ, rX, rC, rV, rB, rN, rM,
		rSHIFT, rCTRL, rALT, rSPACE, rESC, rENTER,
		rNUM1, rNUM2, rNUM3, rNUM4, rNUM5, rNUM6, rNUM7, rNUM8, rNUM9, rNUM0,
		rDOT, rCOMMA; //Button was released this frame

	static void callbackKeyboard(GLFWwindow* wnd, int key, int scancode, int action, int modkeys);

	static bool LMB, RMB, MMB;
	static bool rLMB, rRMB, rMMB;
	static double cursorX, cursorY;
	static void callbackMouseButton(GLFWwindow* wnd, int button, int action, int mods);
	static void callbackCursorPos(GLFWwindow* wnd, double x, double y);

	static double scrollVal;
	static void callbackScroll(GLFWwindow* wnd, double xOffset, double yOffset);

public:
	static Input* getInput();
	void setupCallbacks(GLFWwindow* wnd);

	bool getKeyInfo(int key);
	bool justReleased(int key);

	void getCursor(double &x, double &y);

	double getScrollValue();

	void clearOnRelease();

	static void release();
};

#endif