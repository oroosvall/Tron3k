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
bool Input::TAB = false;
bool Input::F1 = false;
bool Input::F2 = false;
bool Input::F3 = false;
bool Input::F4 = false;
bool Input::F5 = false;
bool Input::F6 = false;
bool Input::F7 = false;
bool Input::F8 = false;
bool Input::F9 = false;
bool Input::F10 = false;
bool Input::F11 = false;
bool Input::F12 = false;

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
bool Input::pTAB = false;
bool Input::pF1 = false;
bool Input::pF2 = false;
bool Input::pF3 = false;
bool Input::pF4 = false;
bool Input::pF5 = false;
bool Input::pF6 = false;
bool Input::pF7 = false;
bool Input::pF8 = false;
bool Input::pF9 = false;
bool Input::pF10 = false;
bool Input::pF11 = false;
bool Input::pF12 = false;

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
	window = wnd;
	glfwSetKeyCallback(window, callbackKeyboard);
	glfwSetMouseButtonCallback(window, callbackMouseButton);
	glfwSetCursorPosCallback(window, callbackCursorPos);
	glfwSetScrollCallback(window, callbackScroll);
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
	case GLFW_KEY_TAB:
		TAB = (bool)action;
		if (action == GLFW_PRESS)
		{
			pTAB = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F1:
		F1 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF1 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F2:
		F2 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF2 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F3:
		F3 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF3 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F4:
		F4 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF4 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F5:
		F5 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF5 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F6:
		F6 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF6 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F7:
		F7 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF7 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F8:
		F8 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF8 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F9:
		F9 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF9 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F10:
		F10 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF10 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F11:
		F11 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF11 = true;
			keyPressedThisFrame = true;
		}
		break;
	case GLFW_KEY_F12:
		F12 = (bool)action;
		if (action == GLFW_PRESS)
		{
			pF12 = true;
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
	case GLFW_KEY_TAB:
		return TAB;
	case GLFW_KEY_F1:
		return F1;
	case GLFW_KEY_F2:
		return F2;
	case GLFW_KEY_F3:
		return F3;
	case GLFW_KEY_F4:
		return F4;
	case GLFW_KEY_F5:
		return F5;
	case GLFW_KEY_F6:
		return F6;
	case GLFW_KEY_F7:
		return F7;
	case GLFW_KEY_F8:
		return F8;
	case GLFW_KEY_F9:
		return F9;
	case GLFW_KEY_F10:
		return F10;
	case GLFW_KEY_F11:
		return F11;
	case GLFW_KEY_F12:
		return F12;

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
		case GLFW_KEY_TAB: //Dash/Underscore
			return pTAB;

		case GLFW_KEY_F1:
			return pF1;
		case GLFW_KEY_F2:
			return pF2;
		case GLFW_KEY_F3:
			return pF3;
		case GLFW_KEY_F4:
			return pF4;
		case GLFW_KEY_F5:
			return pF5;
		case GLFW_KEY_F6:
			return pF6;
		case GLFW_KEY_F7:
			return pF7;
		case GLFW_KEY_F8:
			return pF8;
		case GLFW_KEY_F9:
			return pF9;
		case GLFW_KEY_F10:
			return pF10;
		case GLFW_KEY_F11:
			return pF11;
		case GLFW_KEY_F12:
			return pF12;

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
		pTAB = false;
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

		pF1 = false;
		pF2 = false;
		pF3 = false;
		pF4 = false;
		pF5 = false;
		pF6 = false;
		pF7 = false;
		pF8 = false;
		pF9 = false;
		pF10 = false;
		pF11 = false;
		pF12 = false;

		pLMB = false;
		pRMB = false;
		pMMB = false;

		scrollVal = 0.0;

		keyPressedThisFrame = false;
	}
}

