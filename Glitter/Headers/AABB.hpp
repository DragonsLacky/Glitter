#ifndef AABB_BOX
#define AABB_BOX
#include "glitter.hpp"

class AABB
{
public:
	AABB(glm::vec3 min, glm::vec3 max);
	AABB() {}
	~AABB();
	glm::vec3 min;
	glm::vec3 max;
};
bool intersect(AABB a, AABB b);

#endif // !AABB_BOX



