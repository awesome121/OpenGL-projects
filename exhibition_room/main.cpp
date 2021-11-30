/**
* Exibition room main module
* Rendered using Ray tracing technique
* 
*/

#include "main.h"


glm::vec3 getFloorColor(Ray* &ray)
{
	glm::vec3 color(0);
	int stripeWidth = 5;
	int ix = (ray->mHitPoint.x+40) / stripeWidth;
	int iz = (ray->mHitPoint.z) / stripeWidth;
	int k = (ix + iz) % 2;
	if (k == 0) color = glm::vec3(0.952, 0.898, 0.737);
	else color = glm::vec3(1.00, 0.52, 0.02);
	return color;
}


void setMiddleWallTexture(Ray* &ray, SceneObject* &obj)
{
		float x0 = -25;
        float x1 = 25;
        float y0 = 0;
        float y1 = 30; 
        float texcoords = (ray->mHitPoint.x - x0) / (x1 - x0);
        float texcoordt = (ray->mHitPoint.y - y0) / (y1 - y0);
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->SetColor(textures::middleWallMural.getColorAt(texcoords, texcoordt));
        }
}

void setLeftWallTexture(Ray* &ray, SceneObject* &obj)
{
		float y0 = 0;
        float y1 = 30;
        float z0 = -200;
        float z1 = -100; 
        float texcoords = (ray->mHitPoint.z - z0) / (z1 - z0);
        float texcoordt = (ray->mHitPoint.y - y0) / (y1 - y0);
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->SetColor(textures::leftWallMural.getColorAt(texcoords, texcoordt));
        }
}
void setRightWallTexture(Ray* &ray, SceneObject* &obj)
{
		float y0 = 0;
        float y1 = 30;
        float z0 = -200;
        float z1 = -100; 

        float texcoords = (ray->mHitPoint.z - z0) / (z1 - z0);
        float texcoordt = (ray->mHitPoint.y - y0) / (y1 - y0);
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            obj->SetColor(textures::rightWallMural.getColorAt(texcoords, texcoordt));
        }
}

glm::vec3 getLightingColor(Ray* &ray, SceneObject* &pObj)
{
		glm::vec3 color(0);
        glm::vec3 scale = glm::vec3(0.2);
        color += scale * pObj->Lighting(lights::TOP_LIGHT_POS, -ray->mDir, ray->mHitPoint);
        color += scale * pObj->Lighting(lights::RIGHT_FLOOR_LIGHT_POS, -ray->mDir, ray->mHitPoint);
        color += scale * pObj->Lighting(lights::LEFT_FLOOR_LIGHT_POS, -ray->mDir, ray->mHitPoint);
        color += scale * pObj->Lighting(lights::FRONT_FLOOR_LIGHT_POS, -ray->mDir, ray->mHitPoint);
        color += scale * pObj->Lighting(lights::BACK_FLOOR_LIGHT_POS, -ray->mDir, ray->mHitPoint);
        return color;
}

float getShadowCoff(Ray* &ray, SceneObject* &pObj)
{
	glm::vec3 v_light = lights::TOP_LIGHT_POS - ray->mHitPoint;
    Ray shadow_ray(ray->mHitPoint, v_light);
    shadow_ray.ClosestPt(sceneObjects);
    if (shadow_ray.mObjectIndex > -1 && shadow_ray.mDist < glm::length(v_light)) {
        if(sceneObjects[shadow_ray.mObjectIndex]->IsRefractive() \
        || sceneObjects[shadow_ray.mObjectIndex]->IsTransparent()){
            return 0.7f;
        }
        else {
            return 0.3f;
        }
    }
    return 1;
}



/** 
 * Trace ray recursively
 * Takes a ray and current step,
 * Returns color of the current cell cumulatively 
 */ 

