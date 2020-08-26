#ifndef MAINCHARACTER
#define MAINCHARACTER

#include <vector>
#include "Model.hpp"
#include <iostream>
#include "ResourceManager.hpp"

using namespace std;

//state 0 = normal, 1 = walking, 2 = walking, 3 = crouch-normal 4 = crouch-walk, 5 = crouch-walk, 4 = jump

enum walk_direction
{
    Left, Right
};

class MainCharacter
{
public:
	MainCharacter();
	~MainCharacter();
	vector<Model> character;
	int modelState;
	glm::mat4 model;
	AABB boundingBox;
	bool altWalk;
	void updateBoxes(glm::vec3 m);
	float facingRight = true;
	int framecount_walk = 0;
	int numberOfJumpFrames = 0;
	int numJumps = 0;
	bool attacked = false;

	void jump();
	void crouchHold();
	void crouchRelease();
	void walk(walk_direction dir, float deltatime);
	void gravity();
	void resetBoxes();
	void resetModel();
	AABB& attack();
    void Draw();
	void flipBoundingBox(bool right);
    bool finishedJump = true;
	bool pressedBeforeJump = false;
	bool pressedBeforeCrouch = false;
    AABB box();

	
};

#endif // MAINCHARACTER

