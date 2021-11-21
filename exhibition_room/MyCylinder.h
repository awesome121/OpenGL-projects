/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_MYCYLINDER
#define H_MYCYLINDER
#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cylinder located at 'center'
 * with the specified radius, height
 */
class MyCylinder : public SceneObject
{

private:
    glm::vec3 center = glm::vec3(0);
    float radius = 1;
    float height = 1;

public:
    MyCylinder() {};  //Default constructor creates a unit sphere

    MyCylinder(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 p0, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

    //~ void setTransparency(bool flag);

};

#endif //!H_MYCYLINDER
