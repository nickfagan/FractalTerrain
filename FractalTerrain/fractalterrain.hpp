#ifndef FRACTAL_TERRAIN_HPP
#define FRACTAL_TERRAIN_HPP

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "algebra.hpp"

static const int ITERATIONS = 10;
static const double MOUNTAIN_ROUGHNESS = .98;
static const double HEIGHT_RANGE = 100;

class FractalTerrain {
public:
	void initialize();
	void render();

private:
	
	double * mountains;
	int num_vertices;
	int dimension;
	GLuint dl_index = 0;
	double bottomVal;
	
	void generate_terrain();
	void fractalize(int step, double scale);
	double square_height(int i, int j, int step, double scale);
	double diamond_height(int i, int j, int step, double scale);
};

#endif
