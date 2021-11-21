/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "MyCylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray.
*/
float MyCylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    //~ glm::vec3 vdif = p0 - center;   //Vector s (see Slide 28)

    float a = pow(dir.x, 2) + pow(dir.z, 2);
    float b = 2 * (dir.x * (p0.x-center.x) + dir.z * (p0.z-center.z));
    float c = pow(p0.x-center.x, 2) + pow(p0.z-center.z, 2) - pow(radius, 2);
    float delta = pow(b, 2) - 4 * a * c;



    if(delta < 0.001) return -1.0;    //includes zero and negative values

    float t1 = (-b - sqrt(delta)) / 2 * a;
    float t2 = (-b + sqrt(delta)) / 2 * a;

    bool t1_intersect = ((center.y - p0.y) <= t1*dir.y) && (t1*dir.y <= height-p0.y+center.y);
    bool t2_intersect = ((center.y - p0.y) <= t2*dir.y) && (t2*dir.y <= height-p0.y+center.y);
    if (!t1_intersect && !t2_intersect)
        return -1.0;
    if (t1_intersect && t2_intersect)
        { if (t1 <= t2)
            return t1;
          else
            return t2;
        }
    if (!t1_intersect)
        return t2;
    else
        return t1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 MyCylinder::normal(glm::vec3 p)
{
    glm::vec3 n = p - glm::vec3(center.x, p.y, center.z);
    n = glm::normalize(n);
    return n;
}

