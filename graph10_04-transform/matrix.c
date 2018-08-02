#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"


/*======== struct matrix * make_translate() ==========
Inputs:  int x
         int y
         int z 
Returns: The translation matrix created using x, y and z 
as the translation offsets.
====================*/
struct matrix * make_translate(double x, double y, double z) {
  struct matrix *ret_m = new_matrix(4, 4);

  ident(ret_m);

  (ret_m->m)[0][3] = x;
  (ret_m->m)[1][3] = y;
  (ret_m->m)[2][3] = z;
  
  return ret_m;
}

/*======== struct matrix * make_scale() ==========
Inputs:  int x
         int y
         int z 
Returns: The translation matrix creates using x, y and z
as the scale factors
====================*/
struct matrix * make_scale(double x, double y, double z) {
  struct matrix *ret_m = new_matrix(4, 4);

  ident(ret_m);

  (ret_m->m)[0][0] = x;
  (ret_m->m)[1][1] = y;
  (ret_m->m)[2][2] = z;
  (ret_m->m)[3][3] = 1;
  
  return ret_m;
}

/*======== struct matrix * make_rotX() ==========
Inputs:  double theta
Returns: The rotation matrix created using theta as the 
angle of rotation and X as the axis of rotation.
====================*/
struct matrix * make_rotX(double theta) {
  struct matrix *ret_m = new_matrix(4, 4);

  ident(ret_m);

  (ret_m->m)[1][1] = cos(theta);
  (ret_m->m)[1][2] = -1*sin(theta);
  (ret_m->m)[2][1] = sin(theta);
  (ret_m->m)[2][2] = cos(theta);
  
  return ret_m;
}

/*======== struct matrix * make_rotY() ==========
Inputs:  double theta
         char c 
Returns: The rotation matrix created using theta as the 
angle of rotation and Y as the axis of rotation.
====================*/
struct matrix * make_rotY(double theta) {
  struct matrix *ret_m = new_matrix(4, 4);

  ident(ret_m);

  (ret_m->m)[0][0] = cos(theta);
  (ret_m->m)[0][2] = sin(theta);
  (ret_m->m)[2][0] = -1*sin(theta);
  (ret_m->m)[2][2] = cos(theta);
  
  return ret_m;
}

/*======== struct matrix * make_rotZ() ==========
Inputs:  double theta
         char c 
Returns: The rotation matrix created using theta as the 
angle of rotation and Z as the axis of rotation.
====================*/
struct matrix * make_rotZ(double theta) {
  struct matrix *ret_m = new_matrix(4, 4);

  ident(ret_m);

  (ret_m->m)[0][0] = cos(theta);
  (ret_m->m)[0][1] = -1*sin(theta);
  (ret_m->m)[1][0] = sin(theta);
  (ret_m->m)[1][1] = cos(theta);
  
  return ret_m;
}

/*-------------- void print_matrix() --------------
  Inputs:  struct matrix *m 
  Returns: 

  print the matrix
*/
void print_matrix(struct matrix *mat) {
  int r;
  for(r=0; r<mat->rows; r++){

    printf("| ");
    int c;
    for(c=0; c<mat->cols; c++) {
      double val = mat->m[r][c];      

      // Purely for formatting
      int places = 0;
      if(val < -0.00001) {
	if(val >= 10)
	  places = (log(-1*val)/log(10.0))+1;
	else
	  places = 1;	
      } else if (val > 0.00001) {
	if(val >= 10)
	  places = (log(val)/log(10.0));
	else
	  places = 0;
      } else {
	places = 0;
      }

      if(c != mat->cols) {
	int j;
	for(j=0; j<(4-places); j++)
	  printf(" ");
	
	printf("%.2lf", val);
      } else {
	printf("%.2lf\n", val);
      }
    }
    printf(" |\n");
  }

  printf("\n");
}

/*-------------- void ident() --------------
  Inputs:  struct matrix *m <-- assumes m is a square matrix
  Returns: 

  turns m in to an identity matrix
*/
void ident(struct matrix *mat) {
  int r;
  for(r=0; r<mat->rows; r++){

    int c;
    for(c=0; c<mat->cols; c++) {
      if(r == c)
	mat->m[r][c] = 1;
      else
	mat->m[r][c] = 0;
    }    
  }
}


/*-------------- void matrix_mult() --------------
  Inputs:  struct matrix *a
  struct matrix *b 
  Returns: 

  a*b -> b
*/
void matrix_mult(struct matrix *a, struct matrix *b) {
  struct matrix *new_mat = new_matrix(a->rows, b->cols);

  int r;
  for(r=0; r<a->rows; r++) {

    int c; // traverse matrix b by columns first
    for(c=0; c<b->cols; c++) {
      double sum = 0;

      int r2;
      for(r2=0; r2<b->rows; r2++) {
	sum += a->m[r][r2]*b->m[r2][c];
      }
      
      new_mat->m[r][c] = sum;
    }    
  }

  // Iterate through new_mat and reassign to b
  for(r=0; r<new_mat->rows; r++) {
    
    int c;
    for(c=0; c<new_mat->cols; c++) {
      b->m[r][c] = new_mat->m[r][c];
    }
  }

  free_matrix(new_mat);
}



/*===============================================
  These Functions do not need to be modified
  ===============================================*/

/*-------------- struct matrix *new_matrix() --------------
  Inputs:  int rows
  int cols 
  Returns: 

  Once allocated, access the matrix as follows:
  m->m[r][c]=something;
  if (m->lastcol)... 
*/
struct matrix *new_matrix(int rows, int cols) {
  double **tmp;
  int i;
  struct matrix *m;

  tmp = (double **)malloc(rows * sizeof(double *));
  for (i=0;i<rows;i++) {
    tmp[i]=(double *)malloc(cols * sizeof(double));
  }

  m=(struct matrix *)malloc(sizeof(struct matrix));
  m->m=tmp;
  m->rows = rows;
  m->cols = cols;
  m->lastcol = 0;

  return m;
}


/*-------------- void free_matrix() --------------
  Inputs:  struct matrix *m 
  Returns: 

  1. free individual rows
  2. free array holding row pointers
  3. free actual matrix
*/
void free_matrix(struct matrix *m) {

  int i;
  for (i=0;i<m->rows;i++) {
    free(m->m[i]);
  }
  free(m->m);
  free(m);
}


/*======== void grow_matrix() ==========
  Inputs:  struct matrix *m
  int newcols 
  Returns: 

  Reallocates the memory for m->m such that it now has
  newcols number of collumns
  ====================*/
void grow_matrix(struct matrix *m, int newcols) {
  
  int i;
  for (i=0;i<m->rows;i++) {
    m->m[i] = realloc(m->m[i],newcols*sizeof(double));
  }
  m->cols = newcols;
}


/*-------------- void copy_matrix() --------------
  Inputs:  struct matrix *a
  struct matrix *b 
  Returns: 

  copy matrix a to matrix b
*/
void copy_matrix(struct matrix *a, struct matrix *b) {

  int r, c;

  for (r=0; r < a->rows; r++) 
    for (c=0; c < a->cols; c++)  
      b->m[r][c] = a->m[r][c];  
}
