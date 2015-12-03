#include <cmath>
#include <cstdlib>
#include <iostream>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "fractalterrain.hpp"
#include "constants.hpp"
#include "utilities.hpp"

void FractalTerrain::initialize()
{
	srand(time(NULL));
	
	m_dimension		= pow(2, ITERATIONS) + 1;
	m_num_vertices	= m_dimension * m_dimension;
	m_mountains		= new double[m_num_vertices];
	
	// Initialize the terrain corners
	int corner1 = 0;
	int corner2 = m_dimension - 1;
	int corner3 = m_num_vertices - m_dimension;
	int corner4 = m_num_vertices - 1;
	
	m_mountains[corner1] = 0;
	m_mountains[corner2] = 0;
	m_mountains[corner3] = 0;
	m_mountains[corner4] = 0;

	generate_terrain();
}

void FractalTerrain::render()
{
	if(dl_index == 0)
	{
		// Generate a new display list
		dl_index = glGenLists(1);
		glNewList(dl_index, GL_COMPILE);

		glPushMatrix();

		glTranslatef(-m_dimension/2, 0, 0);
		
		// Set the material for the mountain
		GLfloat mat_diffuse[] = {0.4, 0.4, 0.4};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

		for (int i = 0; i < m_dimension-1; i++)
		{
			for (int j = 0; j < m_dimension-1; j++)
			{
				int p1i = i * m_dimension + j;
				int p2i = i * m_dimension + (j + 1);
				int p3i = (i + 1) * m_dimension + (j + 1);
				int p4i = (i + 1) * m_dimension + j;

				Point3D p1 = Point3D(j, m_mountains[p1i], i);
				Point3D p2 = Point3D(j+1, m_mountains[p2i], i);
				Point3D p3 = Point3D(j+1, m_mountains[p3i], i + 1);
				Point3D p4 = Point3D(j, m_mountains[p4i], i + 1);

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
			}
		}

		glPopMatrix();
		glEndList();

		// No longer need the mountians height data
		delete [] m_mountains;
	}
	else
	{
		glCallList(dl_index);
	}
}

void FractalTerrain::generate_terrain()
{
	int		step	= (m_dimension-1)/2.0;
	double	ratio	= (double) pow(2,-MOUNTAIN_ROUGHNESS);
	double	scale	= HEIGHT_RANGE * ratio;

	for(int i = 0; i < ITERATIONS; i++)
	{
		fractalize(step, scale);

		scale	= scale * ratio;
		step	= step / 2;
	}
}

void FractalTerrain::fractalize(int step, double scale)
{
	for (int i = step; i < (m_dimension-1); i += 2*step)
	{
		for (int j = step; j < (m_dimension-1); j += 2*step)
		{
			m_mountains[(i * m_dimension) + j] = diamond_height(i, j, step, scale);
		}
	}
	
	bool even_column = false;
	
	for (int i = 0; i < m_dimension; i += step)
	{
		even_column = !even_column;

		for (int j = even_column ? step : 0; j < m_dimension; j += 2*step)
		{
			m_mountains[(i * m_dimension) + j] = square_height(i, j, step, scale);
		}
	}
}

double FractalTerrain::diamond_height(int i, int j, int step, double height)
{
	double top_left		= m_mountains[((i - step) * m_dimension) + j - step];
	double top_right	= m_mountains[((i - step) * m_dimension) + j + step];
	double bottom_left	= m_mountains[((i + step) * m_dimension) + j - step];
	double bottom_right	= m_mountains[((i + step) * m_dimension) + j + step];

	double avg = (top_left + top_right + bottom_left + bottom_right) / 4.0;

	return avg + (((double)rand() / (double)RAND_MAX) * height * 2.0 - height);
}

double FractalTerrain::square_height(int i, int j, int step, double height)
{
	int index1 = ((i - step) * m_dimension) + j;
	int index2 = ((i + step) * m_dimension) + j;
	int index3 = (i * m_dimension) + j - step;
	int index4 = (i * m_dimension) + j + step;

	// Adjust the indexes for the edge cases
	if(i == 0)
	{
		index1 = ((m_dimension - 1 - step) * m_dimension) + j;
	}
	else if (i == m_dimension-1)
	{
		index2 = (step * m_dimension) + j;
	}
	else if (j == 0)
	{
		index3 = (i * m_dimension) + m_dimension - 1 - step;
	}
	else if (j == m_dimension-1)
	{
		index4 = (i * m_dimension) + step;
	}

	// Average the 4 points
	double avg = (m_mountains[index1] + m_mountains[index2] + m_mountains[index3] + m_mountains[index4]) / 4.0;

	// Return the average + a random height
	return avg + (((double)rand() / (double)RAND_MAX) * height * 2.0 - height);
}
