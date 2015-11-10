#ifndef CONSOLE_H
#define CONSOLE_H

#include <GLFW\glfw3.h>

#include <string>
#include <vector>

#include "Input.h"

#define MAXHISTORY 25
#define VALIDKEYS 39

using namespace std;

class Console
{
private:
	const int validKeyboardInputs[VALIDKEYS] = { GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T, GLFW_KEY_Y, GLFW_KEY_U, GLFW_KEY_I, GLFW_KEY_O, GLFW_KEY_P,
		GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L,
		GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B, GLFW_KEY_N, GLFW_KEY_M, GLFW_KEY_COMMA, GLFW_KEY_PERIOD, GLFW_KEY_SPACE,
		GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_0 };
	string msg;
	string history[MAXHISTORY];
	int latestMsg = MAXHISTORY-1;

	void printConsole();
public:
	Console();
	~Console();
	void update();
};

#endif