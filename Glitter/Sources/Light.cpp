#include "Light.hpp"


Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->color = color;
	this->type = type;
}

Light::~Light() {}

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type, glm::vec3 direction) : Light(ambient, diffuse, specular, color, type)
{
	this->direction = direction;
}

DirectionalLight::~DirectionalLight() {}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type, glm::vec3 position, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular, color, type)
{
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

PointLight::~PointLight()
{
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 color, int type, glm::vec3 direction, glm::vec3 position, float constant, float linear, float quadratic, float cutOff, float outerCutOff) : PointLight(ambient, diffuse, specular, color, type, position, constant, linear, quadratic)
{
	this->direction = direction;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
}

SpotLight::~SpotLight()
{
}