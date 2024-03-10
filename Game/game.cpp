#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define PI 3.14
#define HALF_CIRCLE 180
#define MAX_ROT HALF_CIRCLE
#define MIN_ROT 27.5/2
#define _USE_MATH_DEFINES
#include <math.h>

bool Game::mousePressed = false;

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}

Game::Game() : Scene()
{


}

// rubiks cube funcs:

RubikCube* Game::getCubesData() {
	return &cubes_data;
}

void Game::doubleRotDeg()
{
	if (this->rotDeg < MAX_ROT)
	{
		rotDeg *= 2;
	}
}

void Game::divRotDeg()
{
	if (this->rotDeg > MIN_ROT)
	{
		rotDeg /= 2;
	}
}

Game::Game(float angle, float relationWH, float near1, float far1) : Scene(angle, relationWH, near1, far1)
{
}

void Game::Init()
{

	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");

	AddTexture("../res/textures/plane.png", false); // tex-id =0

	int delta = 1;
	int cube_dim = 3;

	/*
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int j = 0; j < 3; j++) {
				AddShape(Cube, -1, TRIANGLES); // 2 is the enum of Cube
				shapes[i]->MyTranslate(glm::vec3(1 + i, 1 + j, 1 + (delta * mod9)), 0);
				shapes[i]->MyScale(glm::vec3(0.5, 0.5, 0.5));
				shapes[i]->SetTexture(0);
			}
		}
	}
	*/

	/// <summary>
	/// /
	/// </summary>
	int size = 3;
	float distance = 1;
	int index = 0;

	for (float i = -distance; i <= distance; i += 1)
	{
		for (float j = -distance; j <= distance; j += 1)
		{
			for (float k = distance; k >= -distance; k -= 1)
			{
				AddShape(Scene::Cube, -1, Scene::TRIANGLES);
				SetShapeTex(index, 0);
				shapes[index]->MyTranslate(glm::vec3(i, j, k), 0);
				shapes[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));
				index++;
			}
		}
	}
	pickedShape = 0; //irrelevant - all shapes defined are rendered......

	//SetShapeTex(0,0);
	MoveCamera(0, zTranslate, 10);
	pickedShape = -1;

	//ReadPixel(); //uncomment when you are reading from the z-buffer
}


