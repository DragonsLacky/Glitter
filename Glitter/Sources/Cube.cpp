#include "Cube.hpp"

vertex genVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture)
{
	vertex vertex;
	vertex.Position = position;
	vertex.Normal = normal;
	vertex.TexCoords = texture;
	return vertex;
}
Cube::Cube(glm::vec3 position, glm::vec3 length, string path, string name, string ext, bool isSolid)
{
	model = glm::mat4(1.0f);
	this->isSolid = isSolid;
	initVertices(position, length);
	boundingBox = AABB(glm::vec3(min(position.x, position.x + length.x), min(position.y, position.y - length.y), min(position.z, position.z + length.z)), glm::vec3(max(position.x, position.x + length.x), max(position.y, position.y - length.y), max(position.z, position.z + length.z)));
	//updateBox(glm::vec3(-(2 * position.x + length.x), 0.0f, 0.0f));
	this->name = name;
	setUpObject(path, name, ext);
}

void Cube::initVertices(glm::vec3 position, glm::vec3 length)
{
	//back
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y, position.z				), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f))); //bottom-left
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y, position.z				), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f))); //bottom-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z			), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z			), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(position																		 , glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))); //top-left
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y, position.z				), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f))); //bottom-left

	//front
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y , position.z + length.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f))); //bottom-left
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z + length.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y , position.z + length.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f))); //bottom-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z + length.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y , position.z + length.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f))); //bottom-left
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y			, position.z + length.z), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f))); //top-left

	//left 
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y			, position.z + length.z	), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y , position.z			), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))); //bottom-left
	vertices.push_back(genVertex(position																		 , glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-left
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y , position.z			), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))); //bottom-left
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y			, position.z + length.z	), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y , position.z + length.z	), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-right

	//right
	if (isSolid)
	{
		vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z + length.z	), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-left
		vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z			), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f))); //top-right
		vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y	, position.z			), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-right
		vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y	, position.z			), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-right
		vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y , position.z + length.z	), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))); //bottom-left
		vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z + length.z	), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-left
	}
	
	//bottom
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y, position.z				), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y, position.z + length.z	), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-left
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y, position.z				), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f))); //top-left
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y - length.y, position.z + length.z	), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-left
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y, position.z				), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y - length.y, position.z + length.z	), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f))); //bottom-right

	//top
	vertices.push_back(genVertex(position																		 , glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-left
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z			), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f))); //top-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z + length.z	), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-right
	vertices.push_back(genVertex(glm::vec3(position.x + length.x, position.y			, position.z + length.z	), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f))); //bottom-right
	vertices.push_back(genVertex(glm::vec3(position.x			, position.y			, position.z + length.z	), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f))); //bottom-left
	vertices.push_back(genVertex(position																		 , glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f))); //top-left
}

void Cube::setUpObject(string path, string name, string ext)
{
	ResourceManager::LoadTexture((path + name + "-diffuse" + ext).c_str(), name + "_diffuse");
	ResourceManager::LoadTexture((path + name + "-specular" + ext).c_str(), name + "_specular");


	//ResourceManager::LoadTexture((path + "/top.jpg").c_str(), false, name + "_top");
	//ResourceManager::LoadTexture((path + "/bottom.jpg").c_str(), false, name + "_bottom");
	//textures.push_back(ResourceManager::GetTexture(name + "_sides"));
	//textures.push_back(ResourceManager::GetTexture(name + "_top"));
	textures.push_back(ResourceManager::GetTexture(name + "_diffuse"));
	textures.push_back(ResourceManager::GetTexture(name + "_specular"));

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, TexCoords));

	glBindVertexArray(0);
}

void Cube::Draw()
{
	//Shader& shader = ResourceManager::GetShader("shader");
	//shader.use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	textures[0].Bind();
	glActiveTexture(GL_TEXTURE1);
	textures[1].Bind();
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

void Cube::updateBox(glm::vec3 model)
{
	boundingBox.min = glm::vec3(boundingBox.min.x + model.x, boundingBox.min.y + model.y, boundingBox.min.z + model.z);
	boundingBox.max = glm::vec3(boundingBox.max.x + model.x, boundingBox.max.y + model.y, boundingBox.max.z + model.z);
}

void Cube::updateModel(glm::vec3 model)
{
	this->model = glm::translate(this->model, model);
}

Cube::~Cube()
{
}