glm::vec3 trace(Ray* ray, int step)
{
	
	glm::vec3 cumulative_color(0);
	if (step == MAX_STEPS) return cumulative_color;
	glm::vec3 backgroundCol(1, 1, 1);
	
	SceneObject* pObj; // incident object
	ray->ClosestPt(sceneObjects);   
	if(ray->mObjectIndex == -1) return backgroundCol;
	pObj = sceneObjects[ray->mObjectIndex];    
	
	glm::vec3 saved_color = pObj->GetColor();         // save original color, apply texture color with lighting
	if (ray->mObjectIndex == 0) 
		cumulative_color += getFloorColor(ray);
    if (ray->mObjectIndex == 1) 
		setMiddleWallTexture(ray, pObj);
    if (ray->mObjectIndex == 2) 
		setLeftWallTexture(ray, pObj);
    if (ray->mObjectIndex == 3) 
		setRightWallTexture(ray, pObj);
    if (ray->mObjectIndex == 8) // front ball
    {
        glm::vec3 n = pObj->GetNormal(ray->mHitPoint);
        float texcoords = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
        float texcoordt = -asin(n.y) / M_PI + 0.5;
        if (texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            pObj->SetColor(textures::ballMiddleWallFront.getColorAt(texcoords, texcoordt));
        }
    }
	// lighting
    if (ray->mObjectIndex != 0) // not apply lighting on floor
		cumulative_color += getLightingColor(ray, pObj);
	
	// shadow
    cumulative_color *= getShadowCoff(ray, pObj);
    
    if (pObj->IsReflective() && step < MAX_STEPS)
    {
        float rho = pObj->GetReflectionCoeff();
        glm::vec3 reflected_dir = glm::reflect(ray->mDir,  pObj->GetNormal(ray->mHitPoint));
        Ray reflected_ray(ray->mHitPoint, reflected_dir);
        glm::vec3 reflected_color = trace(&reflected_ray, step + 1);
        cumulative_color += (rho * reflected_color);
    }

    if (pObj->IsRefractive() && step < MAX_STEPS)
    {
        float eta = 1.01;
        glm::vec3 n = pObj->GetNormal(ray->mHitPoint);
        glm::vec3 g = glm::refract(ray->mDir, n, eta);
        Ray refra_in_ray(ray->mHitPoint, g);
        refra_in_ray.ClosestPt(sceneObjects);
        glm::vec3 m = pObj->GetNormal(refra_in_ray.mHitPoint);
        glm::vec3 h = glm::refract(g, -m, 1.0f/eta);
        Ray refra_out_ray(refra_in_ray.mHitPoint, h);
        cumulative_color = trace(&refra_out_ray, step+1);
    }

    if (pObj->IsTransparent() && step < MAX_STEPS)
    {
        Ray trancy_in_ray(ray->mHitPoint, ray->mDir);
        trancy_in_ray.ClosestPt(sceneObjects);

        Ray trancy_out_ray(trancy_in_ray.mHitPoint, trancy_in_ray.mDir);
        trancy_out_ray.ClosestPt(sceneObjects);

        float trans = pObj->GetTransparencyCoeff();
        cumulative_color = pObj->GetColor() * (1-trans) + trans*trace(&trancy_out_ray, step+1);

    }
    pObj->SetColor(saved_color); // get back the original color
    return cumulative_color;
}


/** 
 * Registered display funtion
 * Trace the primary ray
 */
