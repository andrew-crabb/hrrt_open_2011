/* static char sccsid[] = "@(#)matpkg.h	1.2 UCL-TOPO 99/08/16"; */
#ifndef matpkg_h_defined
#define matpkg_h_defined
#include <math.h>

typedef struct matrix
	{
	  int ncols, nrows;
	  float *data;
	}
*Matrix;

typedef struct vol3d
	{
	  int xdim, ydim, zdim;
	  float voxel_size;
	  float *data;
	}
*Vol3d;

typedef struct stack3d
	{
	  int xdim, ydim, zdim;
	  float xy_size, z_size;
	  float *data;
	}
*Stack3d;

typedef struct view2d
	{
	  int xdim, ydim;
	  float x_pixel_size, y_pixel_size;
	  float *data;
	}
*View2d;

void matmpy(Matrix res, Matrix a, Matrix b);
void mat_print(Matrix);
void mat_unity(Matrix);
Matrix mat_alloc(int ncols, int nrows);
void mat_copy(Matrix a, Matrix b);
void rotate(Matrix a,float rx, float ry, float rz);
void translate(Matrix a,float tx, float ty, float tz);
void scale(Matrix a,float sx, float sy, float sz);
void mat_apply(Matrix a, float *x1, float *x2);
void mat_free(Matrix);
Vol3d make3d_volume();
Stack3d make3d_stack();

#endif /* matpkg_h_defined */
