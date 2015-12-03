#ifndef FRACTAL_TERRAIN_HPP
#define FRACTAL_TERRAIN_HPP

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "algebra.hpp"

class FractalTerrain {
public:
	void initialize();
	void render();

private:
	void generate_terrain();
	void fractalize(int step, double scale);
	double square_height(int i, int j, int step, double scale);
	double diamond_height(int i, int j, int step, double scale);
	void free_mountains();
private:
	double * m_mountains;

	int m_num_vertices;
	int m_dimension;

	GLuint dl_index = 0;
};

#endif
