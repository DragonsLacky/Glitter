#ifndef GAME_LEVEL
#define GAME_LEVEL
#include "MainCharacter.hpp"
#include "glitter.hpp"
#include "Cube.hpp"
#include "AABB.hpp"
#include "CubeMap.hpp"

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
	vector<Model> objects;
	vector<Model> DestructableObjects;
	Cube* objective;
	CubeMap skybox;

	virtual void processMovement(GLFWwindow* window, float deltaTime);
	virtual void Draw();
	bool Collision(AABB& box);
	void CollisionBellow();
	bool CollisionRightSide();
	bool CollisionLeftSide();
	bool CollisionAboveCrouch();
	bool CollisionAbove();
	virtual Level* getNextLevel() = 0;
	Level();
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
	~Level_1();
	virtual Level* getNextLevel();

private:

};



class Level_2 : public Level
{
public:
	Level_2();
	~Level_2();
	virtual Level* getNextLevel();
private:

};


#endif // !GAME_LEVEL



