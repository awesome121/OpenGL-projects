//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Vase.cpp
//  See Lab03 (II) for details
//  ========================================================================

#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include "loadTGA.h"

using namespace std;

const int N = 37;
float angle=100, look_x=15, look_z=0, eye_x=50, eye_z=70;  //Camera parameters
float cam_hgt = 20.0;  //Rotation angle, camera height
float broken_disk_vx[N] = {0};
float broken_disk_vy[N] = {0};
float broken_disk_vz[N] = {0};

float cylinder_vx[N] = {0};
float cylinder_vy[N] = {0};
float cylinder_vz[N] = {0};

double beta = 90; // degree of the spinning balls
float theta = 0; // degree of pendulum
bool reverse = false;
bool stop = false;
int v = 0;

GLuint texid[10];

void loadTexture()
{
    glGenTextures(10, texid);
    glBindTexture(GL_TEXTURE_2D, texid[0]); //cylinders
    loadTGA("cylinders.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texid[2]); //ball
    loadTGA("ball.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, texid[3]); //sphere
    loadTGA("spinning_sphere.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, texid[4]); //hangers
    loadTGA("hangers.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
	

    glBindTexture(GL_TEXTURE_2D, texid[6]); //floor
    loadTGA("floor.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, texid[7]); // walls
    loadTGA("wall.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);


    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void loadPoints(int object) //0:broken_ring, 1:cylinder
{
    if (object == 0) {
        for (int i=0; i<N; i++)
        {
            float disk_x = -1.5;
            broken_disk_vx[i] = disk_x + 0.2 * cos((float)10*i/180*M_PI);
            broken_disk_vy[i] = 0.2 * sin((float)10*i/180*M_PI);
        }
    } else if (object == 1) {
        for (int i=0; i<31; i++)
        {
            cylinder_vx[i] = 1;
            cylinder_vy[i] = i;
            cylinder_vz[i] = 0;
        }
	}

}

void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}


void drawCylinder(bool isShadow)
{
    if (!isShadow)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texid[0]);
    }
    loadPoints(1);
    float vx[37], vy[37], vz[37];
    float wx[37], wy[37], wz[37];

    for (int i = 0; i < N; i++)     //Initialize data everytime
    {
        vx[i] = cylinder_vx[i];
        vy[i] = cylinder_vy[i];
        vz[i] = cylinder_vz[i];
    }

    for (int j = 0; j <= 36; j++)
    {
        for (int i = 0; i < N; i++) {
            wx[i] = vx[i] * cos(10*3.1415/180) + vz[i] * sin(10*3.1415/180);
            wy[i] = vy[i];
            wz[i] = -vx[i] * sin(10*3.1415/180) + vz[i] * cos(10*3.1415/180);
        }

        glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 30; i++) {
                if (i > 0) normal(vx[i-1], vy[i-1], vz[i-1], wx[i-1], wy[i-1], wz[i-1], vx[i], vy[i], vz[i]);
                if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1], wx[i], wy[i], wz[i], vx[i], vy[i], vz[i]);
                glTexCoord2f((float)j/36, (float)i/15);
                glVertex3f(vx[i], vy[i], vz[i]);
                glTexCoord2f((float)(j+1)/36, (float)i/15);
                glVertex3f(wx[i], wy[i], wz[i]);

            }
            glVertex3f(wx[0], wy[0], wz[0]);
        glEnd();

        for (int i =0; i < N; i++)
        {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void drawBrokenDisk(bool isShadow)
{
    if (!isShadow)
        glColor3f(1, 0.0, 0.0);
    loadPoints(0);
    float vx[N], vy[N], vz[N];
    float wx[N], wy[N], wz[N];
    for (int i = 0; i < N; i++)     //Initialize data everytime
    {
        vx[i] = broken_disk_vx[i];
        vy[i] = broken_disk_vy[i];
        vz[i] = broken_disk_vz[i]; 
    }

    for (int j = 0; j <= 33; j++)
    {
        for (int i = 0; i < N; i++) {
            wx[i] = vx[i] * cos(10*3.1415/180) + vz[i] * sin(10*3.1415/180);
            wy[i] = vy[i];
            wz[i] = -vx[i] * sin(10*3.1415/180) + vz[i] * cos(10*3.1415/180);
        }

        glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < N; i++) {
                if (i > 0) normal(vx[i], vy[i], vz[i], wx[i], wy[i], wz[i], wx[i-1], wy[i-1], wz[i-1]);
                //~ if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1], wx[i], wy[i], wz[i], vx[i], vy[i], vz[i]);
                glVertex3f(vx[i], vy[i], vz[i]);
                glVertex3f(wx[i], wy[i], wz[i]);
            }
            glVertex3f(wx[0], wy[0], wz[0]);
        glEnd();

        for (int i =0; i < N; i++)
        {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }

}




void initialise()
{
    float grey[4] = {0.4, 0.4, 0.4, 1.0};
    float light_grey[4] = {0.76,0.81,0.85, 1.0};
    float white[4]  = {1.0, 1.0, 1.0,.0};

    loadTexture();
    glEnable(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_grey);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
	
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
}


void drawSurroundings()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid[6]);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(-200, -0.1, -200);

        glTexCoord2f(15, 0);
        glVertex3f(200, -0.1, -200);

        glTexCoord2f(15, 15);
        glVertex3f(200, -0.1, 200);

        glTexCoord2f(0, 15);
        glVertex3f(-200, -0.1, 200);

    }
    glEnd();


    int height = 200;
    glBindTexture(GL_TEXTURE_2D, texid[7]);
    for (int i=-1; i!=3; i+=2) {
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0, 0);
            glVertex3f(-200*i, 0, -200*i);

            glTexCoord2f(2, 0);
            glVertex3f(200*i, 0, -200*i);

            glTexCoord2f(2, 1);
            glVertex3f(200*i, height, -200*i);

            glTexCoord2f(0, 1);
            glVertex3f(-200*i, height, -200*i);

        }
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texid[7]);
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0, 0);
            glVertex3f(200*i, 0, -200*i);

            glTexCoord2f(2, 0);
            glVertex3f(200*i, 0, 200*i);

            glTexCoord2f(2, 1);
            glVertex3f(200*i, height, 200*i);

            glTexCoord2f(0, 1);
            glVertex3f(200*i, height, -200*i);

        }
        glEnd();
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}


