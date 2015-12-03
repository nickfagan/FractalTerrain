#include <cmath>
#include <cstdlib>
#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "fractalterrain.hpp"
#include "utilities.hpp"

void FractalTerrain::initialize()
{
	srand(time(NULL));
	
	dimension		= pow(2, ITERATIONS) + 1;
	num_vertices	= dimension * dimension;
	mountains		= new double[num_vertices];
	
	// Initialize the terrain corners
	int corner1 = 0;
	int corner2 = dimension - 1;
	int corner3 = num_vertices - dimension;
	int corner4 = num_vertices - 1;
	
	mountains[corner1] = 0;
	mountains[corner2] = 0;
	mountains[corner3] = 0;
	mountains[corner4] = 0;

	generate_terrain();
	
	// Get the lowest height value, this will be used to draw the walls around the mountina
	bottomVal = INT_MAX;
	for (int i = 0; i < num_vertices; i++)
	{
		if (mountains[i] < bottomVal)
		{
			bottomVal = mountains[i];
		}
	}
	
	// Move it down a little bit more
	bottomVal -= 10;
}

void FractalTerrain::render()
{
	if (dl_index == 0)
	{
		// Generate a new display list
		dl_index = glGenLists(1);
		glNewList(dl_index, GL_COMPILE);

		glPushMatrix();

		double size = 200.0 / (double)dimension;
		
		// Set the material for the mountain
		GLfloat mat_diffuse[] = {0.4, 0.4, 0.4};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

		for (int i = 0; i < dimension - 1; i++)
		{
			for (int j = 0; j < dimension - 1; j++)
			{
				int p1i = i * dimension + j;
				int p2i = i * dimension + (j + 1);
				int p3i = (i + 1) * dimension + (j + 1);
				int p4i = (i + 1) * dimension + j;

				Point3D p1 = Point3D(j * size, mountains[p1i], i * size);
				Point3D p2 = Point3D((j + 1) * size, mountains[p2i], i * size);
				Point3D p3 = Point3D((j + 1) * size, mountains[p3i], (i + 1) * size);
				Point3D p4 = Point3D(j * size, mountains[p4i], (i + 1) * size);

				Vector3D n1, n2, u, v;

				u = p3 - p1;
				v = p2 - p1;
				n1 = u.cross(v);

				u = p4 - p1;
				v = p3 - p1;
				n2 = u.cross(v);

				glBegin(GL_TRIANGLES);
				glNormal3f(n1[0], n1[1], n1[2]);
				glVertex3f(p1[0], p1[1], p1[2]);
				glVertex3f(p2[0], p2[1], p2[2]);
				glVertex3f(p3[0], p3[1], p3[2]);

				glNormal3f(n2[0], n2[1], n2[2]);
				glVertex3f(p1[0], p1[1], p1[2]);
				glVertex3f(p3[0], p3[1], p3[2]);
				glVertex3f(p4[0], p4[1], p4[2]);
				glEnd();
				
				
//				glBegin(GL_QUADS);
//				if(i == 0)
//				{
//					Point3D bottomP1 = Point3D((j + 1) * size, bottomVal, i * size);
//					Point3D bottomP2 = Point3D(j * size, bottomVal, i * size);
//					Vector3D normal;
//					
//					u		= p1 - bottomP1;
//					v		= p2 - bottomP1;
//					normal	= u.cross(v);
//					
//					glNormal3f(normal[0], normal[1], normal[2]);
//					glVertex3f(p1[0], p1[1], p1[2]);
//					glVertex3f(p2[0], p2[1], p2[2]);
//					glVertex3f(bottomP1[0], bottomP1[1], bottomP1[2]);
//					glVertex3f(bottomP2[0], bottomP2[1], bottomP2[2]);
//				}
//				else if (i == dimension-2)
//				{
//					Point3D bottomP1 = Point3D(j * size, bottomVal, (i + 1) * size);
//					Point3D bottomP2 = Point3D((j + 1) * size, bottomVal, (i + 1) * size);
//					Vector3D normal;
//					
//					u		= p3 - bottomP1;
//					v		= p4 - bottomP1;
//					normal	= u.cross(v);
//					
//					glNormal3f(normal[0], normal[1], normal[2]);
//					glVertex3f(p3[0], p3[1], p3[2]);
//					glVertex3f(p4[0], p4[1], p4[2]);
//					glVertex3f(bottomP1[0], bottomP1[1], bottomP1[2]);
//					glVertex3f(bottomP2[0], bottomP2[1], bottomP2[2]);
//				}
//				
//				if (j == 0)
//				{
//					Point3D bottomP2 = Point3D(j * size, bottomVal, i * size);
//					Point3D bottomP3 = Point3D(j * size, bottomVal, (i + 1) * size);
//					Vector3D normal;
//					
//					u		= p1 - bottomP2;
//					v		= p4 - bottomP2;
//					normal	= u.cross(v);
//					
//					glNormal3f(normal[0], normal[1], normal[2]);
//					glVertex3f(p1[0], p1[1], p1[2]);
//					glVertex3f(bottomP2[0], bottomP2[1], bottomP2[2]);
//					glVertex3f(bottomP3[0], bottomP3[1], bottomP3[2]);
//					glVertex3f(p4[0], p4[1], p4[2]);
//				}
//				else if (j == dimension-2)
//				{
//					Point3D bottomP1 = Point3D((j + 1) * size, bottomVal, i * size);
//					Point3D bottomP4 = Point3D((j + 1) * size, bottomVal, (i + 1) * size);
//					Vector3D normal;
//					
//					u		= p2 - bottomP1;
//					v		= p3 - bottomP1;
//					normal	= u.cross(v);
//					
//					glNormal3f(normal[0], normal[1], normal[2]);
//					glVertex3f(bottomP1[0], bottomP1[1], bottomP1[2]);
//					glVertex3f(p2[0], p2[1], p2[2]);
//					glVertex3f(p3[0], p3[1], p3[2]);
//					glVertex3f(bottomP4[0], bottomP4[1], bottomP4[2]);
//					
//				}
//				glEnd();
			}
		}

		glPopMatrix();
		glEndList();

		// No longer need the mountians height data
		delete [] mountains;
	}
	else
	{
		glCallList(dl_index);
	}
}