void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/*
	#define 	GLFW_KEY_RIGHT   262
	#define 	GLFW_KEY_LEFT   263
	#define 	GLFW_KEY_DOWN   264
	#define 	GLFW_KEY_UP   265
	*/
	Game* scn = (Game*)glfwGetWindowUserPointer(window);
	int ckDir = key == GLFW_KEY_LEFT || key == GLFW_KEY_UP ? -1 : 1;
	std::vector<std::vector<int>> cubesToRotate;

	RubikCube* cube_data = scn->getCubesData();

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {

			scn->MyRotate(ckDir *10, glm::vec3(0.0, 1.0, 0.0), 0); //rotate the scene after the model transform
		}
		else if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {

			scn->MyRotate(ckDir * 20, glm::vec3(1.0, 0.0, 0.0), 0); //rotate the scene after the model transform
		}
		else if (key == GLFW_KEY_U) {
			// Rotate upper face

			if (cube_data->isAbleToRot('U'))
			{
				std::vector<std::vector<int>> cubesToRotate = cube_data->getFace('U');
				cube_data->updateFaceState(FACES::UP, scn->getRotDeg(), scn->getCkw());
				std::cout << "Locations before rotation:" << std::endl;
				cube_data->printData();
				//std::vector<Shape*> shapes = scn->getShapes();
				glm::vec3 rotateAboutVec = glm::vec3(0.0, -1.0, 0.0);
				Game::rotateFace(scn, cubesToRotate, rotateAboutVec, scn->getRotDeg(), scn->getCkw());

				//cubesToRotate = cube_data->rotateFace(cubesToRotate, true);//update the data after rotation
				//cube_data->setFace(cubesToRotate, 'U');

				std::cout << "printing updated locations after upper wall rotation:" << std::endl;
				cube_data->printData();
			}
		}
		else if (key == GLFW_KEY_D) {
			// Rotate down face
			if (cube_data->isAbleToRot('D'))
			{
				std::vector<std::vector<int>> cubesToRotate = cube_data->getFace('D');
				cube_data->updateFaceState(FACES::DOWN, scn->getRotDeg(), scn->getCkw());
				//std::vector<Shape*> shapes = scn->getShapes();
				std::cout << "Locations before rotation:" << std::endl;
				cube_data->printData();
				Game::rotateFace(scn, cubesToRotate, glm::vec3(0.0, -1.0, 0.0), scn->getRotDeg(), scn->getCkw());

				//cubesToRotate = cube_data->rotateFace(cubesToRotate, true);//update the data after rotation
				//cube_data->setFace(cubesToRotate, 'D');

				std::cout << "printing updated locations after Down wall rotation:" << std::endl;
				cube_data->printData();
			}
		}
		else if (key == GLFW_KEY_L) {
			// Rotate left face

			if (cube_data->isAbleToRot('L'))
			{
				std::cout << "Locations before rotation:" << std::endl;
				cube_data->printData();
				std::vector<std::vector<int>> cubesToRotate = cube_data->getFace('L');
				cube_data->updateFaceState(FACES::LEFT, scn->getRotDeg(), scn->getCkw());
				Game::rotateFace(scn, cubesToRotate, glm::vec3(-1.0, 0.0, 0.0), scn->getRotDeg(), scn->getCkw());

				//cubesToRotate = cube_data->rotateFace(cubesToRotate, true);//update the data after rotation
				//cube_data->setFace(cubesToRotate, 'L');
				std::cout << "printing updated locations after Left wall rotation:" << std::endl;
				cube_data->printData();
			}

		}
		else if (key == GLFW_KEY_R) {
			// Rotate right face
			std::vector<std::vector<int>> cubesToRotate = cube_data->getFace('R');

			if (cube_data->isAbleToRot('R'))
			{
				std::cout << "Locations before rotation:" << std::endl;
				cube_data->printData();
				cube_data->updateFaceState(FACES::RIGHT, scn->getRotDeg(), scn->getCkw());
				Game::rotateFace(scn, cubesToRotate, glm::vec3(1.0, 0.0, 0.0), scn->getRotDeg(), scn->getCkw());

				//cubesToRotate = cube_data->rotateFace(cubesToRotate, true);//update the data after rotation
				//cube_data->setFace(cubesToRotate, 'R');

				std::cout << "printing updated locations after Right wall rotation:" << std::endl;
				cube_data->printData();
			}
		}
		else if (key == GLFW_KEY_F) {

			if (cube_data->isAbleToRot('F'))
			{
				std::cout << "Locations before rotation:" << std::endl;
				cube_data->printData();

				// Rotate front face
				std::vector<std::vector<int>> cubesToRotate = cube_data->getFace('F');
				cube_data->updateFaceState(FACES::FRONT, scn->getRotDeg(), scn->getCkw());
				Game::rotateFace(scn, cubesToRotate, glm::vec3(0.0, 0.0, 1.0), scn->getRotDeg(), scn->getCkw());

				//cubesToRotate = cube_data->rotateFace(cubesToRotate, true);//update the data after rotation
				//cube_data->setFace(cubesToRotate, 'F');

				std::cout << "printing updated locations after Front wall rotation:" << std::endl;
				cube_data->printData();
			}

		}
		else if (key == GLFW_KEY_B) {
			// Rotate back face
			if (cube_data->isAbleToRot('B'))
			{
				std::vector<std::vector<int>> cubesToRotate = cube_data->getFace('B');
				std::cout << "Locations before rotation:" << std::endl;
				cube_data->printData();
				cube_data->updateFaceState(FACES::BACK, scn->getRotDeg(), scn->getCkw());
				Game::rotateFace(scn, cubesToRotate, glm::vec3(0.0, 0.0, -1.0), scn->getRotDeg(), scn->getCkw());

				//cubesToRotate = cube_data->rotateFace(cubesToRotate, true);//update the data after rotation
				//cube_data->setFace(cubesToRotate, 'B');

				std::cout << "printing updated locations after Back wall rotation:" << std::endl;
				cube_data->printData();
			}
		}
		else if (GLFW_KEY_SPACE == key)
		{
			scn->switchRotSide();
		}
		else if (GLFW_KEY_A==key)
		{
			scn->doubleRotDeg();
		}
		else if (GLFW_KEY_Z == key)
		{
			scn->divRotDeg();
		}
	}
}


void Game::Update(const glm::mat4& MVP, const glm::mat4& Model, const int  shaderIndx)
{
	Shader* s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Model);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.7f, 0.8f, 0.1f, 1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if (isActive)
	{
	}
}

Game::~Game(void)
{
}

void Game::mouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS)
			Game::mousePressed = true;
		else if (action == GLFW_RELEASE)
			Game::mousePressed = false;
	}
}
void Game::mousePosCallback(GLFWwindow* window, int xPos, int yPos) {

}

void Game::rotateFace(Game* scn, Face face, glm::vec3 rotVec, float deg, bool ckw)
{
	for (std::vector<int> faceLine : face) {
		for (int cubeIndexToRotate : faceLine) {
			std::cout << "Up - curr index to rotate= " << cubeIndexToRotate << std::endl;
			scn->getShapes()[cubeIndexToRotate]->MyRotate(deg, rotVec, 0);
		}
	}
}
