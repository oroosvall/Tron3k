#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class TextObject
{
private:
	int size;
	vec2 pos;
	vector<vec2> vertices;
	string text;
	int rightLimit;
	int bottomLimit;
	void setupText(string str, vec2 screenPos);
public:
	TextObject(string str, int fontSize, vec2 screenPos);
	TextObject(TextObject* obj, vec2 screenPos = vec2(-1, -1));
	~TextObject();
	void getBuffer(int &fSize, vector<vec2> &dataBuf);
	void setText(string str);
	void getTextLimits(int &right, int &bottom)
	{
		right = rightLimit;
		bottom = bottomLimit;
	}

	static int ScreenResWidth;
	static int ScreenResHeight;

	string getText()
	{
		return text;
	}
};

#endif