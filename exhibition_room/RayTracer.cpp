/*==================================================================================
* COSC 363  Computer Graphics (2021)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf  for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "MyCylinder.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"
using namespace std;

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 400; // 600
const int MAX_STEPS = 4;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;
TextureBMP texture1;
TextureBMP texture2;
TextureBMP texture3;
TextureBMP texture4;



//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray* ray, int step)
{
    glm::vec3 backgroundCol(1, 1, 1);
    glm::vec3 floorLightPosRight(28, -9, -150);                 //Floor Light's position
    glm::vec3 floorLightPosLeft(-28, -9, -150);                 //Floor Light's position
    glm::vec3 floorLightPosFront(0, -9.9, -240);
    glm::vec3 floorLightPosBack(0, -9.9, -30);
    glm::vec3 lightPos(15, 39, -90);

    glm::vec3 color(0);
    glm::vec3 trancyColor;
    SceneObject* obj;


    if (step == MAX_STEPS) return color;


    ray->closestPt(sceneObjects);                    //Compare the ray with all objects in the scene
    if(ray->index == -1) return backgroundCol;       //no intersection

    obj = sceneObjects[ray->index];                  //object on which the closest point of intersection is found
    glm::vec3 saved_color = obj->getColor();         //save original color, apply texture color with lighting

    if (ray->index == 0) { // floor
        int stripeWidth = 5;
        int ix = (ray->hit.x+40) / stripeWidth;
        int iz = (ray->hit.z) / stripeWidth;
        int k = (ix + iz) % 2;
        if (k == 0) color = glm::vec3(0.952, 0.898, 0.737);
        else color = glm::vec3(1.00, 0.52, 0.02);
    }

    else if (ray->index == 1) //front wall
    {
        float x0 = -25;
        float x1 = 25;
        float y0 = 0;
        float y1 = 30; //-150

        float texcoords = (ray->hit.x - x0) / (x1 - x0);
        float texcoordt = (ray->hit.y - y0) / (y1 - y0);
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->setColor(texture2.getColorAt(texcoords, texcoordt));
        }
    }


    else if (ray->index == 2) //left wall
    {
        float y0 = 0;
        float y1 = 30;
        float z0 = -200;
        float z1 = -100; //-150

        float texcoords = (ray->hit.z - z0) / (z1 - z0);
        float texcoordt = (ray->hit.y - y0) / (y1 - y0);
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->setColor(texture1.getColorAt(texcoords, texcoordt));
        }

    }

    else if (ray->index == 3) //right wall
    {
        float y0 = 0;
        float y1 = 30;
        float z0 = -200;
        float z1 = -100; //-150

        float texcoords = (ray->hit.z - z0) / (z1 - z0);
        float texcoordt = (ray->hit.y - y0) / (y1 - y0);
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->setColor(texture3.getColorAt(texcoords, texcoordt));
        }

    }

    else if (ray->index == 8) //front ball
    {
        glm::vec3 n = obj->normal(ray->hit);
        float texcoords = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
        float texcoordt = -asin(n.y) / M_PI + 0.5;

        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->setColor(texture4.getColorAt(texcoords, texcoordt));
        }
    }

    if (ray->index != 0) //not apply lighting on floor ------------------
    {
        glm::vec3 scale = glm::vec3(0.2);
        color += scale * obj->lighting(lightPos, -ray->dir, ray->hit);
        color += scale * obj->lighting(floorLightPosLeft, -ray->dir, ray->hit);
        color += scale * obj->lighting(floorLightPosRight, -ray->dir, ray->hit);
        color += scale * obj->lighting(floorLightPosFront, -ray->dir, ray->hit);
        color += scale * obj->lighting(floorLightPosBack, -ray->dir, ray->hit);
    }
    //-------------------------------------------------------------------
    glm::vec3 lightVec = lightPos - ray->hit; // shadow -----------------
    Ray shadowRay(ray->hit, lightVec);
    shadowRay.closestPt(sceneObjects);
    if (shadowRay.index > -1 && shadowRay.dist < glm::length(lightVec)) {
        if(sceneObjects[shadowRay.index]->isRefractive() || sceneObjects[shadowRay.index]->isTransparent()){
            color = 0.7f * color;
        }
        else {
            color = 0.3f * color;
        }
    }

    //-------------------------------------------------------------------

    if (obj-> isReflective() && step < MAX_STEPS)
    {
        float rho = obj->getReflectionCoeff();
        glm::vec3 normalVec = obj->normal(ray->hit);
        glm::vec3 reflectedDir = glm::reflect(ray->dir, normalVec);
        Ray reflectedRay(ray->hit, reflectedDir);
        glm::vec3 reflectedColor = trace(&reflectedRay, step + 1);
        color = color + (rho * reflectedColor);
    }

    if (obj->isRefractive() && step < MAX_STEPS)
    {
        //~ float eta = obj->getRefractionCoeff();
        float eta = 1.01;
        glm::vec3 n = obj->normal(ray->hit);
        glm::vec3 g = glm::refract(ray->dir, n, eta);
        Ray refraInRay(ray->hit, g);
        refraInRay.closestPt(sceneObjects);
        glm::vec3 m = obj->normal(refraInRay.hit);
        glm::vec3 h = glm::refract(g, -m, 1.0f/eta);
        Ray refraOutRay(refraInRay.hit, h);
        color = trace(&refraOutRay, step+1);
    }

    if (obj->isTransparent() && step < MAX_STEPS)
    {
        //~ color = color * glm::vec3(0.5, 0.5, 0.5) + trancyColor;
        Ray trancyInRay(ray->hit, ray->dir);
        trancyInRay.closestPt(sceneObjects);

        Ray trancyOutRay(trancyInRay.hit, trancyInRay.dir);
        trancyOutRay.closestPt(sceneObjects);
        //color = color * glm::vec3(0.6, 0.6, 0.6) * trace(&trancyOutRay, step+1);

        float trans = obj->getTransparencyCoeff();
        color = obj->getColor() * (1-trans) + trans*trace(&trancyOutRay, step+1);

    }


    obj->setColor(saved_color);//get back the original color
    return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
    float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
    glm::vec3 eye(0., 0., 0.);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a tiny quad.

    for(int i = 0; i < NUMDIV; i++) //Scan every cell of the image plane
    {
        xp = XMIN + i*cellX;
        for(int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j*cellY;

            glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);  //direction of the primary ray

            Ray ray = Ray(eye, dir);
            glm::vec3 col = trace (&ray, 1);
            //~ if (ray.index >= 0 && ray.index <= 1 || ray.index == 3 || ray.index == 5) {
                glm::vec3 dir1(xp+0.25*cellX, yp+0.25*cellY, -EDIST);
                Ray ray1 = Ray(eye, dir1);
                glm::vec3 col1 = trace (&ray1, 1);

                glm::vec3 dir2(xp+0.25*cellX, yp+0.75*cellY, -EDIST);
                Ray ray2 = Ray(eye, dir2);
                glm::vec3 col2 = trace (&ray2, 1);

                glm::vec3 dir3(xp+0.75*cellX, yp+0.75*cellY, -EDIST);
                Ray ray3 = Ray(eye, dir3);
                glm::vec3 col3 = trace (&ray3, 1);

                glm::vec3 dir4(xp+0.75*cellX, yp+0.25*cellY, -EDIST);
                Ray ray4 = Ray(eye, dir4);
                glm::vec3 col4 = trace (&ray4, 1);

                glColor3f((col.r+col1.r+col2.r+col3.r+col4.r)/5, (col.g+col1.g+col2.g+col3.g+col4.g)/5, (col.b+col1.b+col2.b+col3.b+col4.b)/5);
            //~ } else {
                //~ glColor3f(col.r, col.g, col.b);

            //~ }

                glVertex2f(xp, yp);             //Draw each cell with its color value
                glVertex2f(xp+cellX, yp);
                glVertex2f(xp+cellX, yp+cellY);
                glVertex2f(xp, yp+cellY);

        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);
    texture1 = TextureBMP("mona.bmp");
    texture2 = TextureBMP("last_supper.bmp");
    texture3 = TextureBMP("newton.bmp");
    texture4 = TextureBMP("ball.bmp");

    glm::vec3 vertex1 = glm::vec3(-30, -10, -20);
    glm::vec3 vertex2 = glm::vec3(30, -10, -20);
    glm::vec3 vertex3 = glm::vec3(30, 50, -20);
    glm::vec3 vertex4 = glm::vec3(-30, 50, -20);
    glm::vec3 vertex5 = glm::vec3(-30, -10, -250);
    glm::vec3 vertex6 = glm::vec3(30, -10, -250);
    glm::vec3 vertex7 = glm::vec3(30, 50, -250);
    glm::vec3 vertex8 = glm::vec3(-30, 50, -250);

    Plane *floor = new Plane(vertex1, vertex2, vertex6, vertex5); //index: 0
    floor->setSpecularity(false);
    sceneObjects.push_back(floor);

    Plane *front_wall = new Plane(vertex5, vertex6, vertex7, vertex8);//index: 1
    //~ front_wall->setColor(glm::vec3(1, 0.929, 0.749));
    front_wall->setColor(glm::vec3(0.952, 0.898, 0.737));
    front_wall->setSpecularity(false);
    sceneObjects.push_back(front_wall);

    Plane *left_wall = new Plane(vertex1, vertex5, vertex8, vertex4);//index: 2, -150
    left_wall->setColor(glm::vec3(0.952, 0.898, 0.737));
    left_wall->setSpecularity(false);
    sceneObjects.push_back(left_wall);

    Plane *right_wall = new Plane(vertex2, vertex3, vertex7, vertex6);//index: 3, -170
    right_wall->setColor(glm::vec3(0.952, 0.898, 0.737));
    right_wall->setSpecularity(false);
    sceneObjects.push_back(right_wall);

    Plane *ceiling = new Plane(vertex8, vertex7, vertex3, vertex4);//index: 4
    ceiling->setColor(glm::vec3(0.952, 0.898, 0.737));
    ceiling->setSpecularity(false);
    sceneObjects.push_back(ceiling);

    Sphere *sphereHollow = new Sphere(glm::vec3(0.0, -2, -50.0), 3.0); //hollow sphere, index 5
    sphereHollow->setColor(glm::vec4(0.71, 0.74, 0.87, 1));
    sphereHollow->setTransparency(true, 0.3);
    sphereHollow->setReflectivity(true, 0.9);
    sceneObjects.push_back(sphereHollow);

    Sphere *sphere_left = new Sphere(glm::vec3(-20, 5, -150.0), 4.0);//index 6
    sphere_left->setRefractivity(true, 1.01, 1.01);
    sceneObjects.push_back(sphere_left);

    Sphere *sphere_right = new Sphere(glm::vec3(20, 5, -150.0), 4.0);//index 7
    sphere_right->setRefractivity(true, 1.01, 1.01);
    sceneObjects.push_back(sphere_right);

    Sphere *sphere_front = new Sphere(glm::vec3(0.5, 3, -200.0), 2.0);//index 8
    //~ sphere_front->setRefractivity(true, 1.01, 1.01);
    sceneObjects.push_back(sphere_front);

    MyCylinder *left_cylinder_base = new MyCylinder(glm::vec3(-20, -10, -150.0), 1.0, 11);//index 9
    left_cylinder_base->setColor(glm::vec3(1,0,0));
    front_wall->setSpecularity(false);
    sceneObjects.push_back(left_cylinder_base);

    MyCylinder *right_cylinder_base = new MyCylinder(glm::vec3(20, -10, -150.0), 1.0, 11);//index 10
    right_cylinder_base->setColor(glm::vec3(1,0,0));
    front_wall->setSpecularity(false);
    sceneObjects.push_back(right_cylinder_base);

    MyCylinder *front_cylinder_base = new MyCylinder(glm::vec3(0.5, -10, -200.0), 1.0, 11);//index 11
    front_cylinder_base->setColor(glm::vec3(1,0,0));
    front_wall->setSpecularity(false);
    sceneObjects.push_back(front_cylinder_base);




    //Cube
    float center_x = 0;
    float center_y = -8;
    float center_z = -50;
    float length = 6;
    glm::vec3 cubeColor = glm::vec3(0.71, 0.95, 0.70);

    glm::vec3 vertex9 = glm::vec3(center_x-length/2, center_y-length/2, center_z+length/2);
    glm::vec3 vertex10 = glm::vec3(center_x+length/2, center_y-length/2, center_z+length/2);
    glm::vec3 vertex11 = glm::vec3(center_x+length/2, center_y+length/2, center_z+length/2);
    glm::vec3 vertex12 = glm::vec3(center_x-length/2, center_y+length/2, center_z+length/2);

    glm::vec3 vertex13 = glm::vec3(center_x-length/2, center_y-length/2, center_z-length/2);
    glm::vec3 vertex14 = glm::vec3(center_x+length/2, center_y-length/2, center_z-length/2);
    glm::vec3 vertex15 = glm::vec3(center_x+length/2, center_y+length/2, center_z-length/2);
    glm::vec3 vertex16 = glm::vec3(center_x-length/2, center_y+length/2, center_z-length/2);


    Plane *topCube = new Plane(vertex12, vertex11, vertex15, vertex16);//index: 4
    topCube->setColor(cubeColor);
    topCube->setReflectivity(true, 0.5);
    topCube->setSpecularity(false);
    sceneObjects.push_back(topCube);

    Plane *bottomCube = new Plane(vertex9, vertex10, vertex14, vertex13);//index: 4
    bottomCube->setColor(cubeColor);
    bottomCube->setSpecularity(false);
    sceneObjects.push_back(bottomCube);

    Plane *leftCube = new Plane(vertex16, vertex13, vertex9, vertex12);//index: 4
    leftCube->setColor(cubeColor);
    leftCube->setSpecularity(false);
    sceneObjects.push_back(leftCube);

    Plane *rightCube = new Plane(vertex10, vertex14, vertex15, vertex11);//index: 4
    rightCube->setColor(cubeColor);
    rightCube->setSpecularity(false);
    sceneObjects.push_back(rightCube);

    Plane *frontCube = new Plane(vertex12, vertex11, vertex10, vertex9);//index: 4
    frontCube->setColor(cubeColor);
    frontCube->setReflectivity(true, 0.5);
    frontCube->setSpecularity(false);
    sceneObjects.push_back(frontCube);

    Plane *backCube = new Plane(vertex16, vertex15, vertex14, vertex13);//index: 4
    backCube->setColor(cubeColor);
    backCube->setSpecularity(false);
    sceneObjects.push_back(backCube);

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
