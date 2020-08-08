#ifndef CUBEMAP_SKYBOX
#define CUBEMAP_SKYBOX

#include "glitter.hpp"

#include "Shader.hpp"
#include "Cube.hpp"

#include <iostream>
#include <vector>


using namespace std;

class CubeMap
{
public:
	CubeMap(string path);
    CubeMap() = default;
	~CubeMap();
    void Draw();
private:
    void initCubeMap();
    vector<vertex> vertices;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int texture;
};
unsigned int loadCubemap(vector<std::string> faces);

#endif