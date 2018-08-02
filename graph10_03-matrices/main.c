#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"

int main() {
  screen s;
  color c;

  c.red = 230;
  c.green = 210;
  c.blue = 180;
  
  struct matrix *trans; // "transformation" matrix (test)
  struct matrix *edges; // "edge" matrix (test)
  
  trans = new_matrix(4, 0);

  printf("\n<============> Testing add_point <============>\n");
  printf("Matrix 1: (after successive add points)\n");
  add_point(trans, 12, 40, 15);
  print_matrix(trans);  
  add_point(trans, 30, 15, 20);
  print_matrix(trans);
  add_point(trans, 20, 35, 10);
  print_matrix(trans);
  add_point(trans, 10, 30, 20);
  print_matrix(trans);  

  printf("\n<============> Testing add_edge <============>\n");  
  edges = new_matrix(4, 0);
  printf("Matrix 2:\n");
  add_edge(edges, 20, 60, 0, 40, 90, 0);  
  print_matrix(edges);
  add_edge(edges, 40, 90, 0, 130, 280, 0);  
  print_matrix(edges);
  add_edge(edges, 130, 180, 0, 200, 150, 0);  
  print_matrix(edges);
  add_edge(edges, 200, 150, 0, 130, 180, 0);  
  print_matrix(edges);
  add_edge(edges, 130, 180, 0, 20, 60, 0);  
  print_matrix(edges);

  printf("\n<============> Testing matrix_mult <============>\n");
  matrix_mult(trans, edges);
  printf("Matrix 2 = Matrix 1 x Matrix 2\n");
  printf("Matrix 2:\n");
  print_matrix(edges);

  printf("\n<============> Testing ident <============>\n");
  printf("Creating 4x4 matrix, and identing:\n");
  ident(trans);
  print_matrix(trans);

  printf("\n<============> Testing draw_lines <============>\n");

  struct matrix *new_edges = new_matrix(4, 0);
  printf("Created new edge matrix for Saturn!\n");
  
  // edges for saturn
  int sx, sy, ex, ey;
  double amt = 750, inc = 0.1;
  int cx = XRES/2, cy=YRES/2;
  int i;

  // planet
  for(i=0; i<amt/inc; i++) {    
    float mult = 0.45*(amt - i*inc)/amt;
    sx = cx*(1+cos(i*inc)*mult);
    sy = cy*(1+sin(i*inc)*mult);
    ex = cx*(1+cos(i*inc+inc)*mult);
    ey = cy*(1+sin(i*inc+inc)*mult);
    add_edge(new_edges, sx, sy, 0, ex, ey, 0);
  }

  // rings
  for(i=0; i<2000; i++) {    
    sx = cx+(i%100+120)*sin(i);
    sy = cy+(i%100+120)*sin(i+0.5);
    ex = cx+(i%100+120)*sin(i+0.5);
    ey = cy+(i%100+120)*sin(i+1);    
    add_edge(new_edges, sx, sy, 0, ex, ey, 0);
  }

  printf("Drawing Saturn edge matrix...\n");
  draw_lines(new_edges, s, c);  
  
  display(s);
  save_extension(s, "pic.png");
  
  free_matrix( trans );
  free_matrix( edges );
  free_matrix( new_edges );
  printf("Finished!\n");

  return 0;
}