void drawSpinningPart(bool isShadow)
{
    glPushMatrix(); // spanning cylinder
        drawCylinder(isShadow);
    glPopMatrix();

    glPushMatrix();
		if (!isShadow)
            glEnable(GL_TEXTURE_2D);
        glRotated(beta, 0.0f, 1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, texid[3]);
        glPushMatrix();// spinning sphere
            if (!isShadow) glColor3f(1, 1, 1);
            glTranslatef(0, 30, 0);
            GLUquadricObj *sphere_obj = gluNewQuadric();
            gluQuadricTexture(sphere_obj, GL_TRUE);
            gluSphere(sphere_obj, 3, 30, 30);
        glPopMatrix();

        
        glBindTexture(GL_TEXTURE_2D, texid[2]);
        glPushMatrix();// spinning ball
            glTranslatef(0, 15, 15);
            GLUquadricObj *sphere_ball1 = gluNewQuadric();
            gluQuadricTexture(sphere_ball1, GL_TRUE);
            gluSphere(sphere_ball1, 1, 30, 30);
        glPopMatrix();


        glPushMatrix();// spinning ball
            glRotated(8.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0, 15, 15);
            GLUquadricObj *sphere_ball2 = gluNewQuadric();
            gluQuadricTexture(sphere_ball2, GL_TRUE);
            gluSphere(sphere_ball2, 1, 30, 30);
        glPopMatrix();


        glPushMatrix();// spinning ball
            glRotated(-8.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0, 15, 15);
            GLUquadricObj *sphere_ball3 = gluNewQuadric();
            gluQuadricTexture(sphere_ball3, GL_TRUE);
            gluSphere(sphere_ball3, 1, 30, 30);
        glPopMatrix();


        glPushMatrix();// spinning ball
            glTranslatef(0, 15, -15);
            GLUquadricObj *sphere_ball4 = gluNewQuadric();
            gluQuadricTexture(sphere_ball4, GL_TRUE);
            gluSphere(sphere_ball4, 1, 30, 30);
        glPopMatrix();


        glPushMatrix();// spinning ball
            glRotated(8.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0, 15, -15);
            GLUquadricObj *sphere_ball5 = gluNewQuadric();
            gluQuadricTexture(sphere_ball5, GL_TRUE);
            gluSphere(sphere_ball5, 1, 30, 30);
        glPopMatrix();


        glPushMatrix();// spinning ball
            glRotated(-8.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0, 15, -15);
            GLUquadricObj *sphere_ball6 = gluNewQuadric();
            gluQuadricTexture(sphere_ball6, GL_TRUE);
            gluSphere(sphere_ball6, 1, 30, 30);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);



        glPushMatrix(); // linen
            glColor3f(0, 0, 0);
            glTranslatef(0, 30, 0);
            glRotated(45.0f, 1.0f, 0.0f, 0.0f);
            GLUquadricObj *cylinder_linen1 = gluNewQuadric();
            gluCylinder(cylinder_linen1, 0.1, 0.1, 15*sqrtf(2), 30, 30); // length is 18*sqrt(2)
        glPopMatrix();


        glPushMatrix(); // linen
            glColor3f(0, 0, 0);
            glTranslatef(0, 30, 0);
            glRotated(8.0f, 0.0f, 1.0f, 0.0f);
            glRotated(45.0f, 1.0f, 0.0f, 0.0f);
            GLUquadricObj *cylinder_linen2 = gluNewQuadric();
            gluCylinder(cylinder_linen2, 0.1, 0.1, 15*sqrtf(2), 30, 30);
        glPopMatrix();

        glPushMatrix(); // linen
            glColor3f(0, 0, 0);
            glTranslatef(0, 30, 0);
            glRotated(-8.0f, 0.0f, 1.0f, 0.0f);
            glRotated(45.0f, 1.0f, 0.0f, 0.0f);
            GLUquadricObj *cylinder_linen3 = gluNewQuadric();
            gluCylinder(cylinder_linen3, 0.1, 0.1, 15*sqrtf(2), 30, 30);
        glPopMatrix();


        glPushMatrix(); // linen
            glColor3f(0, 0, 0);
            glTranslatef(0, 30, 0);
            glRotated(-45.0f, 1.0f, 0.0f, 0.0f);
            glRotated(180.0f, 0.0f, 1.0f, 0.0f);
            GLUquadricObj *cylinder_linen4 = gluNewQuadric();
            gluCylinder(cylinder_linen4, 0.1, 0.1, 15*sqrtf(2), 30, 30); // length is 18*sqrt(2)
        glPopMatrix();


        glPushMatrix(); // linen
            glColor3f(0, 0, 0);
            glTranslatef(0, 30, 0);
            glRotated(8.0f, 0.0f, 1.0f, 0.0f);
            glRotated(-45.0f, 1.0f, 0.0f, 0.0f);
            glRotated(180.0f, 0.0f, 1.0f, 0.0f);
            GLUquadricObj *cylinder_linen5 = gluNewQuadric();
            gluCylinder(cylinder_linen5, 0.1, 0.1, 15*sqrtf(2), 30, 30);
        glPopMatrix();


        glPushMatrix(); // linen
            glColor3f(0, 0, 0);
            glTranslatef(0, 30, 0);
            glRotated(-8.0f, 0.0f, 1.0f, 0.0f);
            glRotated(-45.0f, 1.0f, 0.0f, 0.0f);
            glRotated(180.0f, 0.0f, 1.0f, 0.0f);
            GLUquadricObj *cylinder_linen6 = gluNewQuadric();
            gluCylinder(cylinder_linen6, 0.1, 0.1, 15*sqrtf(2), 30, 30);
        glPopMatrix();
    glPopMatrix();
}