void display()
{
    float xp, yp;  // grid point
    float cellX = (grid::XMAX-grid::XMIN)/grid::NUMDIV;  // cell width
    float cellY = (grid::YMAX-grid::YMIN)/grid::NUMDIV;  // cell height
    glm::vec3 eye(0., 0., 0.);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS); 
    for(int i = 0; i < grid::NUMDIV; i++) 
    {
        xp = grid::XMIN + i*cellX;
        for(int j = 0; j < grid::NUMDIV; j++)
        {
            yp = grid::YMIN + j*cellY;
            glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -grid::EDIST);  // direction of the primary ray
            Ray ray = Ray(eye, dir);
            glm::vec3 col = trace (&ray, 1);
            if (IS_SUPERSAMPLING) {
				for (int ii=1; ii<=3; ii+=2)
				{
					for (int jj=1; jj<=3; jj+=2)
					{
						glm::vec3 dir1(xp+ii*0.25*cellX, yp+jj*0.25*cellY, -grid::EDIST);
						Ray ray1 = Ray(eye, dir1);
						col += trace (&ray1, 1);
					}
				}
                col = col / glm::vec3(5);
            }
            glColor3f(col.r, col.g, col.b);
			// Draw each cell with its color value
			glVertex2f(xp, yp);             
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);
        }
    }
    glEnd();
    glFlush();
}


