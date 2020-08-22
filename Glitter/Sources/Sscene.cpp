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
	loading = true;
	level->character->walk(Right, 0.0001);
	level->character->resetModel();
	level->character->resetBoxes();
	camera->Position = camera->startPos;
	Level* lvl = level->getNextLevel();
	delete level;
	level = lvl;
	startingScreen->Level1 = lvl;
	level->camera = camera;
}

void Scene::processMovement(GLFWwindow* window, float deltaTime)
{
	if (level->checkObjectiveReached())
	{
		NextLevel();
	}
	else
	{
		level->processMovement(window, deltaTime);
		if (loading)
		{
			level->character->resetModel();
			level->character->resetBoxes();
			camera->Position = camera->startPos;
			loading = false;
		}
	}

}

Scene::Scene()
{
	startingScreen = new StartScreen();
	level = startingScreen->Level1;
	level->camera = camera;
	loading = false;
}

Scene::~Scene()
{
}