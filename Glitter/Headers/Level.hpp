#ifndef GAME_LEVEL
#define GAME_LEVEL
#include "MainCharacter.hpp"
#include "glitter.hpp"
#include "Cube.hpp"
#include "AABB.hpp"
#include "CubeMap.hpp"
#include "Light.hpp"
#include "Camera.hpp"

enum LEVEL {
	Level_01, Level_02, Level_03
};

class Level
{
public:
	MainCharacter* character;
	
	vector<Model> enemies;
	vector<Cube> cubes;
	vector<Cube> DestCubes;
	vector<Cube> interiors;
	Cube* lightCube;
	vector<Model> objects;
	vector<Model> DestructableObjects;
	vector<Light*> lights;
	Camera* camera;
	Cube* objective;
	CubeMap skybox;

	virtual void processMovement(GLFWwindow* window, float deltaTime);
	virtual void Draw();
	void DrawShadowMap();
	bool Collision(AABB& box,int i = -1);
	void CollisionBellow();
	bool CollisionRightSide();
	bool CollisionLeftSide();
	bool CollisionAboveCrouch();
	bool CollisionAbove();
	bool checkObjectiveReached();
	virtual Level* getNextLevel() = 0;
	Level();
	Level(MainCharacter* mc);
	~Level();

private:

};

class StartScreen 
{
public:
	Level* Level1;
	bool MouseEnabled;
	Cube StartButton;
	Cube LevelsButton;
	Cube ExitButton;
	StartScreen();
	~StartScreen();
	virtual Level* getNextLevel();
	virtual void Draw();
	virtual void StartScreen::processMovement(GLFWwindow* window, float deltaTime);
};



class Level_1 : public Level
{
public:
	Level_1();
	Level_1(MainCharacter* mc);
	~Level_1();
	virtual Level* getNextLevel();

private:

};



class Level_2 : public Level
{
public:
	Level_2();
	~Level_2();
	Level_2(MainCharacter* mc);
	virtual Level* getNextLevel();
private:

};

class Level_3 : public Level
{
public:
	Level_3();
	~Level_3();
	Level_3(MainCharacter* mc);
	virtual Level* getNextLevel();
private:

};

#endif // !GAME_LEVEL



