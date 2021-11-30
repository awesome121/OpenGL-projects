/**
 * Class Cylinder
 * Subclass of scene object class.
 * This is an implementation of abstract class SceneObject.
 * 
 * Implementation of  intersect() and normal()
 * needs to be provided.
 * 
 */

#include "cylinder.h"
#include <math.h>

float Cylinder::Intersect(glm::vec3 p0, glm::vec3 dir)
{
	glm::vec3 vdiff = p0 - mBottomCenter;
	float delta = 2 * dir.x * dir.z * vdiff.x * vdiff.z
	- (pow(dir.x * vdiff.z, 2) + pow(dir.z * vdiff.x, 2)
	- pow(mRadius, 2) * (pow(dir.x, 2) + pow(dir.z, 2)));
	
	if (delta < 1.e-6) return -1; // not intersect
	float t1 = ( - (dir.x * vdiff.x + dir.z * vdiff.z) - sqrt(delta))
	/ (pow(dir.x, 2) + pow(dir.z, 2));
	float t2 = ( - (dir.x * vdiff.x + dir.z * vdiff.z) + sqrt(delta))
	/ (pow(dir.x, 2) + pow(dir.z, 2));
	
	float y1 = p0.y + t1 * dir.y;
	float y2 = p0.y + t2 * dir.y;
	if (y1 <= (mBottomCenter.y + mHeight) && y2 <= (mBottomCenter.y + mHeight))
		return t1;
	else if (y1 <= (mBottomCenter.y + mHeight)  && y2 > (mBottomCenter.y + mHeight) )
		return t1;
	else if (y1 > (mBottomCenter.y + mHeight)  && y2 <= (mBottomCenter.y + mHeight) )
		return (mBottomCenter.y + mHeight - p0.y) / dir.y; // intersect but exceed height
	else
		return -1;
}



/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cylinder::GetNormal(glm::vec3 p)
{
    glm::vec3 n = p - glm::vec3(mBottomCenter.x, p.y, mBottomCenter.z);
    return glm::normalize(n);
}
