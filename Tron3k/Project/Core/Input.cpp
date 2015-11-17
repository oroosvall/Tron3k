#include "Input.h"

bool Input::Q = false;
bool Input::W = false;
bool Input::E = false;
bool Input::R = false;
bool Input::T = false;
bool Input::Y = false;
bool Input::U = false;
bool Input::I = false;
bool Input::O = false;
bool Input::P = false;
bool Input::A = false;
bool Input::S = false;
bool Input::D = false;
bool Input::F = false;
bool Input::G = false;
bool Input::H = false;
bool Input::J = false;
bool Input::K = false;
bool Input::L = false;
bool Input::Z = false;
bool Input::X = false;
bool Input::C = false;
bool Input::V = false;
bool Input::B = false;
bool Input::N = false;
bool Input::M = false;
bool Input::SHIFT = false;
bool Input::CTRL = false;
bool Input::ALT = false;
bool Input::SPACE = false;
bool Input::ESC = false;
bool Input::ENTER = false;
bool Input::DOT = false;
bool Input::COMMA = false;
bool Input::NUM1 = false;
bool Input::NUM2 = false;
bool Input::NUM3 = false;
bool Input::NUM4 = false;
bool Input::NUM5 = false;
bool Input::NUM6 = false;
bool Input::NUM7 = false;
bool Input::NUM8 = false;
bool Input::NUM9 = false;
bool Input::NUM0 = false;
bool Input::BACKSPACE = false;
bool Input::PLUS = false;
bool Input::APOSTROPHE = false;
bool Input::DASH = false;

bool Input::pQ = false;
bool Input::pW = false;
bool Input::pE = false;
bool Input::pR = false;
bool Input::pT = false;
bool Input::pY = false;
bool Input::pU = false;
bool Input::pI = false;
bool Input::pO = false;
bool Input::pP = false;
bool Input::pA = false;
bool Input::pS = false;
bool Input::pD = false;
bool Input::pF = false;
bool Input::pG = false;
bool Input::pH = false;
bool Input::pJ = false;
bool Input::pK = false;
bool Input::pL = false;
bool Input::pZ = false;
bool Input::pX = false;
bool Input::pC = false;
bool Input::pV = false;
bool Input::pB = false;
bool Input::pN = false;
bool Input::pM = false;
bool Input::pSHIFT = false;
bool Input::pCTRL = false;
bool Input::pALT = false;
bool Input::pSPACE = false;
bool Input::pESC = false;
bool Input::pENTER = false;
bool Input::pDOT = false;
bool Input::pCOMMA = false;
bool Input::pNUM1 = false;
bool Input::pNUM2 = false;
bool Input::pNUM3 = false;
bool Input::pNUM4 = false;
bool Input::pNUM5 = false;
bool Input::pNUM6 = false;
bool Input::pNUM7 = false;
bool Input::pNUM8 = false;
bool Input::pNUM9 = false;
bool Input::pNUM0 = false;
bool Input::pBACKSPACE = false;
bool Input::pPLUS = false;
bool Input::pAPOSTROPHE = false;
bool Input::pDASH = false;

bool Input::LMB = false;
bool Input::RMB = false;
bool Input::MMB = false;
bool Input::pLMB = false;
bool Input::pRMB = false;
bool Input::pMMB = false;
double Input::cursorX = 0.0;
double Input::cursorY = 0.0;

double Input::scrollVal = 0.0;

bool Input::keyPressedThisFrame = false;

Input* Input::singleton = nullptr;

Input* Input::getInput()
{
	if (singleton == nullptr)
		singleton = new Input();
	return singleton;
}

void Input::release()
{
	if (singleton)
		delete singleton;
}

Input::Input()
{
}

