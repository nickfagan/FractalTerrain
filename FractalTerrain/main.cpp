#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "fractalterrain.hpp"
#include "algebra.hpp"
#include "utilities.hpp"

FractalTerrain ft;

//-------------------------------------------------------
//--- Debug
//-------------------------------------------------------

int moveSpeed	= 5;
double x_trans	= 0;
double y_trans	= -100;
double z_trans	= -400;
double x_rot	= 20;
double y_rot	= 0;

bool mouseDown = false;
Point2D lastMousePos;

Vector3D look_direction		= Vector3D(0, 0, -1);
Vector3D right_direction	= Vector3D(1, 0, 0);

void keyboard(unsigned char k, int x, int y)
{
	switch(k)
	{
		case 'w':
			x_trans += look_direction[0]*moveSpeed;
			y_trans += look_direction[1]*moveSpeed;
			z_trans -= look_direction[2]*moveSpeed;
			break;
		case 's':
			x_trans -= look_direction[0]*moveSpeed;
			y_trans -= look_direction[1]*moveSpeed;
			z_trans += look_direction[2]*moveSpeed;
			break;
		case 'd':
			x_trans -= right_direction[0]*moveSpeed;
			z_trans += right_direction[2]*moveSpeed;
			break;
		case 'a':
			x_trans += right_direction[0]*moveSpeed;
			z_trans -= right_direction[2]*moveSpeed;
			break;
	}
}

void motion(int x, int y)
{
	if (mouseDown)
	{
		double rot_x_change = (double)(y-lastMousePos.y)/5.0;
		double rot_y_change = (double)(x-lastMousePos.x)/5.0;
		
		x_rot  += rot_x_change;
		y_rot  += rot_y_change;
		
		x_rot = (x_rot < 0) ? x_rot + 360 : x_rot;
		x_rot = (x_rot > 360) ? x_rot - 360 : x_rot;
		
		y_rot = (y_rot < 0) ? y_rot + 360 : y_rot;
		y_rot = (y_rot > 360) ? y_rot - 360 : y_rot;
		
		look_direction = rotation(x_rot, 'x')*Vector3D(0, 0, -1);
		look_direction = rotation(y_rot, 'y')*look_direction;
		
		right_direction = rotation(-90, 'y')*look_direction;
		
		lastMousePos = Point2D(x, y);
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == 0)
	{
		mouseDown		= (state == GLUT_DOWN);
		lastMousePos	= Point2D(x, y);
	}
}

//-------------------------------------------------------

void display(void)
{
	// Clear some stuff
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glLoadIdentity();
	
	glRotatef(x_rot, 1, 0, 0);
	glRotatef(y_rot, 0, 1, 0);
	glTranslatef(x_trans, y_trans, z_trans);
	
    ft.render();
    
    glFinish();
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(40.0, (GLfloat)w/(GLfloat)h, 0.1, 3000.0);
    
    /* Reset to modelview matrix mode to avoid confusion later */
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    int windowWidth		= 800;
    int windowHeight	= 600;
	
	ft.initialize();
	
    // intialize glut and main window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Fractal Terrain");
    
    // Initialize callbacks
    glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	
	// Enable some stuff
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	
	// Initialize lights
	GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 0.0f};
	GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 0.0f};
	GLfloat light_position[] = { 100.0, 100.0, 1.0, 0.0};
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
    reshape(windowWidth, windowHeight);
	
    glutMainLoop();
    
    return 0;
}
