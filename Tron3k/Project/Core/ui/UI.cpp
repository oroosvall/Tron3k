#include "UI.h"

UI::UI() 
{
	//Pointers
	UiObjects = nullptr;
	textureIdList = nullptr;

	//Counters
	nrOfObjects = 0;
	nrOfObjectsToRender = 0;

	//id
	menuId = -1;

	//Render
	buffers = VertexBuffers();
	uniformTextureLocation = -1;
	uniformWorldMatrix = -1;
	shader = -1;
}
UI::~UI() 
{
	clean();
}

void UI::render(std::vector<GLuint> uiTextureIds)
{
	glUseProgram(shader);

//	for (int i = 0; i < nrOfObjects; i++) 
//		UiObjects->render(uiTextureIds, i);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		printf("Error");
}

void UI::init(std::string fileName, GLuint shader, GLuint worldMat, GLuint textureLocation, Console* console)
{
	this->shader = shader;
	this->uniformWorldMatrix = worldMat;
	this->uniformTextureLocation = textureLocation;
	this->console = console;

	bool result = loadUI(fileName);
	if (!result)
		console->printMsg("Error: LoadUI in UI was unsucessfull","System",'S');

	newBuffers();
}

bool UI::loadUI(std::string fileName) 
{
	bool result = false;
	
	std::ifstream myfile;
	myfile.open(fileName);
	
	if (myfile.is_open())
	{
		//Variables for the while loop
		std::string inputString;
		int convertedResult = -1;
		float x = -1.0f, y = -1.0f, u = -1.0f, v = -1.0f;
		int textureId1 = -1, textureId2 = -1, scale = -1, counter = 0, uniqueKey = 0;
	
		//Number of objects
		getline(myfile, inputString);
		nrOfObjects = std::stoi(inputString); //Converting
	
		//Which menu
		getline(myfile, inputString);
		menuId = std::stoi(inputString);
	
		//Allocate memory
		textureIdList = new int[nrOfObjects];
	
		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			std::string temp;
			glm::vec2 xy[4];
			glm::vec2 uv[4];
			std::stringstream ss("");
	
			for (int i = 0; i < 4; i++) //stoi doesn't work with decimals
			{
				getline(myfile, inputString); //X, Y
				ss = std::stringstream(inputString);
				ss >> temp;
				x = std::stof(temp);
				ss >> temp;
				y = std::stof(temp);
	
				getline(myfile, inputString); //U, Vs
				ss = std::stringstream(inputString);
				ss >> temp;
				u = std::stoi(temp);
				ss >> temp;
				v = std::stoi(temp);
	
				xy[i] = glm::vec2(x, y);
				uv[i] = glm::vec2(u, v);
				xy[i] = fileCoordToScreenSpace(xy[i]);
			}
	
			getline(myfile, inputString); //tex1
			textureId1 = std::stoi(inputString);
			getline(myfile, inputString); //tex2
			textureId2 = std::stoi(inputString);
			getline(myfile, inputString); //uniqueKey
			uniqueKey = std::stoi(inputString);
			getline(myfile, inputString); //scale
			scale = std::stoi(inputString);
	
			UiObjects[counter] = Button(xy, uv, textureId1, textureId2, uniqueKey, shader, uniformWorldMatrix, uniformTextureLocation);
//			UiObjects[counter].scalePositions(scale);
			textureIdList[counter] = textureId1;
			result = true;
			counter++;
		}
	}
	else
		console->printMsg("Error: File could not be opened", "System", 'S');
	
	myfile.close();
	
	return result;
}

void UI::clean()
{
	if (UiObjects != nullptr)
		delete[] UiObjects;
	if (textureIdList != nullptr)
		delete[] textureIdList;

	UiObjects = nullptr;
	textureIdList = nullptr;
	console = nullptr;

	nrOfObjects = 0;
	nrOfObjectsToRender = 0;
	menuId = 0;

	buffers.clean();
}

glm::vec2 UI::fileCoordToScreenSpace(glm::vec2 pos)
{
	glm::vec2 newVec2;

	newVec2.x = pos.x * 2 - 1;
	newVec2.y = pos.y * 2 - 1;

	return newVec2;
}


int UI::mouseCollission(glm::vec2 pos) 
{
	int hit = -1;
	int result = -1;
	
	for (int i = 0; i < nrOfObjects && hit != -1; i++)
	{
		hit = UiObjects[i].checkCollision(pos);
	}
	
	if (hit != -1)
		result = collisionEvent(hit);
	
	return result;
}

int UI::collisionEvent(int UniqueKey) //Every button in every menu have a unique key
{
	int result = -1;

	//Texture change is suppose to be changed to tex 1
	switch (UniqueKey)
	{
	case(4) : //IP
		//Här ska det sättas så man kan skriva in en IP
		break;
	//case(0) : //Roam 0
	//case(1) : //Play 1
	//case(2) : //Settings 2
	//case(3) : //Exit 3
	//case(5) : //Connect 5
	//case(6) : //Back 6
	default: //Everthing that needs to go back to UIManager is defaulted with returning their unique key.
		result = UniqueKey;
		break;
	}

	return result;
}


void UI::changeTex(int objId)
{
	UiObjects[objId].changeTexUsed();
}

//Empty
void hideWindow()
{

}

void UI::setWorldMatrix(float x, float y, int objId)
{
//	UiObjects[objId].setWorldMatrix(x, y);
}


void UI::newBuffers()
{
	//Set the defualt values.
	buffers = VertexBuffers(nrOfObjects);
	for (int i = 0; i < nrOfObjects; i++)
	{
		//Sets the newly created buffers values
		createBuffer(i);

		//Set the list of texture ids
		int tmp = textureIdList[i];
		buffers.textureIDs[i] = tmp;
	}
}
void UI::createBuffer(int id)
{
	//create buffer and set data
	glGenBuffers(1, &buffers.gVertexBuffer[id]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.gVertexBuffer[id]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_ui) * 4, &UiObjects[id].returnPosAUv()[0], GL_STATIC_DRAW); // &tester[id].posList[0] bytt ut mot UIs buttons Vertex

	//define vertex data layout
	glGenVertexArrays(1, &buffers.gVertexAttribute[id]);
	glBindVertexArray(buffers.gVertexAttribute[id]);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	glEnableVertexAttribArray(1);

	GLint vertexPos = glGetAttribLocation(shader, "vertex_position");
	glVertexAttribPointer(vertexPos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_ui), BUFFER_OFFSET(0));
	GLint vertexUV = glGetAttribLocation(shader, "vertex_uv");
	glVertexAttribPointer(vertexUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_ui), BUFFER_OFFSET(sizeof(float) * 2));

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		printf("Error");
}
//Empty
void UI::removeMenu() 
{

}