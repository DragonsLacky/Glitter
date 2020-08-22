#ifndef MODEL_H
#define MODEL_H

#include "glitter.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AABB.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Model
{
public:
	vector<Texture1> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	AABB boundingBox;
	AABB startBox;
	bool gammaCorrection;
	glm::mat4 model;

	Model(char* path, bool gamma = false);
	~Model();
	void Draw(Shader& shader);
	void updateBox(glm::vec3 model);
	void resetBox();
	void updateModelMatrix(glm::mat4 modelm);
private:

	void loadModel(string path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture1> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);

#endif