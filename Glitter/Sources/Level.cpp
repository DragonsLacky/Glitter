#include "Level.hpp"

void Level::processMovement(GLFWwindow* window, float deltaTime)
{
	float movement = deltaTime * 2.5f;
	CollisionBellow();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (!CollisionLeftSide())
		{
			character->walk(Left, movement);
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (!CollisionRightSide())
		{
			character->walk(Right, movement);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || character->pressedBeforeJump || !character->finishedJump)
	{
		if (!character->pressedBeforeJump) {
			character->finishedJump = false;
			character->pressedBeforeJump = true;
		}
		else if (character->numberOfJumpFrames == 600)
		{
			character->numberOfJumpFrames = 0;
			character->pressedBeforeJump = false;
		}
		else if (character->finishedJump) {
			if (character->numberOfJumpFrames % 3 == 0) {

			}
			character->numberOfJumpFrames++;
		}
		else if (character->numJumps == 600 && !character->finishedJump)
		{
			character->numJumps = 0;
			character->finishedJump = true;
		}
		else {
			if (character->numJumps % 3 == 0) {
				if (!CollisionAbove())
				{
					character->jump();
				}
			}
			character->numJumps++;
		}
		
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || character->pressedBeforeCrouch) {
		character->crouchHold();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && character->pressedBeforeCrouch) {
		if (!CollisionAboveCrouch())
		{
			character->crouchRelease();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		AABB box = character->attack();
		for (int i = 0; i < DestCubes.size(); i++)
		{
			if (intersect(box, DestCubes[i].boundingBox))
			{
				DestCubes.erase(DestCubes.begin() + i);
				i--;
			}
		}
	}
}

bool Level::Collision(AABB& box)
{
	for each (Cube cube in cubes)
	{
		if (intersect(box, cube.boundingBox))
		{
			return true;
		}
	}

	for each (Cube cube in DestCubes)
	{
		if (intersect(box, cube.boundingBox))
		{
			return true;
		}
	}	
	
	return false;

}

bool Level::CollisionRightSide()
{
	AABB box = character->box();
	box.min.y = box.min.y + 0.1;
	box.max.y = box.max.y - 0.1;
	box.min.x = box.min.x + 0.1;

	return Collision(box);
}


bool Level::CollisionLeftSide()
{
	AABB box = character->box();
	box.min.y = box.min.y + 0.1;
	box.max.y = box.max.y - 0.1;
	box.max.x = box.max.x - 0.1;

	return Collision(box);
}


bool Level::CollisionAbove()
{
	AABB box = character->box();
	box.min.y = box.min.y + 0.1;
	box.min.x = box.min.x + 0.1;
	box.max.x = box.max.x - 0.1;
	
	return Collision(box);
}

bool Level::CollisionAboveCrouch()
{
	if (character->pressedBeforeCrouch)
	{
		int ms = character->modelState;
		character->modelState = 0;
		AABB box = character->box();
		box.min.y = box.min.y + 0.1;
		box.min.x = box.min.x + 0.1;
		box.max.x = box.max.x - 0.1;
		bool intersects = Collision(box);			
		if (intersects)
		{
			character->modelState = ms;
			return true;
		}
		else
		{
			character->modelState = ms;
			return false;
		}
	}
	else
	{
		return false;
	}

}

void Level::CollisionBellow()
{
	AABB box = character->box();
	box.max.y = box.max.y - 0.1;
	box.min.x = box.min.x + 0.1;
	box.max.x = box.max.x - 0.1;
	bool intersects = Collision(box);
	if (!intersects && character->finishedJump)
	{
		character->gravity();
	}
}

void Level::Draw()
{
	ResourceManager::GetShader("shader").use();
	for each (Cube cube in cubes)
	{
		cube.Draw();
	}
	for each (Cube cube in DestCubes)
	{
		cube.Draw();
	}

	/*for each (Model obj in DestructableObjects)
	{
		obj.Draw(modelShader);
	}
	for each (Model obj in objects)
	{
		obj.Draw(modelShader);
	}
	for each (Model enemy in enemies)
	{
		enemy.Draw(modelShader);
	}*/
	//AABB box = DestCubes[0].boundingBox;
	//Cube* cube = new Cube(glm::vec3(box.min.x, box.max.y, box.min.z), glm::vec3(box.max.x - box.min.x, -(box.min.y - box.max.y), box.max.z - box.min.z), "Resources/Textures/DirtBlock", "DirtBlock");

	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	cube->Draw(cubeShader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
	//delete cube;

	character->Draw();

	skybox.Draw();
}

StartScreen::StartScreen()
{
	MouseEnabled = true;
	Level1 = new Level_1();
	StartButton = Cube(glm::vec3(-0.5f, 0.8f, 0.0f), glm::vec3(1.0f, 0.2f, 0.0f), "../Glitter/Resources/Textures/", "StartButton.png");
	LevelsButton = Cube(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.2f, 0.0f), "../Glitter/Resources/Textures/", "LevelSelsectButton.png");
	ExitButton = Cube(glm::vec3(-0.5f, 0.2f, 0.0f), glm::vec3(1.0f, 0.2f, 0.0f), "../Glitter/Resources/Textures/", "ExitButton.png");

};

void StartScreen::processMovement(GLFWwindow* window, float deltaTime)
{

}

Level* StartScreen::getNextLevel()
{
	return Level1;
}

void StartScreen::Draw()
{
	Shader& shader = ResourceManager::GetShader("screen");
	shader.setBool("blur", true);
	Level1->Draw();
	shader.setBool("blur", false);
	glDisable(GL_DEPTH_TEST);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Shader& shader1 = ResourceManager::GetShader("basic");
	shader1.use();
	StartButton.Draw();
	LevelsButton.Draw();
	ExitButton.Draw();

	glEnable(GL_DEPTH_TEST);
}

StartScreen::~StartScreen()
{
	delete Level1;
};

Level::Level()
{
	character = new MainCharacter();
}

Level::~Level()
{
	delete character;
}

Level* Level_1::getNextLevel()
{
	return new Level_2();
}


Level_1::Level_1()
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass.jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt.jpg"));

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass.jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt.jpg"));

	cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass.jpg"));
	cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt.jpg"));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 3.0f)), "../Glitter/Resources/Textures/", "container2.png"));
}

Level_1::~Level_1()
{
}

Level* Level_2::getNextLevel()
{
	return new Level_1();
}

Level_2::Level_2()
{

}

Level_2::~Level_2()
{
}

