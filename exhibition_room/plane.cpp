/*
 * Class Plane
 * Subclass of scene object class.
 * This is an implementation of abstract class SceneObject.
 * 
 * Implementation of  intersect() and normal()
 * needs to be provided.
 * 
 */

#include "plane.h"
#include <math.h>

/* 
 * Take a Ray object and return its distance from p0 towards
 * the plane in float. Return -1 if not intersecting.
 * The ray has a starting point p0 and a normalised
 * direction vector.
 */
float Plane::Intersect(glm::vec3 p0, glm::vec3 dir)
{
	glm::vec3 v_normal = GetNormal(p0);
	float cos_mPtrDn = glm::dot(dir, v_normal);
	if (fabs(cos_mPtrDn) < 1.e-4) return -1;  // Parallel
	
	glm::vec3 v_diff = mPtrA - p0;
	float t = glm::dot(v_diff, v_normal)/cos_mPtrDn;
	if (t < 0) return -1;
	
	glm::vec3 q = p0 + dir * t;
	if (IsInside(q)) return t;
	else return -1; // not within the plane
}

/*
 * Return the unit normal vector of a plane
 */
 glm::vec3 Plane::GetNormal(glm::vec3 ptr)
 {
	glm::vec3 v1 = mPtrC - mPtrB;
    glm::vec3 v2 = mPtrA - mPtrB;
    glm::vec3 n = glm::cross(v1, v2);
    return glm::normalize(n);
 }
 
 /*
  * Return true if a ptr is inside the plane
  * Involving plane inclusion test
  */
 bool Plane::IsInside(glm::vec3 q)
 {
	glm::vec3 n = GetNormal(q);     //Normal vector at the point of intersection
    glm::vec3 ua = mPtrB - mPtrA, ub = mPtrC - mPtrB, uc = mPtrD - mPtrC, ud = mPtrA - mPtrD;
    glm::vec3 va = q - mPtrA, vb = q - mPtrB, vc = q - mPtrC, vd = q - mPtrD;
    if (mNumVertices == 3) uc = mPtrA - mPtrC;
    float ka = glm::dot(glm::cross(ua, va), n);
    float kb = glm::dot(glm::cross(ub, vb), n);
    float kc = glm::dot(glm::cross(uc, vc), n);
    float kd;
    if (mNumVertices == 4)
        kd = glm::dot(glm::cross(ud, vd), n);
    else
        kd = ka;
    if (ka > 0 && kb > 0 && kc > 0 && kd > 0) return true;
    if (ka < 0 && kb < 0 && kc < 0 && kd < 0) return true;
    else return false;
 }
 
 /*
  * Return the number of vertices defining the plane
  */
  int Plane::GetNumVerts()
  {
	  return mNumVertices;
  }
	 
