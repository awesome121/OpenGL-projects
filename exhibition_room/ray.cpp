/**
 * Ray object: can be either primary or 
 * secondary rays
 * 
 */

#include "ray.h"

void Ray::ClosestPt(std::vector<SceneObject*> &rSceneObjects)
{
	glm::vec3 intersect_point(0,0,0);
	float tmin = 1.e+6;
    for(int i = 0;  i < (int) rSceneObjects.size();  i++)
    {
        float t = rSceneObjects[i]->Intersect(mP0, mDir);
        if (t > 0)        // Intersects an object
        {
            intersect_point = mP0 + mDir*t;
            if (t < tmin)
            {
                mHitPoint = intersect_point;
                mObjectIndex = i;
                mDist = t;
                tmin = t;
            }
        }
    }
}
