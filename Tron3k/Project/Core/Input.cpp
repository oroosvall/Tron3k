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

bool Input::rQ = false;
bool Input::rW = false;
bool Input::rE = false;
bool Input::rR = false;
bool Input::rT = false;
bool Input::rY = false;
bool Input::rU = false;
bool Input::rI = false;
bool Input::rO = false;
bool Input::rP = false;
bool Input::rA = false;
bool Input::rS = false;
bool Input::rD = false;
bool Input::rF = false;
bool Input::rG = false;
bool Input::rH = false;
bool Input::rJ = false;
bool Input::rK = false;
bool Input::rL = false;
bool Input::rZ = false;
bool Input::rX = false;
bool Input::rC = false;
bool Input::rV = false;
bool Input::rB = false;
bool Input::rN = false;
bool Input::rM = false;
bool Input::rSHIFT = false;
bool Input::rCTRL = false;
bool Input::rALT = false;
bool Input::rSPACE = false;
bool Input::rESC = false;
bool Input::rENTER = false;
bool Input::rDOT = false;
bool Input::rCOMMA = false;
bool Input::rNUM1 = false;
bool Input::rNUM2 = false;
bool Input::rNUM3 = false;
bool Input::rNUM4 = false;
bool Input::rNUM5 = false;
bool Input::rNUM6 = false;
bool Input::rNUM7 = false;
bool Input::rNUM8 = false;
bool Input::rNUM9 = false;
bool Input::rNUM0 = false;

bool Input::LMB = false;
bool Input::RMB = false;
bool Input::MMB = false;
bool Input::rLMB = false;
bool Input::rRMB = false;
bool Input::rMMB = false;
double Input::cursorX = 0.0;
double Input::cursorY = 0.0;

double Input::scrollVal = 0.0;

Input* Input::singleton = 0;

