#pragma once
#include "scene.h"
#include "display.h"
#include "RubikCube.h"
//class RubikCube;
//class GLFWwindow;
class Game : public Scene
{
public:

	Game();
	Game(float angle, float relationWH, float near, float far);
	void Init();
	void Update(const glm::mat4& MVP, const glm::mat4& Model, const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
	std::vector<Shape*> getShapes() { return this->shapes; };
	//ADDED ASS3
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static bool mousePressed;
	static void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);
	static void mousePosCallback(GLFWwindow* window, int xPos, int yPos);
	static void rotateFace(Game* scn, Face face, glm::vec3 rotVec, float deg, bool ckw);
	RubikCube* getCubesData();
	bool getCkw() { return ckw; }
	double getRotDeg() { return rotDeg*(ckw?1:-1); }
	void switchRotSide() { ckw = !ckw; }
	void doubleRotDeg();
	void divRotDeg();
private:
	RubikCube cubes_data;
	bool ckw = true;
	double rotDeg = 90.0;
};

