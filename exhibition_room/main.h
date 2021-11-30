/**
 * Header file for main.cpp.
 * Namespace "grid" refers to ray tracing grid
 * It is placed in the center of x, y coordinate.
 */
#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <cmath>
#include <vector>

#include "sphere.h"
#include "cylinder.h"
#include "scene_object.h"
#include "ray.h"
#include "plane.h"
#include "textureBMP.h"

const int MAX_STEPS = 4; // Max tracing depth
const bool IS_SUPERSAMPLING = true;
std::vector<SceneObject*> sceneObjects;

namespace grid {
	const int NUMDIV = 400; // Number of cells on grid
	const float WIDTH = 20.0; // Grid width
	const float HEIGHT = 20.0; // Grid height
	const float EDIST = 40.0; // Ray length
	const float XMIN = -WIDTH * 0.5;
	const float XMAX =  WIDTH * 0.5;
	const float YMIN = -HEIGHT * 0.5;
	const float YMAX =  HEIGHT * 0.5;
}

namespace lights {
	glm::vec3 RIGHT_FLOOR_LIGHT_POS(28, -9, -150);       // Floor Light's position
    glm::vec3 LEFT_FLOOR_LIGHT_POS(-28, -9, -150);        // Floor Light's position
    glm::vec3 FRONT_FLOOR_LIGHT_POS(0, -9.9, -240);
    glm::vec3 BACK_FLOOR_LIGHT_POS(0, -9.9, -35);
    glm::vec3 TOP_LIGHT_POS(5, 13, -125.0);
}

namespace textures {
	TextureBMP leftWallMural;
	TextureBMP middleWallMural;
	TextureBMP rightWallMural;
	TextureBMP ballMiddleWallFront;
}
