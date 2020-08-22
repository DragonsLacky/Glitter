#include "Scene.hpp"

void Scene::Draw()
{
	if (startingScreen->MouseEnabled)
	{
		startingScreen->Draw();
	}
	else
	{
		level->Draw();
	}
}


void Scene::NextLevel()
{
	Level* lvl = level->getNextLevel();
	delete level;
	level = lvl;
	startingScreen->Level1 = lvl;
	level->character->walk(Right, 0.01);
	level->character->resetModel();
	level->character->resetBoxes();
}

void Scene::processMovement(GLFWwindow* window, float deltaTime)
{
	if (level->checkObjectiveReached())
	{
		NextLevel();
	}
	level->processMovement(window, deltaTime);
}

Scene::Scene()
{
	startingScreen = new StartScreen();
	level = startingScreen->Level1;
}

Scene::~Scene()
{
}