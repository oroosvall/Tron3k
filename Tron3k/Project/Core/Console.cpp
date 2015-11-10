#include "Console.h"

Console::Console()
{
	for (int c = 0; c < MAXHISTORY; c++)
	{
		history[c] = "";
	}
	printConsole();
}

Console::~Console()
{

}

void Console::update()
{
	Input* i = Input::getInput();

	for (int c = 0; c < VALIDKEYS; c++)
	{
		if (i->justReleased(validKeyboardInputs[c]))
		{
			char ch = i->keyToChar(validKeyboardInputs[c]);
			
			if (!i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
			{
				ch = tolower(ch);
			}
			
			msg += ch;

			printf("%c", ch);
		}
	}
	if (i->justReleased(GLFW_KEY_BACKSPACE))
	{
		if (msg.size() > 0)
		{
			msg = msg.substr(0, msg.size() - 1);
			printConsole();
		}
	}
	if (i->justReleased(GLFW_KEY_ENTER))
	{
		latestMsg++;
		latestMsg = latestMsg%MAXHISTORY;

		history[latestMsg] = msg;
		msg = "";
		printConsole();
	}
}
#include <Windows.h>
void Console::printConsole()
{
	system("cls");
	int counter = (latestMsg+1)%MAXHISTORY;
	for (int c = 0; c < MAXHISTORY; c++)
	{
		printf("%s\n", history[counter].c_str());
		counter++;
		counter = counter%MAXHISTORY;
	}
	printf("********************************\n");
	printf("%s", msg.c_str());

}