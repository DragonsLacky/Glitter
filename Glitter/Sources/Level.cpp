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
	if (intersects && character->finishedJump && character->pressedBeforeJump)
	{
		character->finishedJump = true;
		character->pressedBeforeJump = false;
	}
}

bool Level::checkObjectiveReached()
{
	return intersect(character->box(), objective->boundingBox);
}

void Level::Draw()
{
	Shader& shader = ResourceManager::GetShader("shader");
	Shader& shader1 = ResourceManager::GetShader("model");

	shader.use();
	shader.setBool("gamma", false);
	shader.setBool("light", false);
	for (int i = 0; i < lights.size(); i++)
	{
		string l = "Lights[" + to_string(i);
		shader.setInt(l + "].type", lights[i]->type);
		shader.setVec3(l + "].ambient", lights[i]->ambient);
		shader.setVec3(l + "].diffuse", lights[i]->diffuse);
		shader.setVec3(l + "].specular", lights[i]->specular);
		shader.setVec3(l + "].color", lights[i]->color);

		if (lights[i]->type == 0)
		{
			DirectionalLight* light = dynamic_cast<DirectionalLight*>(lights[i]);
			if (light != nullptr)
			{
				shader.setVec3(l + "].direction", light->direction);
				shader1.setFloat(l + "].constant", 1);
				shader1.setFloat(l + "].linear", 0.0014);
				shader1.setFloat(l + "].quadratic", 0.000007);
			}
		}
		else if (lights[i]->type == 1)
		{
			PointLight* light = dynamic_cast<PointLight*>(lights[i]);
			if (light != nullptr)
			{
				shader.setVec3(l + "].position", light->position);
				shader.setFloat(l + "].constant", light->constant);
				shader.setFloat(l + "].linear", light->linear);
				shader.setFloat(l + "].quadratic", light->quadratic);

			}
		}
		else if (lights[i]->type == 2)
		{
			SpotLight* light = dynamic_cast<SpotLight*>(lights[i]);
			if (light != nullptr)
			{
				shader.setVec3(l + "].position", light->position);
				shader.setVec3(l + "].direction", light->direction);
				shader.setFloat(l + "].constant", light->constant);
				shader.setFloat(l + "].linear", light->linear);
				shader.setFloat(l + "].quadratic", light->quadratic);
				shader.setFloat(l + "].cutOff", light->cutOff); 
				shader.setFloat(l + "].outerCutOff", light->outerCutOff);
			}
		}
	}
	shader.setInt("size", lights.size());
	for each (Cube cube in cubes)
	{
		cube.Draw();
	}
	for each (Cube cube in DestCubes)
	{
		cube.Draw();
	}

	shader1.use();
	shader1.setBool("gamma", false);
	for (int i = 0; i < lights.size(); i++)
	{
		string l = "Lights[" + to_string(i);
		shader1.setInt(l + "].type", lights[i]->type);
		shader1.setVec3(l + "].ambient", lights[i]->ambient);
		shader1.setVec3(l + "].diffuse", lights[i]->diffuse);
		shader1.setVec3(l + "].specular", lights[i]->specular);
		shader1.setVec3(l + "].color", lights[i]->color);
		//cout << lights[i]->ambient.x << " " << lights[i]->ambient.y << " " << lights[i]->ambient.z <<"\n" ;
		if (lights[i]->type == 0)
		{
			DirectionalLight* light = dynamic_cast<DirectionalLight*>(lights[i]);
			if (light != nullptr)
			{
				shader1.setVec3(l + "].direction", light->direction);
				shader1.setFloat(l + "].constant", 1);
				shader1.setFloat(l + "].linear", 0.0014);
				shader1.setFloat(l + "].quadratic", 0.000007);
			}
		}
		else if (lights[i]->type == 1)
		{
			PointLight* light = dynamic_cast<PointLight*>(lights[i]);
			if (light != nullptr)
			{
				shader1.setVec3(l + "].position", light->position);
				shader1.setFloat(l + "].constant", light->constant);
				shader1.setFloat(l + "].linear", light->linear);
				shader1.setFloat(l + "].quadratic", light->quadratic);

			}
		}
		else if (lights[i]->type == 2)
		{
			SpotLight* light = dynamic_cast<SpotLight*>(lights[i]);
			if (light != nullptr)
			{
				shader1.setVec3(l + "].position", light->position);
				shader1.setVec3(l + "].direction", light->direction);
				shader1.setFloat(l + "].constant", light->constant);
				shader1.setFloat(l + "].linear", light->linear);
				shader1.setFloat(l + "].quadratic", light->quadratic);
				shader1.setFloat(l + "].cutOff", light->cutOff);
				shader1.setFloat(l + "].outerCutOff", light->outerCutOff);
			}
		}
	}
	shader1.setInt("size", lights.size());
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

	Shader lightShader = ResourceManager::GetShader("shader");
	lightShader.use();
	lightShader.setBool("light", true);
	for (int i = 0; i < lights.size(); i++)
	{
		lightShader.use();
		glm::mat4 model(1.0f);
		if (lights[i]->type == 1)
		{
			PointLight* light = dynamic_cast<PointLight*>(lights[i]);
			if (light != nullptr)
			{
				model = glm::translate(model, light->position);
			}
			lightShader.setMat4("model", model);
			lightCube.Draw();
		}
	}

	skybox.Draw();
}