void drawPendulumPart(bool isShadow)
{
	if (!isShadow) {glColor3f(1, 1, 1); glEnable(GL_TEXTURE_2D);}
	
    glPushMatrix();// ring cylinder
        glTranslatef(30, 0, -5);
        drawCylinder(isShadow);
    glPopMatrix();


	
    glPushMatrix();// hanger box
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texid[4]);
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glTexCoord2f(0, 0); glVertex3f(30-4, 30-4, -0.99);
				glTexCoord2f(1, 0); glVertex3f(30+4, 30-4, -0.99);
				glTexCoord2f(1, 1); glVertex3f(30+4, 30+4, -0.99);
				glTexCoord2f(0, 1); glVertex3f(30-4, 30+4, -0.99);
			glEnd();
			
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glTexCoord2f(0, 0); glVertex3f(30-4, 30-4, -9.01);
				glTexCoord2f(1, 0); glVertex3f(30+4, 30-4, -9.01);
				glTexCoord2f(1, 1); glVertex3f(30+4, 30+4, -9.01);
				glTexCoord2f(0, 1); glVertex3f(30-4, 30+4, -9.01);
			glEnd();
			
			glBegin(GL_QUADS);
				glNormal3f(-1, 0, 0);
				glTexCoord2f(0, 0); glVertex3f(30-4-0.01, 30-4, -9);
				glTexCoord2f(1, 0); glVertex3f(30-4-0.01, 30-4, -1);
				glTexCoord2f(1, 1); glVertex3f(30-4-0.01, 30+4, -1);
				glTexCoord2f(0, 1); glVertex3f(30-4-0.01, 30+4, -9);
			glEnd();
			
			glBegin(GL_QUADS);
				glNormal3f(1, 0, 0);
				glTexCoord2f(0, 0); glVertex3f(30+4+0.01, 30-4, -1);
				glTexCoord2f(1, 0); glVertex3f(30+4+0.01, 30-4, -9);
				glTexCoord2f(1, 1); glVertex3f(30+4+0.01, 30+4, -9);
				glTexCoord2f(0, 1); glVertex3f(30+4+0.01, 30+4, -1);
			glEnd();
			
			glBegin(GL_QUADS);
				glNormal3f(0, 1, 0);
				glTexCoord2f(0, 0); glVertex3f(30-4, 30+4+0.01, -1);
				glTexCoord2f(1, 0); glVertex3f(30+4, 30+4+0.01, -1);
				glTexCoord2f(1, 1); glVertex3f(30+4, 30+4+0.01, -9);
				glTexCoord2f(0, 1); glVertex3f(30-4, 30+4+0.01, -9);
			glEnd();
			
			
		glTranslatef(30, 30, -5);
		glutSolidCube(8);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    

    glPushMatrix();
        glTranslatef(30, 30, 0);
        glRotated(-theta, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, texid[4]);
        glPushMatrix(); // hanger
			if (!isShadow) {glColor3f(1, 1, 1); glEnable(GL_TEXTURE_2D);}
			glTranslatef(0, 0, -3);
			GLUquadricObj *cylinder_hanger = gluNewQuadric();
			gluQuadricTexture(cylinder_hanger, GL_TRUE);
			gluCylinder(cylinder_hanger, 1, 1, 5, 30, 30);
		glPopMatrix();
		
		glPushMatrix(); // hanger cover
			if (!isShadow) {glColor3f(1, 1, 1); glEnable(GL_TEXTURE_2D);}
			glTranslatef(0, 0, 2);
			GLUquadricObj *hanger_cover = gluNewQuadric();
			gluQuadricTexture(hanger_cover, GL_TRUE);
			gluDisk(hanger_cover, 0, 1, 20, 20);
		glPopMatrix();
		
		glDisable(GL_TEXTURE_2D);

        glPushMatrix(); // disk
            glTranslatef(15, -15, 0);
            glRotatef(-90, 1, 0, 0);
            glRotatef(-35, 0, 1, 0);
            drawBrokenDisk(isShadow);
        glPopMatrix();


        glPushMatrix(); // linen7
            glColor3f(0, 0, 0);
            glRotated(45.0f, 0.0f, 0.0f, 1.0f);
            glRotated(90.0f, 1.0f, 0.0f, 0.0f);
            GLUquadricObj *cylinder_linen7 = gluNewQuadric();
            gluCylinder(cylinder_linen7, 0.2, 0.2, 15*sqrtf(2)-1.6, 30, 30);
        glPopMatrix();
    glPopMatrix();

}

