#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "scene_object.h"

class Ray
{
public:
	glm::vec3 mP0 =  glm::vec3(0);
	glm::vec3 mDir = glm::vec3(0,0,-1);
	glm::vec3 mHitPoint = glm::vec3(0);
	int mObjectIndex = -1;
	float mDist = 0; // distance from P0 to hit point
	
public:
	Ray() {}
	Ray(glm::vec3 source, glm::vec3 direction)
	{
		const float RSTEP = 0.005f; // ray stepping distance
		mP0 = source;
		mDir = glm::normalize(direction);
		mP0 += RSTEP * mDir;
	}
	void ClosestPt(std::vector<SceneObject*>& rSceneObjects);

};