void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(grid::XMIN, grid::XMAX, grid::YMIN, grid::YMAX);

    glClearColor(0, 0, 0, 1);
    textures::leftWallMural = TextureBMP("img/left_wall.bmp");
    textures::middleWallMural = TextureBMP("img/middle_wall.bmp");
    textures::rightWallMural = TextureBMP("img/right_wall.bmp");
    textures::ballMiddleWallFront = TextureBMP("img/ball_middle_wall_front.bmp");

    glm::vec3 vertex1 = glm::vec3(-30, -10, -20);
    glm::vec3 vertex2 = glm::vec3(30, -10, -20);
    glm::vec3 vertex3 = glm::vec3(30, 50, -20);
    glm::vec3 vertex4 = glm::vec3(-30, 50, -20);
    glm::vec3 vertex5 = glm::vec3(-30, -10, -250);
    glm::vec3 vertex6 = glm::vec3(30, -10, -250);
    glm::vec3 vertex7 = glm::vec3(30, 50, -250);
    glm::vec3 vertex8 = glm::vec3(-30, 50, -250);

    Plane *floor = new Plane(vertex1, vertex2, vertex6, vertex5); //index: 0
    floor->SetSpecularity(false);
    sceneObjects.push_back(floor);

    Plane *front_wall = new Plane(vertex5, vertex6, vertex7, vertex8);//index: 1
    front_wall->SetColor(glm::vec3(0.952, 0.898, 0.737));
    front_wall->SetSpecularity(false);
    sceneObjects.push_back(front_wall);

    Plane *left_wall = new Plane(vertex1, vertex5, vertex8, vertex4);//index: 2
    left_wall->SetColor(glm::vec3(0.952, 0.898, 0.737));
    left_wall->SetSpecularity(false);
    sceneObjects.push_back(left_wall);

    Plane *right_wall = new Plane(vertex2, vertex3, vertex7, vertex6);//index: 3
    right_wall->SetColor(glm::vec3(0.952, 0.898, 0.737));
    right_wall->SetSpecularity(false);
    sceneObjects.push_back(right_wall);

    Plane *ceiling = new Plane(vertex8, vertex7, vertex3, vertex4);//index: 4
    ceiling->SetColor(glm::vec3(0.952, 0.898, 0.737));
    ceiling->SetSpecularity(false);
    sceneObjects.push_back(ceiling);

    Sphere *sphereHollow = new Sphere(glm::vec3(0.0, -2, -50.0), 3.0); //hollow sphere, index 5
    sphereHollow->SetColor(glm::vec4(0.71, 0.74, 0.87, 1));
    sphereHollow->SetTransparency(true, 0.3);
    sphereHollow->SetReflectivity(true, 0.9);
    sceneObjects.push_back(sphereHollow);

    Sphere *sphere_left = new Sphere(glm::vec3(-20, 5, -150.0), 4.0);//index 6
    sphere_left->SetRefractivity(true, 1.01, 1.01);
    sceneObjects.push_back(sphere_left);

    Sphere *sphere_right = new Sphere(glm::vec3(20, 5, -150.0), 4.0);//index 7
    sphere_right->SetRefractivity(true, 1.01, 1.01);
    sceneObjects.push_back(sphere_right);

    Sphere *sphere_front = new Sphere(glm::vec3(0.5, 3, -200.0), 2.0);//index 8
    sceneObjects.push_back(sphere_front);

    Cylinder *left_cylinder_base = new Cylinder(glm::vec3(-20, -10, -150.0), 1.0, 11);//index 9
    left_cylinder_base->SetColor(glm::vec3(1,0,0));
    front_wall->SetSpecularity(false);
    sceneObjects.push_back(left_cylinder_base);

    Cylinder *right_cylinder_base = new Cylinder(glm::vec3(20, -10, -150.0), 1.0, 11);//index 10
    right_cylinder_base->SetColor(glm::vec3(1,0,0));
    front_wall->SetSpecularity(false);
    sceneObjects.push_back(right_cylinder_base);

    Cylinder *front_cylinder_base = new Cylinder(glm::vec3(0.5, -10, -200.0), 1.0, 11);//index 11
    front_cylinder_base->SetColor(glm::vec3(1,0,0));
    front_wall->SetSpecularity(false);
    sceneObjects.push_back(front_cylinder_base);

    // Cube
    float center_x = 0;
    float center_y = -8;
    float center_z = -50;
    float length = 6;
    glm::vec3 cube_color = glm::vec3(0.71, 0.95, 0.70);

    glm::vec3 vertex9 = glm::vec3(center_x-length/2, center_y-length/2, center_z+length/2);
    glm::vec3 vertex10 = glm::vec3(center_x+length/2, center_y-length/2, center_z+length/2);
    glm::vec3 vertex11 = glm::vec3(center_x+length/2, center_y+length/2, center_z+length/2);
    glm::vec3 vertex12 = glm::vec3(center_x-length/2, center_y+length/2, center_z+length/2);

    glm::vec3 vertex13 = glm::vec3(center_x-length/2, center_y-length/2, center_z-length/2);
    glm::vec3 vertex14 = glm::vec3(center_x+length/2, center_y-length/2, center_z-length/2);
    glm::vec3 vertex15 = glm::vec3(center_x+length/2, center_y+length/2, center_z-length/2);
    glm::vec3 vertex16 = glm::vec3(center_x-length/2, center_y+length/2, center_z-length/2);


    Plane *cube_top = new Plane(vertex12, vertex11, vertex15, vertex16);//index: 4
    cube_top->SetColor(cube_color);
    cube_top->SetReflectivity(true, 0.5);
    cube_top->SetSpecularity(false);
    sceneObjects.push_back(cube_top);

    Plane *cube_bottom = new Plane(vertex9, vertex10, vertex14, vertex13);//index: 4
    cube_bottom->SetColor(cube_color);
    cube_bottom->SetSpecularity(false);
    sceneObjects.push_back(cube_bottom);

    Plane *cube_left = new Plane(vertex16, vertex13, vertex9, vertex12);//index: 4
    cube_left->SetColor(cube_color);
    cube_left->SetSpecularity(false);
    sceneObjects.push_back(cube_left);

    Plane *cube_right = new Plane(vertex10, vertex14, vertex15, vertex11);//index: 4
    cube_right->SetColor(cube_color);
    cube_right->SetSpecularity(false);
    sceneObjects.push_back(cube_right);

    Plane *cube_front = new Plane(vertex12, vertex11, vertex10, vertex9);//index: 4
    cube_front->SetColor(cube_color);
    cube_front->SetReflectivity(true, 0.5);
    cube_front->SetSpecularity(false);
    sceneObjects.push_back(cube_front);

    Plane *cube_back = new Plane(vertex16, vertex15, vertex14, vertex13);//index: 4
    cube_back->SetColor(cube_color);
    cube_back->SetSpecularity(false);
    sceneObjects.push_back(cube_back);
    

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Exhibition Room");
    
    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