Input* Input::getInput()
{
	if (singleton == 0)
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

void Input::callbackKeyboard(GLFWwindow* wnd, int key, int scancode, int action, int modkeys)
{
	switch (key)
	{
	case GLFW_KEY_Q:
		Q = (bool)action;
		if (action == GLFW_RELEASE)
			rQ = true;
		break;
	case GLFW_KEY_W:
		W = (bool)action;
		if (action == GLFW_RELEASE)
			rW = true;
		break;
	case GLFW_KEY_E:
		E = action;
		if (action == GLFW_RELEASE)
			rE = true;
		break;
	case GLFW_KEY_R:
		R = (bool)action;
		if (action == GLFW_RELEASE)
			rR = true;
		break;
	case GLFW_KEY_T:
		T = (bool)action;
		if (action == GLFW_RELEASE)
			rT = true;
		break;
	case GLFW_KEY_Y:
		Y = (bool)action;
		if (action == GLFW_RELEASE)
			rY = true;
		break;
	case GLFW_KEY_U:
		U = (bool)action;
		if (action == GLFW_RELEASE)
			rU = true;
		break;
	case GLFW_KEY_I:
		I = (bool)action;
		if (action == GLFW_RELEASE)
			rI = true;
		break;
	case GLFW_KEY_O:
		O = (bool)action;
		if (action == GLFW_RELEASE)
			rO = true;
		break;
	case GLFW_KEY_P:
		P = (bool)action;
		if (action == GLFW_RELEASE)
			rP = true;
		break;
	case GLFW_KEY_A:
		A = (bool)action;
		if (action == GLFW_RELEASE)
			rA = true;
		break;
	case GLFW_KEY_S:
		S = (bool)action;
		if (action == GLFW_RELEASE)
			rS = true;
		break;
	case GLFW_KEY_D:
		D = (bool)action;
		if (action == GLFW_RELEASE)
			rD = true;
		break;
	case GLFW_KEY_F:
		F = (bool)action;
		if (action == GLFW_RELEASE)
			rF = true;
		break;
	case GLFW_KEY_G:
		G = (bool)action;
		if (action == GLFW_RELEASE)
			rG = true;
		break;
	case GLFW_KEY_H:
		H = (bool)action;
		if (action == GLFW_RELEASE)
			rH = true;
		break;
	case GLFW_KEY_J:
		J = (bool)action;
		if (action == GLFW_RELEASE)
			rJ = true;
		break;
	case GLFW_KEY_K:
		K = (bool)action;
		if (action == GLFW_RELEASE)
			rK = true;
		break;
	case GLFW_KEY_L:
		L = (bool)action;
		if (action == GLFW_RELEASE)
			rL = true;
		break;
	case GLFW_KEY_Z:
		Z = (bool)action;
		if (action == GLFW_RELEASE)
			rZ = true;
		break;
	case GLFW_KEY_X:
		X = (bool)action;
		if (action == GLFW_RELEASE)
			rX = true;
		break;
	case GLFW_KEY_C:
		C = (bool)action;
		if (action == GLFW_RELEASE)
			rC = true;
		break;
	case GLFW_KEY_V:
		V = (bool)action;
		if (action == GLFW_RELEASE)
			rV = true;
		break;
	case GLFW_KEY_B:
		B = (bool)action;
		if (action == GLFW_RELEASE)
			rB = true;
		break;
	case GLFW_KEY_N:
		N = (bool)action;
		if (action == GLFW_RELEASE)
			rN = true;
		break;
	case GLFW_KEY_M:
		M = (bool)action;
		if (action == GLFW_RELEASE)
			rM = true;
		break;

	case GLFW_KEY_1:
		NUM1 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM1 = true;
		break;
	case GLFW_KEY_2:
		NUM2 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM2 = true;
		break;
	case GLFW_KEY_3:
		NUM3 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM3 = true;
		break;
	case GLFW_KEY_4:
		NUM4 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM4 = true;
		break;
	case GLFW_KEY_5:
		NUM5 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM5 = true;
		break;
	case GLFW_KEY_6:
		NUM6 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM6 = true;
		break;
	case GLFW_KEY_7:
		NUM7 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM7 = true;
		break;
	case GLFW_KEY_8:
		NUM8 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM8 = true;
		break;
	case GLFW_KEY_9:
		NUM9 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM9 = true;
		break;
	case GLFW_KEY_0:
		NUM0 = (bool)action;
		if (action == GLFW_RELEASE)
			rNUM0 = true;
		break;

	case GLFW_KEY_LEFT_SHIFT:
		SHIFT = (bool)action;
		if (action == GLFW_RELEASE)
			rSHIFT = true;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		CTRL = (bool)action;
		if (action == GLFW_RELEASE)
			rCTRL = true;
		break;
	case GLFW_KEY_LEFT_ALT:
		ALT = (bool)action;
		if (action == GLFW_RELEASE)
			rALT = true;
		break;
	case GLFW_KEY_SPACE:
		SPACE = (bool)action;
		if (action == GLFW_RELEASE)
			rSPACE = true;
		break;
	case GLFW_KEY_ENTER:
		ENTER = (bool)action;
		if (action == GLFW_RELEASE)
			rENTER = true;
		break;
	case GLFW_KEY_ESCAPE:
		ESC = (bool)action;
		if (action == GLFW_RELEASE)
			rESC = true;
		break;
	case GLFW_KEY_PERIOD:
		DOT = (bool)action;
		if (action == GLFW_RELEASE)
			rDOT = true;
		break;
	case GLFW_KEY_COMMA:
		COMMA = (bool)action;
		if (action == GLFW_RELEASE)
			rCOMMA = true;
		break;
	}
}

void Input::callbackMouseButton(GLFWwindow* wnd, int button, int action, int mods)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		LMB = (bool)action;
		if (action == GLFW_RELEASE)
			rLMB = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		RMB = (bool)action;
		if (action == GLFW_RELEASE)
			rRMB = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		MMB = (bool)action;
		if (action == GLFW_RELEASE)
			rMMB = true;
		break;
	}
}

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
		return SHIFT;
	case GLFW_KEY_LEFT_CONTROL:
		return CTRL;
	case GLFW_KEY_LEFT_ALT:
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

