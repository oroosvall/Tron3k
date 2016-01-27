#pragma once
#ifndef UI_H
#define UI_H

#include <GL\glew.h>

#include "uiVertex.h"
#include "../Console.h"
#include "Button.h"
#include "VertexBufferUI.h"

#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class UI
{
private:
	//Lists
	Button* UiObjects;
	int* textureIdList;

	//Counters
	int nrOfObjects;
	int nrOfObjectsToRender;

	int menuId;

	Console* console;

	//Render
	VertexBuffers buffers;
	GLuint uniformTextureLocation;
	GLuint uniformWorldMatrix;
	GLuint shader;


	//	Functions
	//Load from file
	bool loadUI(std::string fileName);
	//Buffers
	void newBuffers();
	void createBuffer(int id);
	void removeMenu();
	//Clean up
	//Convert
	glm::vec2 fileCoordToScreenSpace(glm::vec2 pos);
	//Events
	int collisionEvent(int UniqueKey);
public:
	UI();
	~UI();

	//Start
	void init(std::string fileName, GLuint shader, GLuint worldMat, GLuint textureLocation, Console* console);
	
	void render(std::vector<GLuint> uiTextureIds);

	void clean();

	int mouseCollission(glm::vec2 pos);
	
	void changeTex(int objId);

	void hideWindow();

	void setWorldMatrix(float x, float y, int objId);
};

#endif