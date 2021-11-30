/**
 * Header file for class Sphere
 * Subclass of scene object class.
 * This is an implementation of abstract class SceneObject.
 * 
 * Implementation of  intersect() and normal()
 * needs to be provided.
 * 
 */
#pragma once
#include <glm/glm.hpp>
#include "scene_object.h"

class Sphere : public SceneObject
{
private:
	glm::vec3 mCenter = glm::vec3(0);
	float mRadius = 1;
		
public:
	Sphere() {}
	
	Sphere(glm::vec3 center, float radius) :
	mCenter(center), mRadius(radius) {}
	
	float Intersect(glm::vec3 p0, glm::vec3 dir);
	
	glm::vec3 GetNormal(glm::vec3 p0);
	
	
};