bool Input::justReleased(int key)
{
	switch (key)
	{
	case GLFW_KEY_Q:
		return rQ;
	case GLFW_KEY_W:
		return rW;
	case GLFW_KEY_E:
		return rE;
	case GLFW_KEY_R:
		return rR;
	case GLFW_KEY_T:
		return rT;
	case GLFW_KEY_Y:
		return rY;
	case GLFW_KEY_U:
		return rU;
	case GLFW_KEY_I:
		return rI;
	case GLFW_KEY_O:
		return rO;
	case GLFW_KEY_P:
		return rP;
	case GLFW_KEY_A:
		return rA;
	case GLFW_KEY_S:
		return rS;
	case GLFW_KEY_D:
		return rD;
	case GLFW_KEY_F:
		return rF;
	case GLFW_KEY_G:
		return rG;
	case GLFW_KEY_H:
		return rH;
	case GLFW_KEY_J:
		return rJ;
	case GLFW_KEY_K:
		return rK;
	case GLFW_KEY_L:
		return L;
	case GLFW_KEY_Z:
		return rZ;
	case GLFW_KEY_X:
		return rX;
	case GLFW_KEY_C:
		return rC;
	case GLFW_KEY_V:
		return rV;
	case GLFW_KEY_B:
		return rB;
	case GLFW_KEY_N:
		return rN;
	case GLFW_KEY_M:
		return rM;

	case GLFW_KEY_1:
		return rNUM1;
	case GLFW_KEY_2:
		return rNUM2;
	case GLFW_KEY_3:
		return rNUM3;
	case GLFW_KEY_4:
		return rNUM4;
	case GLFW_KEY_5:
		return rNUM5;
	case GLFW_KEY_6:
		return rNUM6;
	case GLFW_KEY_7:
		return rNUM7;
	case GLFW_KEY_8:
		return rNUM8;
	case GLFW_KEY_9:
		return rNUM9;
	case GLFW_KEY_0:
		return rNUM0;

	case GLFW_KEY_LEFT_SHIFT:
		return rSHIFT;
	case GLFW_KEY_LEFT_CONTROL:
		return rCTRL;
	case GLFW_KEY_LEFT_ALT:
		return rALT;
	case GLFW_KEY_SPACE:
		return rSPACE;
	case GLFW_KEY_ENTER:
		return rENTER;
	case GLFW_KEY_ESCAPE:
		return rESC;
	case GLFW_KEY_PERIOD:
		return rDOT;
	case GLFW_KEY_COMMA:
		return rCOMMA;

	case GLFW_MOUSE_BUTTON_LEFT:
		return rLMB;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return rRMB;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return rMMB;
	default:
		return false;
	}
}

void Input::clearOnRelease()
{
	rQ = false;
	rW = false;
	rE = false;
	rR = false;
	rT = false;
	rY = false;
	rU = false;
	rI = false;
	rO = false;
	rP = false;
	rA = false;
	rS = false;
	rD = false;
	rF = false;
	rG = false;
	rH = false;
	rJ = false;
	rK = false;
	rL = false;
	rZ = false;
	rX = false;
	rC = false;
	rV = false;
	rB = false;
	rN = false;
	rM = false;
	rSHIFT = false;
	rCTRL = false;
	rALT = false;
	rSPACE = false;
	rESC = false;
	rENTER = false;
	rDOT = false;
	rCOMMA = false;
	rNUM1 = false;
	rNUM2 = false;
	rNUM3 = false;
	rNUM4 = false;
	rNUM5 = false;
	rNUM6 = false;
	rNUM7 = false;
	rNUM8 = false;
	rNUM9 = false;
	rNUM0 = false;

	rLMB = false;
	rRMB = false;
	rMMB = false;

	scrollVal = 0.0;
}

double Input::getScrollValue()
{
	return scrollVal;
}