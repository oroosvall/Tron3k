#include "Console.h"

Console::Console()
{
	for (int c = 0; c < MAXHISTORY; c++)
	{
		history[c] = "";
	}
	cmdReady = false;
	printConsole();
}

Console::~Console()
{

}

bool Console::update(string clientName)
{
	Input* i = Input::getInput();
	if (inChatMode)
	{
		for (int c = 0; c < VALIDKEYS; c++)
		{
			if (i->justPressed(validKeyboardInputs[c]))
			{
				char ch = i->keyToChar(validKeyboardInputs[c]);

				if (!i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
				{
					ch = tolower(ch);
				}
				else
				{
					if (ch == '1')
						ch = '!';
					if (ch == '2')
						ch = '\"';
					if (ch == '3')
						ch = '#';
					if (ch == '4')
						ch = '$';
					if (ch == '5')
						ch = '%';
					if (ch == '6')
						ch = '&';
					if (ch == '7')
						ch = '/';
					if (ch == '8')
						ch = '(';
					if (ch == '9')
						ch = ')';
					if (ch == '0')
						ch = '=';
					if (ch == '-')
						ch = '_';
					if (ch == '\'')
						ch = '*';
					if (ch == '+')
						ch = '?';
					if (ch == '.')
						ch = ':';
					if (ch == ',')
						ch = ';';
				}

				msg += ch;

				printf("%c", ch);
			}
		}
		if (i->justPressed(GLFW_KEY_BACKSPACE))
		{
			if (msg.size() > 0)
			{
				msg = msg.substr(0, msg.size() - 1);
				printConsole();
			}
		}
		if (i->justPressed(GLFW_KEY_ENTER))
		{
			if (msg[0] == '/')
			{
				cmd = msg;
				cmdReady = true;
				addMsg(msg, clientName, scope);
			}
			else
			{
				if (msg.size() > 0)
				{
					lastMsg = msg;
					msgReady = true;
					addMsg(msg, clientName, scope);
				}
				else
					scope = 'T';
			}
			inChatMode = false;
		}
	}
	else
	{
		if (i->justPressed(teamchat))
		{
			inChatMode = true;
			scope = 'T';
		}
		if (i->justPressed(allchat))
		{
			inChatMode = true;
			scope = 'A';
		}
	}

	return cmdReady;
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

void Console::addMsg(string &m, string n, char s)
{
	latestMsg++;
	latestMsg = latestMsg%MAXHISTORY;

	stringstream ss;
	ss << n;
	ss << "[";
	ss << s;
	ss << "] : ";
	ss << m;

	string newmsg = ss.str();

	//m = n + "[" + s + "] : " + m;

	history[latestMsg] = newmsg;

	m = "";
	historyUpdate = true;
	printConsole();
}

void Console::printMsg(string m, string n, char s)
{
	addMsg(m, n, s);
}

bool Console::commandReady()
{
	return cmdReady;
}

string Console::getCommand()
{
	return cmd;
}

bool Console::getHistoryUpdate()
{
	return historyUpdate;
}

string Console::getHistory()
{
	string result = "";

	int iter = latestMsg-4;
	if (iter < 0)
		iter = MAXHISTORY + iter;
	for (size_t i = 0; i < 5; i++)
	{
		result += history[iter] + "\n";

		iter++;
		iter = iter%MAXHISTORY;
	}
	historyUpdate = false;
	return result;
}

void Console::discardCommandAndLastMsg()
{
	cmd = "";
	cmdReady = false;

	lastMsg = "";
	msgReady = false;
}

bool Console::messageReady()
{
	return msgReady;
}

string Console::getMessage(char &s)
{
	s = scope;
	scope = 'T';
	return lastMsg;
}