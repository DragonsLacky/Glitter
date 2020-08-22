#ifndef GAME_SCENE
#define GAME_SCENE

#include "MainCharacter.hpp"
#include "Cube.hpp"
#include "Level.hpp"
#include "Camera.hpp"

#include <iostream>
#include <vector>

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();
	StartScreen* startingScreen;
	Level* level;
	Camera* camera;
	bool loading;
	//glm::mat4 model;
	void processMovement(GLFWwindow* window, float deltaTime);
	void NextLevel();
	void Draw();
};


#endif