void FractalTerrain::generate_terrain()
{
	int		step	= (dimension - 1)/2.0;
	double	ratio	= (double) pow(2,-MOUNTAIN_ROUGHNESS);
	double	height	= HEIGHT_RANGE * ratio;

	for (int i = 0; i < ITERATIONS; i++)
	{
		fractalize(step, height);

		height	= height * ratio;
		step	= step / 2;
	}
}

void FractalTerrain::fractalize(int step, double height)
{
	for (int i = step; i < dimension; i += 2*step)
	{
		for (int j = step; j < dimension; j += 2*step)
		{
			mountains[(i * dimension) + j] = diamond_height(i, j, step, height);
		}
	}
	
	bool even_column = false;
	
	for (int i = 0; i < dimension; i += step)
	{
		even_column = !even_column;

		for (int j = even_column ? step : 0; j < dimension; j += 2*step)
		{
			mountains[(i * dimension) + j] = square_height(i, j, step, height);
		}
	}
}

double FractalTerrain::diamond_height(int i, int j, int step, double height)
{
	double top_left		= mountains[((i - step) * dimension) + j - step];
	double top_right	= mountains[((i - step) * dimension) + j + step];
	double bottom_left	= mountains[((i + step) * dimension) + j - step];
	double bottom_right	= mountains[((i + step) * dimension) + j + step];

	double avg = (top_left + top_right + bottom_left + bottom_right) / 4.0;

	return avg + (((double)rand() / (double)RAND_MAX) * height * 2.0 - height);
}

double FractalTerrain::square_height(int i, int j, int step, double height)
{
	int index1 = ((i - step) * dimension) + j;
	int index2 = ((i + step) * dimension) + j;
	int index3 = (i * dimension) + j - step;
	int index4 = (i * dimension) + j + step;

	double avg;
	
	if(i == 0)
	{
		avg = (mountains[index2] + mountains[index3] + mountains[index4]) / 3.0;
	}
	else if (i == dimension-1)
	{
		avg = (mountains[index1] + mountains[index3] + mountains[index4]) / 3.0;
	}
	else if (j == 0)
	{
		avg = (mountains[index1] + mountains[index2] + mountains[index4]) / 3.0;
	}
	else if (j == dimension-1)
	{
		avg = (mountains[index1] + mountains[index2] + mountains[index3]) / 3.0;
	}
	else
	{
		avg = (mountains[index1] + mountains[index2] + mountains[index3] + mountains[index4]) / 4.0;
	}

	// Return the average + a random height
	return avg + (((double)rand() / (double)RAND_MAX) * height * 2.0 - height);
}