void drawModels(void)
{
	glColor3f(1, 1, 1);
    drawSurroundings();
    glEnable(GL_LIGHTING);
    drawSpinningPart(false);
    drawPendulumPart(false);

    float shadowMat[16] = { 100,0,0,0, -40,0,-100,-1,
    0,0,100,0, 0,0,0,100 };
    glDisable(GL_LIGHTING);
    glPushMatrix(); //Draw Shadow Object
        glMultMatrixf(shadowMat);
        glColor3f(0, 0, 0);
        drawSpinningPart(true);
        drawPendulumPart(true);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}


void display(void)
{
    float lgt_pos[]={5.0f, 50.0f, 100.0f, 1.0f};  //light0 position

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, cam_hgt, eye_z,  look_x, 0, look_z,   0, 1, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position


    drawModels();
    glutSwapBuffers();
}

void special(int key, int x, int y)
{
    int cam_bound = 150;

    if(key == GLUT_KEY_LEFT) angle -= 0.05;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.05;


    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        float move_x = 0.5*sin(angle);
        float move_z = 0.5*cos(angle);

        if (eye_x - move_x > cam_bound) {eye_x = cam_bound;} else {eye_x-=move_x;}
        if (eye_x - move_x < -cam_bound) {eye_x = -cam_bound;} else {eye_x-=move_x;}
        if (eye_z + move_z < -cam_bound) {eye_z = -cam_bound;} else {eye_z+=move_z;}
        if (eye_z + move_z > cam_bound) {eye_z = cam_bound;} else{eye_z+=move_z;}

    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        float move_x = 0.5*sin(angle);
        float move_z = 0.5*cos(angle);
        if (eye_x + move_x > cam_bound) {eye_x = cam_bound;} else {eye_x+=move_x;}
        if (eye_x + move_x < -cam_bound) {eye_x = -cam_bound;} else {eye_x+=move_x;}
        if (eye_z - move_z < -cam_bound) {eye_z = -cam_bound;} else {eye_z-=move_z;}
        if (eye_z - move_z > cam_bound) {eye_z = cam_bound;} else{eye_z-=move_z;}
    }


    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

