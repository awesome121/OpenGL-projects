/*
 * Header file for class Plane
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
class Plane : public SceneObject
{
private:
	glm::vec3 mPtrA = glm::vec3(0);
	glm::vec3 mPtrB = glm::vec3(0);
	glm::vec3 mPtrC = glm::vec3(0);
	glm::vec3 mPtrD = glm::vec3(0);
	int mNumVertices = 4;
	
public:
	Plane() = default;
	
	Plane(glm::vec3 ptr_a, glm::vec3 ptr_b, glm::vec3 ptr_c, glm::vec3 ptr_d)
	: mPtrA(ptr_a), mPtrB(ptr_b), mPtrC(ptr_c), mPtrD(ptr_d), mNumVertices(4) {}
	
	Plane(glm::vec3 ptr_a, glm::vec3 ptr_b, glm::vec3 ptr_c)
	: mPtrA(ptr_a), mPtrB(ptr_b), mPtrC(ptr_c), mNumVertices(3) {}
	
	bool IsInside(glm::vec3 ptr);
	
	float Intersect(glm::vec3 posn, glm::vec3);
	
	glm::vec3 GetNormal(glm::vec3);
	
	int GetNumVerts();
	
	
};
