#include "Level.hpp"

void Level::processMovement(GLFWwindow* window, float deltaTime)
{
	float movement = deltaTime * 3.0f;
	CollisionBellow();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (!CollisionLeftSide())
		{
			character->walk(Left, movement);
			camera->ProcessKeyboard(LEFT, deltaTime);
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (!CollisionRightSide())
		{
			character->walk(Right, movement);
			camera->ProcessKeyboard(RIGHT, deltaTime);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || character->pressedBeforeJump || !character->finishedJump)
	{
		if (!character->pressedBeforeJump) {
			character->finishedJump = false;
			character->pressedBeforeJump = true;
		}
		else if (character->numberOfJumpFrames == 200)
		{
			character->numberOfJumpFrames = 0;
			character->pressedBeforeJump = false;
		}
		else if (character->finishedJump) {
			if (character->numberOfJumpFrames % 2 == 0) {

			}
			character->numberOfJumpFrames++;
		}
		else if (character->numJumps == 200 && !character->finishedJump)
		{
			character->numJumps = 0;
			character->finishedJump = true;
		}
		else {
			if (character->numJumps % 2 == 0) {
				if (!CollisionAbove())
				{
					character->jump();
					camera->ProcessKeyboard(UP, deltaTime);
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
		character->attacked = true;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE && character->attacked)
	{
		character->modelState = 0;
		character->attacked = false;
	}
	PointLight* light =  dynamic_cast<PointLight*>(lights[0]);
	if (light != nullptr)
	{
		light->position.x = character->box().max.x;
		light->position.y = 15.0f;
	}
}

bool Level::Collision(AABB& box,int ignore)
{
	for each (Cube cube in cubes)
	{
		if (intersect(box, cube.boundingBox))
		{
			return true;
		}
	}

	for(unsigned short int i = 0; i < DestCubes.size(); i++)
	{
		if (i != ignore && intersect(box, DestCubes[i].boundingBox))
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
		camera->ProcessKeyboard(DOWN, 1);
		box = character->box();
		box.max.y = box.max.y - 0.1;
		box.min.x = box.min.x + 0.1;
		box.max.x = box.max.x - 0.1;
		intersects = Collision(box);
		if (!intersects && character->finishedJump)
		{
			character->gravity();
			camera->ProcessKeyboard(DOWN, 1);
		}
	}
	if (intersects && character->finishedJump && character->pressedBeforeJump)
	{
		character->finishedJump = true;
		character->pressedBeforeJump = false;
	}
	for(unsigned short int i = 0; i < DestCubes.size(); i++)
	{
		box = DestCubes[i].boundingBox;
		box.max.y = box.max.y - 0.1;
		box.min.z = box.min.z + 0.1;
		box.max.z = box.max.z - 0.1;
		intersects = Collision(box, i);
		if (!intersects)
		{
			DestCubes[i].updateModel(glm::vec3(0.0f, -0.0095995f, 0.0f));
			DestCubes[i].updateBox(glm::vec3(0.0f, -0.0095995f, 0.0f));
			box = character->box();
			box = DestCubes[i].boundingBox;
			box.max.y = box.max.y - 0.1;
			box.min.z = box.min.z + 0.1;
			box.max.z = box.max.z - 0.1;
			intersects = Collision(box, i);
			if (!intersects)
			{
				DestCubes[i].updateModel(glm::vec3(0.0f, -0.0095995f, 0.0f));
				DestCubes[i].updateBox(glm::vec3(0.0f, -0.0095995f, 0.0f));
			}
		}
	}
}

bool Level::checkObjectiveReached()
{
	return intersect(character->box(), objective->boundingBox);
}

void Level::Draw()
{
	Shader& shader = ResourceManager::GetShader("shader");
	Shader& modelShader = ResourceManager::GetShader("model");

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
				shader.setFloat(l + "].constant", 1);
				shader.setFloat(l + "].linear", 0.0014);
				shader.setFloat(l + "].quadratic", 0.000007);
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for each (Cube cube in cubes)
	{
		shader.setMat4("model", cube.model);
		cube.Draw();
	}
	for each (Cube cube in DestCubes)
	{
		shader.setMat4("model", cube.model);
		cube.Draw();
	}
	glDisable(GL_CULL_FACE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	for each (Cube cube in interiors)
	{
		shader.setMat4("model", cube.model);
		cube.Draw();
	}
	glDisable(GL_CULL_FACE);
	
	modelShader.use();
	modelShader.setBool("gamma", false);
	for (int i = 0; i < lights.size(); i++)
	{
		string l = "Lights[" + to_string(i);
		modelShader.setInt(l + "].type", lights[i]->type);
		modelShader.setVec3(l + "].ambient", lights[i]->ambient);
		modelShader.setVec3(l + "].diffuse", lights[i]->diffuse);
		modelShader.setVec3(l + "].specular", lights[i]->specular);
		modelShader.setVec3(l + "].color", lights[i]->color);

		if (lights[i]->type == 0)
		{
			DirectionalLight* light = dynamic_cast<DirectionalLight*>(lights[i]);
			if (light != nullptr)
			{
				modelShader.setVec3(l + "].direction", light->direction);
				modelShader.setFloat(l + "].constant", 1);
				modelShader.setFloat(l + "].linear", 0.0014);
				modelShader.setFloat(l + "].quadratic", 0.000007);
			}
		}
		else if (lights[i]->type == 1)
		{
			PointLight* light = dynamic_cast<PointLight*>(lights[i]);
			if (light != nullptr)
			{
				modelShader.setVec3(l + "].position", light->position);
				modelShader.setFloat(l + "].constant", light->constant);
				modelShader.setFloat(l + "].linear", light->linear);
				modelShader.setFloat(l + "].quadratic", light->quadratic);

			}
		}
		else if (lights[i]->type == 2)
		{
			SpotLight* light = dynamic_cast<SpotLight*>(lights[i]);
			if (light != nullptr)
			{
				modelShader.setVec3(l + "].position", light->position);
				modelShader.setVec3(l + "].direction", light->direction);
				modelShader.setFloat(l + "].constant", light->constant);
				modelShader.setFloat(l + "].linear", light->linear);
				modelShader.setFloat(l + "].quadratic", light->quadratic);
				modelShader.setFloat(l + "].cutOff", light->cutOff);
				modelShader.setFloat(l + "].outerCutOff", light->outerCutOff);
			}
		}
	}
	modelShader.setInt("size", lights.size());
	/*for each (Model obj in DestructableObjects)
	{
		obj.Draw(modelShader);
	}*/
	for each (Model obj in objects)
	{
		obj.Draw(modelShader);
	}
	/*for each (Model enemy in enemies)
	{
		enemy.Draw(modelShader);
	}*/

	character->Draw();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
			lightCube->Draw();
		}
	}
	glDisable(GL_CULL_FACE);
	skybox.Draw();
}

void Level::DrawShadowMap()
{
	Shader& shader = ResourceManager::GetShader("simpleDepth");

	//Shader& shader1 = ResourceManager::GetShader("model");
	shader.use();
	for each (Cube cube in cubes)
	{
		shader.setMat4("model", cube.model);
		cube.Draw();
	}
	for each (Cube cube in DestCubes)
	{
		shader.setMat4("model", cube.model);
		cube.Draw();
	}
	/*for each (Model obj in DestructableObjects)
	{
		obj.Draw(modelShader);
	}*/
	for each (Model obj in objects)
	{
		obj.Draw(shader);
	}

	character->Draw();

}

StartScreen::StartScreen()
{
	MouseEnabled = true;
	Level1 = new Level_3();
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
	lightCube = new Cube(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.2f, 0.2f, 0.2f), "../Glitter/Resources/Textures/", "light", ".jpg");
}

Level::Level(MainCharacter* mc)
{
	character = mc;
	lightCube = new Cube(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.2f, 0.2f, 0.2f), "../Glitter/Resources/Textures/", "light", ".jpg");
}


Level::~Level()
{
	//delete character;
	delete objective;
	delete lightCube;
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

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(4.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(4.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(9.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(9.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(12.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(12.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	interiors.push_back(Cube(glm::vec3(12.0f, 2.51f, -2.0f), glm::vec3(3.0f, 2.5f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));
	//interiors.push_back(Cube(glm::vec3(12.0f, 1.0f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));
	
	objects.push_back(Model("../Glitter/Resources/Objects/Sign-controls/sign.obj"));
	objects[0].model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, -1, 0));
	objects[0].model = glm::translate(objects[0].model, glm::vec3(-1.0f, -0.5f, -1.5f));
	objects[0].model = glm::scale(objects[0].model, glm::vec3(3.0f,3.0f,3.0f));
	objects.push_back(Model("../Glitter/Resources/Objects/framed_photo/framed_photo.obj"));
	objects[1].model = glm::translate(glm::mat4(1.0f), glm::vec3(13.5f, 1.5f, -1.99f));


	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 3.0f, 0.0f), 1.0, 0.022, 0.0019));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(13.5f, 2.51f, -0.5f), 1.0, 0.014, 0.0007));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(14.0f, 1.5f, -2.0f), glm::vec3(1.0f, 1.5f, 3.0f), "", "", "", "",false);
}
Level_1::Level_1(MainCharacter* mc) : Level(mc)
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.0f, 0.7f, -2.0f), glm::vec3(4.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.0f, 0.2f, -2.0f), glm::vec3(4.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(3.5f, 4.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(3.5f, 3.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(9.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(9.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(12.0f, 0.0f, -2.0f), glm::vec3(3.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(12.0f, -0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	interiors.push_back(Cube(glm::vec3(12.0f, 2.51f, -2.0f), glm::vec3(3.0f, 2.5f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));
	//interiors.push_back(Cube(glm::vec3(12.0f, 1.0f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	objects.push_back(Model("../Glitter/Resources/Objects/Sign-controls/sign.obj"));
	objects[0].model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, -1, 0));
	objects[0].model = glm::translate(objects[0].model, glm::vec3(-1.0f, -0.5f, -1.5f));
	objects[0].model = glm::scale(objects[0].model, glm::vec3(3.0f, 3.0f, 3.0f));
	objects.push_back(Model("../Glitter/Resources/Objects/framed_photo/framed_photo.obj"));
	objects[1].model = glm::translate(glm::mat4(1.0f), glm::vec3(13.5f, 1.5f, -1.99f));


	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 3.0f, 0.0f), 1.0, 0.022, 0.0019));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(13.5f, 2.51f, -0.5f), 1.0, 0.014, 0.0007));

	skybox = CubeMap("../Glitter/Resources/Textures/skybox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(14.0f, 1.5f, -2.0f), glm::vec3(1.0f, 1.5f, 3.0f), "", "", "", "",false);
}

Level_1::~Level_1()
{
}

Level* Level_2::getNextLevel()
{
	return new Level_3(character);
}

Level_2::Level_2()
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(6.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(6.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(6.0f, 0.0f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(6.0f, 3.0f, -2.0f), glm::vec3(8.0f, 1.49f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(14.0f, 0.0f, -2.0f), glm::vec3(6.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(14.0f, -0.5f, -2.0f), glm::vec3(6.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(22.0f, 0.7f, -2.0f), glm::vec3(6.0f, 0.7f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(22.0f, 0.0f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(28.0f, 3.7f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(28.0f, 2.2f, -2.0f), glm::vec3(6.0f, 2.2f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(36.0f, 3.9f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(38.0f, 3.9f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(40.0f, 3.9f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(42.0f, 0.0f, -2.0f), glm::vec3(5.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(42.0f, -0.5f, -2.0f), glm::vec3(5.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(50.0f, 0.0f, -2.0f), glm::vec3(6.2f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(50.2f, 3.0f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(56.2f, 0.0f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	interiors.push_back(Cube(glm::vec3(6.0f, 1.51f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg", "dirt",false));
	interiors.push_back(Cube(glm::vec3(50.2f, 1.51f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg", "dirt",false));
	interiors.push_back(Cube(glm::vec3(56.2f, 3.01f, -2.0f), glm::vec3(8.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));


	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 6.0f, 0.0f), 1.0, 0.0014, 0.000007));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(8.5f, 1.51f, -0.5f), 1.0, 0.35, 0.44));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(11.5f, 1.51f, -0.5f), 1.0, 0.35, 0.44));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(53.1f, 1.51f, -0.5f), 1.0, 0.35, 0.44));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(58.7f, 3.01f, -0.5f), 1.0, 0.35, 0.44));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(61.7f, 3.01f, -0.5f), 1.0, 0.35, 0.44));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(21.5f, 10.0f, -0.5f), 1.0, 0.022, 0.0019));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(38.5f, 15.0f, -0.5f), 1.0, 0.022, 0.0019));

	cubes.push_back(Cube(glm::vec3(56.2f, 0.0f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));


	skybox = CubeMap("../Glitter/Resources/Textures/nightsky/");

	DestCubes.push_back(Cube(glm::vec3(5.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 1.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 1.0f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 2.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 2.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 2.0f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	DestCubes.push_back(Cube(glm::vec3(23.0f, 1.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 1.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 1.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 2.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 2.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 2.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 3.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 3.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 3.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 4.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 4.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 4.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	DestCubes.push_back(Cube(glm::vec3(33.0f, 4.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 4.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 4.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 5.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 5.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 5.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 6.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 6.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 6.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 7.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 7.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 7.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(63.2f, 1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "", "", "", "",false);
}

Level_2::Level_2(MainCharacter* mc) : Level(mc)
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(6.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(0.0f, -0.5f, -2.0f), glm::vec3(6.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(6.0f, 0.0f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(6.0f, 3.0f, -2.0f), glm::vec3(8.0f, 1.49f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(14.0f, 0.0f, -2.0f), glm::vec3(6.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(14.0f, -0.5f, -2.0f), glm::vec3(6.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(22.0f, 0.7f, -2.0f), glm::vec3(6.0f, 0.7f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(22.0f, 0.0f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(28.0f, 3.7f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(28.0f, 2.2f, -2.0f), glm::vec3(6.0f, 2.2f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(36.0f, 3.9f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(38.0f, 3.9f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(40.0f, 3.9f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(42.0f, 0.0f, -2.0f), glm::vec3(5.0f, 0.5f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));
	cubes.push_back(Cube(glm::vec3(42.0f, -0.5f, -2.0f), glm::vec3(5.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(50.0f, 0.0f, -2.0f), glm::vec3(6.2f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(50.2f, 3.0f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	cubes.push_back(Cube(glm::vec3(56.2f, 0.0f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	interiors.push_back(Cube(glm::vec3(6.0f, 1.51f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg", "dirt",false));
	interiors.push_back(Cube(glm::vec3(50.2f, 1.51f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg", "dirt",false));
	interiors.push_back(Cube(glm::vec3(56.2f, 3.01f, -2.0f), glm::vec3(8.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));


	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 6.0f, 0.0f), 1.0, 0.0014, 0.000007));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(8.5f, 1.51f, -0.5f), 1.0, 0.35, 0.44));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(11.5f, 1.51f, -0.5f), 1.0, 0.35, 0.44));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(53.1f, 1.51f, -0.5f), 1.0, 0.35, 0.44));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(58.7f, 3.01f, -0.5f), 1.0, 0.35, 0.44));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(61.7f, 3.01f, -0.5f), 1.0, 0.35, 0.44));

	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(21.5f, 10.0f, -0.5f), 1.0, 0.022, 0.0019));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(38.5f, 15.0f, -0.5f), 1.0, 0.022, 0.0019));

	cubes.push_back(Cube(glm::vec3(56.2f, 0.0f, -2.0f), glm::vec3(8.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	
	skybox = CubeMap("../Glitter/Resources/Textures/nightsky/");

	DestCubes.push_back(Cube(glm::vec3(5.0f, 1.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 1.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 1.0f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 2.0f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 2.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(5.0f, 2.0f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	DestCubes.push_back(Cube(glm::vec3(23.0f, 1.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 1.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 1.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 2.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 2.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 2.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 3.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 3.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 3.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 4.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 4.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(23.0f, 4.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	DestCubes.push_back(Cube(glm::vec3(33.0f, 4.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 4.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 4.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 5.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 5.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 5.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 6.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 6.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 6.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 7.7f, -2.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 7.7f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));
	DestCubes.push_back(Cube(glm::vec3(33.0f, 7.7f, 0.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(63.2f, 1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "", "", "", "",false);
}

Level_2::~Level_2(){}

Level* Level_3::getNextLevel()
{
	return new Level_1(character);
}
Level_3::Level_3()
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(8.0f, 1.5f, -2.0f), glm::vec3(8.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(13.2f, 4.5f, -2.0f), glm::vec3(3.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(18.7f, 1.2f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(22.5f, 1.2f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(24.7f, 1.2f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(27.7f, 2.0f, -2.0f), glm::vec3(8.0f, 3.5f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(35.7f, 1.5f, -2.0f), glm::vec3(4.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(39.7f, 1.5f, -2.0f), glm::vec3(10.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));
	
	cubes.push_back(Cube(glm::vec3(39.7f, 4.51f, -2.0f), glm::vec3(10.0f, 0.1f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(52.2f, 1.6f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(55.2f, 1.6f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));

	interiors.push_back(Cube(glm::vec3(39.7f, 4.5f, -2.0f), glm::vec3(10.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg","",false));



	//interiors.push_back(Cube(glm::vec3(12.0f, 1.0f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	objects.push_back(Model("../Glitter/Resources/Objects/Sign-controls/sign.obj"));
	objects[0].model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, -1, 0));
	objects[0].model = glm::translate(objects[0].model, glm::vec3(-1.0f, -0.5f, -1.5f));
	objects[0].model = glm::scale(objects[0].model, glm::vec3(3.0f, 3.0f, 3.0f));
	objects.push_back(Model("../Glitter/Resources/Objects/framed_photo/framed_photo.obj"));
	objects[1].model = glm::translate(glm::mat4(1.0f), glm::vec3(13.5f, 1.5f, -1.99f));


	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 3.0f, 0.0f), 1.0, 0.022, 0.0019));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(13.5f, 2.51f, -0.5f), 1.0, 0.014, 0.0007));

	skybox = CubeMap("../Glitter/Resources/Textures/woodBox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(56.2f, 1.5f, -2.0f), glm::vec3(1.0f, 3.5f, 3.0f), "", "", "", "", false);
}
Level_3::Level_3(MainCharacter* mc) : Level(mc)
{
	cubes.push_back(Cube(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(6.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(8.0f, 1.5f, -2.0f), glm::vec3(8.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(13.2f, 4.5f, -2.0f), glm::vec3(3.0f, 1.5f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(18.7f, 1.2f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(22.5f, 1.2f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(24.7f, 1.2f, -2.0f), glm::vec3(1.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "drawer", ".jpg", "wood"));

	cubes.push_back(Cube(glm::vec3(27.7f, 2.0f, -2.0f), glm::vec3(8.0f, 3.5f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(35.7f, 1.5f, -2.0f), glm::vec3(4.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(39.7f, 1.5f, -2.0f), glm::vec3(10.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(39.7f, 4.51f, -2.0f), glm::vec3(10.0f, 0.1f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(52.2f, 1.6f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), "../Glitter/Resources/Textures/", "wood", ".jpg"));

	cubes.push_back(Cube(glm::vec3(55.2f, 1.6f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "grass", ".jpg"));

	interiors.push_back(Cube(glm::vec3(39.7f, 4.5f, -2.0f), glm::vec3(10.0f, 3.0f, 3.0f), "../Glitter/Resources/Textures/", "wood", ".jpg", "", false));



	//interiors.push_back(Cube(glm::vec3(12.0f, 1.0f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f), "../Glitter/Resources/Textures/", "dirt", ".jpg"));

	//lights.push_back(new DirectionalLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f) , 0, glm::vec3(-0.2f, -1.0f, -0.2f)));

	objects.push_back(Model("../Glitter/Resources/Objects/Sign-controls/sign.obj"));
	objects[0].model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, -1, 0));
	objects[0].model = glm::translate(objects[0].model, glm::vec3(-1.0f, -0.5f, -1.5f));
	objects[0].model = glm::scale(objects[0].model, glm::vec3(3.0f, 3.0f, 3.0f));
	objects.push_back(Model("../Glitter/Resources/Objects/framed_photo/framed_photo.obj"));
	objects[1].model = glm::translate(glm::mat4(1.0f), glm::vec3(13.5f, 1.5f, -1.99f));


	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(0.0f, 2.0f, 0.0f), 1.0, 0.0014, 0.000007));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(4.0f, 2.5f, 0.0f), 1.0, 0.045, 0.0075));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(1.0f, 3.0f, 0.0f), 1.0, 0.022, 0.0019));
	lights.push_back(new PointLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1, glm::vec3(13.5f, 2.51f, -0.5f), 1.0, 0.014, 0.0007));

	skybox = CubeMap("../Glitter/Resources/Textures/woodBox/");

	DestCubes.push_back(Cube(glm::vec3(2.0f, 1.0f, -1.0f), glm::vec3(glm::vec3(1.0f, 1.0f, 1.0f)), "../Glitter/Resources/Textures/", "container2", ".png"));

	objective = new Cube(glm::vec3(56.2f, 1.5f, -2.0f), glm::vec3(1.0f, 3.5f, 3.0f), "", "", "", "", false);
}
