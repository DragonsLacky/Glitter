#ifndef CUBE_GAME_OBJ
#define CUBE_GAME_OBJ

#include "Shader.hpp"
#include "glitter.hpp"
#include "Texture.hpp"
#include"AABB.hpp"
#include "ResourceManager.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include "Mesh.hpp"

using namespace std;

struct vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Cube
{
public:
	AABB boundingBox;
	glm::mat4 model;
	Cube() = default;
	Cube(glm::vec3 position, glm::vec3 length, string path, string name, string ext,string side = "",  bool isSolid = true);
	~Cube();
	void Draw();
	void updateModel(glm::vec3 model);
	void updateBox(glm::vec3 model);
	void initVertices(glm::vec3 position, glm::vec3 length);
	void setUpObject(string path, string name, string ext, string side = ""); 
	unsigned int VAO;

private:
	//front - back - left - right - top - bottom
	vector<Texture> textures;
	vector<vertex> vertices;
	string name;

	bool isSolid;
	unsigned int VBO, EBO;

};

vertex genVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture);

#endif