double Input::getScrollValue()
{
	if (scrollLastFrame != scrollVal)
	{
		scrollLastFrame = scrollVal;
		return scrollVal;
	}
	return 0.0;
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

int Input::getGLFWkeyFromString(string n)
{
	transform(n.begin(), n.end(), n.begin(), ::toupper);
	if (n == "Q")
		return GLFW_KEY_Q;
	if (n == "W")
		return GLFW_KEY_W;
	if (n == "E")
		return GLFW_KEY_E;
	if (n == "R")
		return GLFW_KEY_R;
	if (n == "T")
		return GLFW_KEY_T;
	if (n == "Y")
		return GLFW_KEY_Y;
	if (n == "U")
		return GLFW_KEY_U;
	if (n == "I")
		return GLFW_KEY_I;
	if (n == "O")
		return GLFW_KEY_O;
	if (n == "P")
		return GLFW_KEY_P;

	if (n == "A")
		return GLFW_KEY_A;
	if (n == "S")
		return GLFW_KEY_S;
	if (n == "D")
		return GLFW_KEY_D;
	if (n == "F")
		return GLFW_KEY_F;
	if (n == "G")
		return GLFW_KEY_G;
	if (n == "H")
		return GLFW_KEY_H;
	if (n == "J")
		return GLFW_KEY_J;
	if (n == "K")
		return GLFW_KEY_K;
	if (n == "L")
		return GLFW_KEY_L;

	if (n == "Z")
		return GLFW_KEY_Z;
	if (n == "X")
		return GLFW_KEY_X;
	if (n == "C")
		return GLFW_KEY_C;
	if (n == "V")
		return GLFW_KEY_V;
	if (n == "B")
		return GLFW_KEY_B;
	if (n == "N")
		return GLFW_KEY_N;
	if (n == "M")
		return GLFW_KEY_J;

	if (n == "1")
		return GLFW_KEY_1;
	if (n == "2")
		return GLFW_KEY_2;
	if (n == "3")
		return GLFW_KEY_3;
	if (n == "4")
		return GLFW_KEY_4;
	if (n == "5")
		return GLFW_KEY_5;
	if (n == "6")
		return GLFW_KEY_6;
	if (n == "7")
		return GLFW_KEY_7;
	if (n == "8")
		return GLFW_KEY_8;
	if (n == "9")
		return GLFW_KEY_9;
	if (n == "0")
		return GLFW_KEY_0;

	if (n == "NUM1")
		return GLFW_KEY_KP_1;
	if (n == "NUM2")
		return GLFW_KEY_KP_2;
	if (n == "NUM3")
		return GLFW_KEY_KP_3;
	if (n == "NUM4")
		return GLFW_KEY_KP_4;
	if (n == "NUM5")
		return GLFW_KEY_KP_5;
	if (n == "NUM6")
		return GLFW_KEY_KP_6;
	if (n == "NUM7")
		return GLFW_KEY_KP_7;
	if (n == "NUM8")
		return GLFW_KEY_KP_8;
	if (n == "NUM9")
		return GLFW_KEY_KP_9;
	if (n == "NUM0")
		return GLFW_KEY_KP_0;

	if (n == "PERIOD" || n == ".")
		return GLFW_KEY_PERIOD;
	if (n == "COMMA" || n == ",")
		return GLFW_KEY_COMMA;
	if (n == "DASH" || n == "-")
		return SWE_KEY_DASH;
	if (n == "APOSTROPHE" || n == "'")
		return SWE_KEY_APOSTROPHE;
	if (n == "PLUS" || n == "+")
		return SWE_KEY_PLUS;
	if (n == "TAB")
		return GLFW_KEY_TAB;
	if (n == "SHIFT")
		return GLFW_KEY_LEFT_SHIFT;
	if (n == "CTRL")
		return GLFW_KEY_LEFT_CONTROL;
	if (n == "ALT")
		return GLFW_KEY_LEFT_ALT;
	if (n == "SPACE")
		return GLFW_KEY_SPACE;
	if (n == "ENTER")
		return GLFW_KEY_ENTER;

	if (n == "F1")
		return GLFW_KEY_F1;
	if (n == "F2")
		return GLFW_KEY_F2;
	if (n == "F3")
		return GLFW_KEY_F3;
	if (n == "F4")
		return GLFW_KEY_F4;
	if (n == "F5")
		return GLFW_KEY_F5;
	if (n == "F6")
		return GLFW_KEY_F6;
	if (n == "F7")
		return GLFW_KEY_F7;
	if (n == "F8")
		return GLFW_KEY_F8;
	if (n == "F9")
		return GLFW_KEY_F9;
	if (n == "F10")
		return GLFW_KEY_F10;
	if (n == "F11")
		return GLFW_KEY_F11;
	if (n == "F12")
		return GLFW_KEY_F12;

	if (n == "M1")
		return GLFW_MOUSE_BUTTON_1;
	if (n == "M2")
		return GLFW_MOUSE_BUTTON_2;
	if (n == "M3")
		return GLFW_MOUSE_BUTTON_3;
	if (n == "M4")
		return GLFW_MOUSE_BUTTON_4;
	if (n == "M5")
		return GLFW_MOUSE_BUTTON_5;
	if (n == "M6")
		return GLFW_MOUSE_BUTTON_6;
	if (n == "M7")
		return GLFW_MOUSE_BUTTON_7;
	if (n == "M8")
		return GLFW_MOUSE_BUTTON_8;

	if (n == "UP")
		return GLFW_KEY_UP;
	if (n == "DOWN")
		return GLFW_KEY_DOWN;
	if (n == "LEFT")
		return GLFW_KEY_LEFT;
	if (n == "RIGHT")
		return GLFW_KEY_RIGHT;

	return 0;
}

void Input::centerCursor(int centerx, int centery)
{
	glfwSetCursorPos(window, centerx, centery);
}