/**
 * Header file for class Cylinder
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

class Cylinder : public SceneObject
{
private:
	glm::vec3 mBottomCenter = glm::vec3(0);
	float mRadius = 1;
	float mHeight = 1;
		
public:
	Cylinder() {}
	
	Cylinder(glm::vec3 center, float radius, float height) :
	mBottomCenter(center), mRadius(radius),
	mHeight(height) {}
	
	float Intersect(glm::vec3 p0, glm::vec3 dir);
	
	glm::vec3 GetNormal(glm::vec3 p0);
	
	
};