void Level::DrawShadowMap()
{
	Shader& shader = ResourceManager::GetShader("simpleDepth");
	//Shader& shader1 = ResourceManager::GetShader("model");
	shader.use();
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

	for (int i = 0; i < lights.size(); i++)
	{
		shader.use();
		glm::mat4 model(1.0f);
		if (lights[i]->type == 1)
		{
			PointLight* light = dynamic_cast<PointLight*>(lights[i]);
			if (light != nullptr)
			{
				model = glm::translate(model, light->position);
			}
			shader.setMat4("model", model);
			lightCube.Draw();
		}
	}
}

StartScreen::StartScreen()
{
	MouseEnabled = true;
	Level1 = new Level_1();
	StartButton = Cube(glm::vec3(-0.5f, 0.8f, 0.0f), glm::vec3(1.0f, 0.2f, 0.0f), "../Glitter/Resources/Textures/", "StartButton", ".png");
	LevelsButton = Cube(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.2f, 0.0f), "../Glitter/Resources/Textures/", "LevelSelsectButton", ".png");
	ExitButton = Cube(glm::vec3(-0.5f, 0.2f, 0.0f), glm::vec3(1.0f, 0.2f, 0.0f), "../Glitter/Resources/Textures/", "ExitButton", ".png");

	Shader& shader = ResourceManager::GetShader("screen");
	shader.use();
	shader.setVec2("tlMenuCoords[0]", glm::vec2(StartButton.boundingBox.min.x, StartButton.boundingBox.max.y));
	shader.setVec2("brMenuCoords[0]", glm::vec2(StartButton.boundingBox.max.x, StartButton.boundingBox.min.y));
	shader.setVec2("tlMenuCoords[1]", glm::vec2(LevelsButton.boundingBox.min.x, LevelsButton.boundingBox.max.y));
	shader.setVec2("brMenuCoords[1]", glm::vec2(LevelsButton.boundingBox.max.x, LevelsButton.boundingBox.min.y));
	shader.setVec2("tlMenuCoords[2]", glm::vec2(ExitButton.boundingBox.min.x, ExitButton.boundingBox.max.y));
	shader.setVec2("brMenuCoords[2]", glm::vec2(ExitButton.boundingBox.max.x, ExitButton.boundingBox.min.y));
	shader.setInt("size", 4);

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
	lightCube = Cube(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.2f, 0.2f, 0.2f), "../Glitter/Resources/Textures/", "light", ".jpg");
}

Level::Level(MainCharacter* mc)
{
	character = mc;
	lightCube = Cube(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.2f, 0.2f, 0.2f), "../Glitter/Resources/Textures/", "light", ".jpg");
}


Level::~Level()
{
	//delete character;
	delete objective;
	for each (Light* light in lights)
	{
		delete light;
	}
}

Level* Level_1::getNextLevel()
{
	return new Level_2(character);
}


Level_1::Level_1()
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 1.0f, 0.0f), 1.0, 0.022, 0.0019));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 3.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(5.0f, 1.0f, -2.0f), glm::vec3(0.1f, 1.0f, 3.0f), "", "", "", false);
}
Level_1::Level_1(MainCharacter* mc) : Level(mc)
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 1.0f, 0.0f), 1.0, 0.022, 0.0019));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 3.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	
	objective = new Cube(glm::vec3(5.0f, 1.0f, -2.0f), glm::vec3(0.1f, 1.0f, 3.0f), "", "", "", false);
}

Level_1::~Level_1()
{
}

Level* Level_2::getNextLevel()
{
	return new Level_1(character);
}

Level_2::Level_2()
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	//cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 1.0f, 0.0f), 1.0, 0.022, 0.0019));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 3.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	
	objective = new Cube(glm::vec3(5.0f, 1.0f, -2.0f), glm::vec3(0.1f, 1.0f, 3.0f), "", "", "", false);
}

Level_2::Level_2(MainCharacter* mc) : Level(mc)
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	//cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 1.0f, 0.0f), 1.0, 0.022, 0.0019));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 3.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	
	objective = new Cube(glm::vec3(5.0f, 1.0f, -2.0f), glm::vec3(0.1f, 1.0f, 3.0f), "", "", "", false);
}

Level_2::~Level_2()
{
}

