#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "glitter.hpp"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture1 {
	unsigned int id;
	string type;
	string path;
};

class Mesh
{

public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture1> textures;
	unsigned int VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture1> textures);
	void Draw(Shader& shader);
	~Mesh();
private:
	unsigned int VBO, EBO;
	void setupMesh();
};

#endif