void keyBoard(unsigned char key, int x, int y)
{
    if (key == ' ') stop = !stop;
    else if(key=='w') cam_hgt+=1;
    else if(key=='s') cam_hgt-=1;
    if(cam_hgt < 10) cam_hgt = 10;
    else if(cam_hgt > 100) cam_hgt = 100;

    glutPostRedisplay();
}



void myTimer(int index)
{
    if (stop) {glutTimerFunc(10, myTimer, 0);return;}
    if (reverse) {
        if (theta == 0 and v == 0) {
            reverse = !reverse;


        } else {
            if (theta == 45) {}
            else if (theta > 45) {v += 1;}
            else v -= 1;
            theta -= 0.1*v * 1;

        }
    } else { // positive direction
        if (theta >= 90 and v == 0) {
            reverse = !reverse;

        } else {
            if (theta == 45) {}
            else if (theta > 45) {v -= 1;}
            else v += 1;
            theta += 0.1*v*1;
        }
    }

    if (beta == 360) beta = 0;
    beta += 6;


    glutPostRedisplay();
    glutTimerFunc(10, myTimer, 0);
}



int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_SINGLE| GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("assignment1");
   initialise ();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyBoard);
   glutTimerFunc(10, myTimer, 0);
   glutMainLoop();
   return 0;
}
