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
	if (intersect(level->character->box(), level->objective->boundingBox))
	{
		Level* lvl = level->getNextLevel();
		delete level;
		level = lvl;
	}
}

void Scene::processMovement(GLFWwindow* window, float deltaTime)
{
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