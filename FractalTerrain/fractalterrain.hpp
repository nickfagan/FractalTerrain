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

  Point3D get_world_coords(Point3D p, double local_translate[]);

  void set_mountain_color();

  void free_mountains();

private:
  double *m_mountains;

  int m_num_vertices;
  int m_dimension;

  double m_scale;
  GLuint dl_index = 0;
};

#endif
