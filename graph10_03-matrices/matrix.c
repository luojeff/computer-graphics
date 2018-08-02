#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"


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

      // Purely for formatting matrix columns
      int places;
      if(val < 0)
	places = log(-1*val)/log(10.0)+1;
      else if (val > 0)
	places = log(val)/log(10.0);
      else // val == 0
	places = 0;
      
      if(c != mat->cols) {
	int j;
	for(j=0; j<(3-places); j++)
	  printf(" ");
	
	printf("%.2lf ", val);
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