void Input::setupCallbacks(GLFWwindow* wnd)
{
	glfwSetKeyCallback(wnd, callbackKeyboard);
	glfwSetMouseButtonCallback(wnd, callbackMouseButton);
	glfwSetCursorPosCallback(wnd, callbackCursorPos);
	glfwSetScrollCallback(wnd, callbackScroll);
}
/*

All callback functions work in the same way: bool <Button> = (bool)action. This is because GLFW_RELEASE = 0 and GLFW_PRESS = 1, which simulates false/true nicely.
We entirely ignore GLFW_REPEAT, since it is uninteresting for our game.

*/
void Input::callbackKeyboard(GLFWwindow* wnd, int key, int scancode, int action, int modkeys)
{
	switch (key)
	{
	case GLFW_KEY_Q:
		Q = (bool)action;
		if (action == GLFW_PRESS)
		{
			pQ = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_W:
		W = (bool)action;
		if (action == GLFW_PRESS)
		{
			pW = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_E:
		E = action;
		if (action == GLFW_PRESS)
		{
			pE = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_R:
		R = (bool)action;
		if (action == GLFW_PRESS)
		{
			pR = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_T:
		T = (bool)action;
		if (action == GLFW_PRESS)
		{
			pT = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_Y:
		Y = (bool)action;
		if (action == GLFW_PRESS)
		{
			pY = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_U:
		U = (bool)action;
		if (action == GLFW_PRESS)
		{
			pU = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_I:
		I = (bool)action;
		if (action == GLFW_PRESS)
		{
			pI = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_O:
		O = (bool)action;
		if (action == GLFW_PRESS)
		{
			pO = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_P:
		P = (bool)action;
		if (action == GLFW_PRESS)
		{
			pP = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_A:
		A = (bool)action;
		if (action == GLFW_PRESS)
		{
			pA = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_S:
		S = (bool)action;
		if (action == GLFW_PRESS)
		{
			pS = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_D:
		D = (bool)action;
		if (action == GLFW_PRESS)
		{
			pD = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F:
		F = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_G:
		G = (bool)action;
		if (action == GLFW_PRESS)
		{
			pG = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_H:
		H = (bool)action;
		if (action == GLFW_PRESS)
		{
			pH = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_J:
		J = (bool)action;
		if (action == GLFW_PRESS)
		{
			pJ = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_K:
		K = (bool)action;
		if (action == GLFW_PRESS)
		{
			pK = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_L:
		L = (bool)action;
		if (action == GLFW_PRESS)
		{
			pL = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_Z:
		Z = (bool)action;
		if (action == GLFW_PRESS)
		{
			pZ = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_X:
		X = (bool)action;
		if (action == GLFW_PRESS)
		{
			pX = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_C:
		C = (bool)action;
		if (action == GLFW_PRESS)
		{
			pC = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_V:
		V = (bool)action;
		if (action == GLFW_PRESS)
		{
			pV = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_B:
		B = (bool)action;
		if (action == GLFW_PRESS)
		{
			pB = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_N:
		N = (bool)action;
		if (action == GLFW_PRESS)
		{
			pN = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_M:
		M = (bool)action;
		if (action == GLFW_PRESS)
		{
			pM = true;
			keyPressedThisFrame = true;
		}
		break;

	case GLFW_KEY_1:
		NUM1 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM1 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_2:
		NUM2 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM2 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_3:
		NUM3 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM3 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_4:
		NUM4 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM4 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_5:
		NUM5 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM5 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_6:
		NUM6 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM6 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_7:
		NUM7 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM7 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_8:
		NUM8 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM8 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_9:
		NUM9 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM9 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_0:
		NUM0 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pNUM0 = true;
			keyPressedThisFrame = true;
		}
		break;

	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_RIGHT_SHIFT:
		SHIFT = (bool)action;
		if (action == GLFW_PRESS)
		{
			pSHIFT = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:
		CTRL = (bool)action;
		if (action == GLFW_PRESS)
		{
			pCTRL = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_LEFT_ALT:
	case GLFW_KEY_RIGHT_ALT:
		ALT = (bool)action;
		if (action == GLFW_PRESS)
		{
			pALT = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_SPACE:
		SPACE = (bool)action;
		if (action == GLFW_PRESS)
		{
			pSPACE = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_ENTER:
		ENTER = (bool)action;
		if (action == GLFW_PRESS)
		{
			pENTER = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_ESCAPE:
		ESC = (bool)action;
		if (action == GLFW_PRESS)
		{
			pESC = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_PERIOD:
		DOT = (bool)action;
		if (action == GLFW_PRESS)
		{
			pDOT = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_COMMA:
		COMMA = (bool)action;
		if (action == GLFW_PRESS)
		{
			pCOMMA = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_BACKSPACE:
		BACKSPACE = (bool)action;
		if (action == GLFW_PRESS)
		{
			pBACKSPACE = true;
			keyPressedThisFrame = true;
		}
		break;
		//Here begins the cheating! Check the defines in .h for info
	case SWE_KEY_APOSTROPHE: //Apostrophe/Asterisk
		APOSTROPHE = (bool)action;
		if (action == GLFW_PRESS)
		{
			pAPOSTROPHE = true;
			keyPressedThisFrame = true;
		}
		break;
	case SWE_KEY_PLUS: //Plus/Questionmark
		PLUS = (bool)action;
		if (action == GLFW_PRESS)
		{
			pPLUS = true;
			keyPressedThisFrame = true;
		}
		break;
	case SWE_KEY_DASH: //Dash/Underscore
		DASH = (bool)action;
		if (action == GLFW_PRESS)
		{
			pDASH = true;
			keyPressedThisFrame = true;
		}
		break;
	}
}

void Input::callbackMouseButton(GLFWwindow* wnd, int button, int action, int mods)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		LMB = (bool)action;
		if (action == GLFW_PRESS)
		{
			pLMB = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		RMB = (bool)action;
		if (action == GLFW_PRESS)
		{
			pRMB = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		MMB = (bool)action;
		if (action == GLFW_PRESS)
		{
			pMMB = true;
			keyPressedThisFrame = true;
		}
		break;
	}
}
/*
Here, we ignore xOffset. Who uses an x-scrolling mouse? Jesus. Our mice only respond with +/- 1 for this either way since their xScroll is binary and function as keys.
*/
void Input::callbackScroll(GLFWwindow* wnd, double xOffset, double yOffset)
{
	scrollVal = yOffset;
}

void Input::callbackCursorPos(GLFWwindow* wnd, double x, double y)
{
	cursorX = x;
	cursorY = y;
}

void Input::getCursor(double &x, double &y)
{
	x = cursorX;
	y = cursorY;
}

bool Input::getKeyInfo(int key)
{
	switch (key)
	{
	case GLFW_KEY_Q:
		return Q;
	case GLFW_KEY_W:
		return W;
	case GLFW_KEY_E:
		return E;
	case GLFW_KEY_R:
		return R;
	case GLFW_KEY_T:
		return T;
	case GLFW_KEY_Y:
		return Y;
	case GLFW_KEY_U:
		return U;
	case GLFW_KEY_I:
		return I;
	case GLFW_KEY_O:
		return O;
	case GLFW_KEY_P:
		return P;
	case GLFW_KEY_A:
		return A;
	case GLFW_KEY_S:
		return S;
	case GLFW_KEY_D:
		return D;
	case GLFW_KEY_F:
		return F;
	case GLFW_KEY_G:
		return G;
	case GLFW_KEY_H:
		return H;
	case GLFW_KEY_J:
		return J;
	case GLFW_KEY_K:
		return K;
	case GLFW_KEY_L:
		return L;
	case GLFW_KEY_Z:
		return Z;
	case GLFW_KEY_X:
		return X;
	case GLFW_KEY_C:
		return C;
	case GLFW_KEY_V:
		return V;
	case GLFW_KEY_B:
		return B;
	case GLFW_KEY_N:
		return N;
	case GLFW_KEY_M:
		return M;

	case GLFW_KEY_1:
		return NUM1;
	case GLFW_KEY_2:
		return NUM2;
	case GLFW_KEY_3:
		return NUM3;
	case GLFW_KEY_4:
		return NUM4;
	case GLFW_KEY_5:
		return NUM5;
	case GLFW_KEY_6:
		return NUM6;
	case GLFW_KEY_7:
		return NUM7;
	case GLFW_KEY_8:
		return NUM8;
	case GLFW_KEY_9:
		return NUM9;
	case GLFW_KEY_0:
		return NUM0;

	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_RIGHT_SHIFT:
		return SHIFT;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:
		return CTRL;
	case GLFW_KEY_LEFT_ALT:
	case GLFW_KEY_RIGHT_ALT:
		return ALT;
	case GLFW_KEY_SPACE:
		return SPACE;
	case GLFW_KEY_ENTER:
		return ENTER;
	case GLFW_KEY_ESCAPE:
		return ESC;
	case GLFW_KEY_PERIOD:
		return DOT;
	case GLFW_KEY_COMMA:
		return COMMA;
	case GLFW_KEY_BACKSPACE:
		return BACKSPACE;
	case SWE_KEY_APOSTROPHE: //Apostrophe/Asterisk
		return APOSTROPHE;
	case SWE_KEY_PLUS: //Plus/Questionmark
		return PLUS;
	case SWE_KEY_DASH: //Dash/Underscore
		return DASH;

	case GLFW_MOUSE_BUTTON_LEFT:
		return LMB;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return RMB;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return MMB;
	default:
		return false;
	}
}

bool Input::justPressed(int key)
{
	if (keyPressedThisFrame)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
			return pQ;
		case GLFW_KEY_W:
			return pW;
		case GLFW_KEY_E:
			return pE;
		case GLFW_KEY_R:
			return pR;
		case GLFW_KEY_T:
			return pT;
		case GLFW_KEY_Y:
			return pY;
		case GLFW_KEY_U:
			return pU;
		case GLFW_KEY_I:
			return pI;
		case GLFW_KEY_O:
			return pO;
		case GLFW_KEY_P:
			return pP;
		case GLFW_KEY_A:
			return pA;
		case GLFW_KEY_S:
			return pS;
		case GLFW_KEY_D:
			return pD;
		case GLFW_KEY_F:
			return pF;
		case GLFW_KEY_G:
			return pG;
		case GLFW_KEY_H:
			return pH;
		case GLFW_KEY_J:
			return pJ;
		case GLFW_KEY_K:
			return pK;
		case GLFW_KEY_L:
			return pL;
		case GLFW_KEY_Z:
			return pZ;
		case GLFW_KEY_X:
			return pX;
		case GLFW_KEY_C:
			return pC;
		case GLFW_KEY_V:
			return pV;
		case GLFW_KEY_B:
			return pB;
		case GLFW_KEY_N:
			return pN;
		case GLFW_KEY_M:
			return pM;

		case GLFW_KEY_1:
			return pNUM1;
		case GLFW_KEY_2:
			return pNUM2;
		case GLFW_KEY_3:
			return pNUM3;
		case GLFW_KEY_4:
			return pNUM4;
		case GLFW_KEY_5:
			return pNUM5;
		case GLFW_KEY_6:
			return pNUM6;
		case GLFW_KEY_7:
			return pNUM7;
		case GLFW_KEY_8:
			return pNUM8;
		case GLFW_KEY_9:
			return pNUM9;
		case GLFW_KEY_0:
			return pNUM0;

		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			return pSHIFT;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL:
			return pCTRL;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			return pALT;
		case GLFW_KEY_SPACE:
			return pSPACE;
		case GLFW_KEY_ENTER:
			return pENTER;
		case GLFW_KEY_ESCAPE:
			return pESC;
		case GLFW_KEY_PERIOD:
			return pDOT;
		case GLFW_KEY_COMMA:
			return pCOMMA;
		case GLFW_KEY_BACKSPACE:
			return pBACKSPACE;
		case SWE_KEY_APOSTROPHE: //Apostrophe/Asterisk
			return pAPOSTROPHE;
		case SWE_KEY_PLUS: //Plus/Questionmark
			return pPLUS;
		case SWE_KEY_DASH: //Dash/Underscore
			return pDASH;

		case GLFW_MOUSE_BUTTON_LEFT:
			return pLMB;
		case GLFW_MOUSE_BUTTON_RIGHT:
			return pRMB;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			return pMMB;
		default:
			return false;
		}
	}
	return false;
}

void Input::clearOnPress()
{
	if (keyPressedThisFrame)
	{
		pQ = false;
		pW = false;
		pE = false;
		pR = false;
		pT = false;
		pY = false;
		pU = false;
		pI = false;
		pO = false;
		pP = false;
		pA = false;
		pS = false;
		pD = false;
		pF = false;
		pG = false;
		pH = false;
		pJ = false;
		pK = false;
		pL = false;
		pZ = false;
		pX = false;
		pC = false;
		pV = false;
		pB = false;
		pN = false;
		pM = false;
		pSHIFT = false;
		pCTRL = false;
		pALT = false;
		pSPACE = false;
		pESC = false;
		pENTER = false;
		pDOT = false;
		pCOMMA = false;
		pBACKSPACE = false;
		pAPOSTROPHE = false;
		pPLUS = false;
		pDASH = false;
		pNUM1 = false;
		pNUM2 = false;
		pNUM3 = false;
		pNUM4 = false;
		pNUM5 = false;
		pNUM6 = false;
		pNUM7 = false;
		pNUM8 = false;
		pNUM9 = false;
		pNUM0 = false;

		pLMB = false;
		pRMB = false;
		pMMB = false;

		scrollVal = 0.0;

		keyPressedThisFrame = false;
	}
}

double Input::getScrollValue()
{
	return scrollVal;
}

char Input::keyToChar(int key)
{
	switch (key)
	{
	case GLFW_KEY_Q:
		return 'Q';
	case GLFW_KEY_W:
		return 'W';
	case GLFW_KEY_E:
		return 'E';
	case GLFW_KEY_R:
		return 'R';
	case GLFW_KEY_T:
		return 'T';
	case GLFW_KEY_Y:
		return 'Y';
	case GLFW_KEY_U:
		return 'U';
	case GLFW_KEY_I:
		return 'I';
	case GLFW_KEY_O:
		return 'O';
	case GLFW_KEY_P:
		return 'P';
	case GLFW_KEY_A:
		return 'A';
	case GLFW_KEY_S:
		return 'S';
	case GLFW_KEY_D:
		return 'D';
	case GLFW_KEY_F:
		return 'F';
	case GLFW_KEY_G:
		return 'G';
	case GLFW_KEY_H:
		return 'H';
	case GLFW_KEY_J:
		return 'J';
	case GLFW_KEY_K:
		return 'K';
	case GLFW_KEY_L:
		return 'L';
	case GLFW_KEY_Z:
		return 'Z';
	case GLFW_KEY_X:
		return 'X';
	case GLFW_KEY_C:
		return 'C';
	case GLFW_KEY_V:
		return 'V';
	case GLFW_KEY_B:
		return 'B';
	case GLFW_KEY_N:
		return 'N';
	case GLFW_KEY_M:
		return 'M';

	case GLFW_KEY_1:
		return '1';
	case GLFW_KEY_2:
		return '2';
	case GLFW_KEY_3:
		return '3';
	case GLFW_KEY_4:
		return '4';
	case GLFW_KEY_5:
		return '5';
	case GLFW_KEY_6:
		return '6';
	case GLFW_KEY_7:
		return '7';
	case GLFW_KEY_8:
		return '8';
	case GLFW_KEY_9:
		return '9';
	case GLFW_KEY_0:
		return '0';

	case GLFW_KEY_SPACE:
		return ' ';
	case GLFW_KEY_PERIOD:
		return '.';
	case GLFW_KEY_COMMA:
		return ',';
	case SWE_KEY_APOSTROPHE: //Apostrophe/Asterisk
		return '\'';
	case SWE_KEY_PLUS: //Plus/Questionmark
		return '+';
	case SWE_KEY_DASH: //Dash/Underscore
		return '-';

	default:
		return '\0'; //Not sure how safe this is
	}
}