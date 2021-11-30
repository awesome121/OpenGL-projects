/**
 * Class Sphere
 * Subclass of scene object class.
 * This is an implementation of abstract class SceneObject.
 * 
 * Implementation of  intersect() and normal()
 * needs to be provided.
 * 
 */

#include "sphere.h"
#include <math.h>


float Sphere::Intersect(glm::vec3 p0, glm::vec3 dir)
{
	glm::vec3 vdiff = p0 - mCenter;
	float delta = pow(glm::dot(vdiff, dir), 2) - \
	(pow(glm::length(vdiff), 2) - pow(mRadius, 2));
	if (delta < 1.e-6) return -1; // Not or just intersecting
	
	float t1 = - glm::dot(vdiff, dir) - sqrt(delta);
	float t2 = - glm::dot(vdiff, dir) + sqrt(delta);
	
	if (t1 < 0)
    {
        return (t2 > 0) ? t2 : -1;
    }
    else return t1;
}

/**
 * Return normal towards p0
 * Assume p0 is on the sphere
 */
glm::vec3 Sphere::GetNormal(glm::vec3 p0)
{
	return glm::normalize(p0 - mCenter);
}
