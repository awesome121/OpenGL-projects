#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <fstream>


using namespace std;
int x, y;
int isRectangle = 0;

class glPoint{
	public:
		int x, y;
	};
int count = 0;
glPoint points[3] = {{10, 10}, {300, 450}, {580, 20}};

void drawSierpinski()
{
	glPoint pre_pt = points[rand()%3];
	glVertex2i(pre_pt.x, pre_pt.y);
	for (int i=0; i<30000; i++)
	{
		glBegin(GL_POINTS);
			int r_num = rand()%3;
			pre_pt.x = (points[r_num].x+pre_pt.x)/2.0f;
			pre_pt.y = (points[r_num].y+pre_pt.y)/2.0f;
			glVertex2i(pre_pt.x, pre_pt.y);
		glEnd();
	}
}


void myDisplay()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	drawSierpinski();
	glFlush();
}


void myMouse(int button, int state, int x, int y)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glPointSize(3);
			glBegin(GL_POINTS);
				glVertex2i(x, 480-y);
			glEnd();
		glPointSize(1);
		
		points[count].x = x;
		points[count].y = 480-y;
		if (count == 2) {
			count = 0;
			drawSierpinski();
		} else {
			count += 1;
		}
	}
	
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		count = 0;
	}
	
	glFlush();
	
}

void myInit()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	
	glutCreateWindow("Sierpinski_triangle");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	
	myInit();
	glutMainLoop();
	
}
