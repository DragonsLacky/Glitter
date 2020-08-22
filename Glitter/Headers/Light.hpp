#include <glitter.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

//enum LightType
//{
//	directional, spotlight, pointLight
//};

class Light
{
public:
	Light();
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type);
	virtual ~Light();
	int type;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 color;
};


class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type, glm::vec3 direction);
	~DirectionalLight();

	glm::vec3 direction;
};


class PointLight : public Light
{
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type, glm::vec3 position, float constant, float linear, float quadratic);
	PointLight() = default;
	~PointLight();

	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

};



class SpotLight : public PointLight
{
public:
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type, glm::vec3 direction, glm::vec3 position, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
	~SpotLight();

	glm::vec3 direction;
	float cutOff;
	float outerCutOff;
};

