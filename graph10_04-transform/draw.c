#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"

/*======== void add_point() ==========
  Inputs:   struct matrix * points
  int x
  int y
  int z 
  Returns: 
  adds point (x, y, z) to points and increment points.lastcol
  if points is full, should call grow on points
  ====================*/
void add_point( struct matrix * points, double x, double y, double z) {
  int new_cols = (points->cols)+1;
  grow_matrix(points, new_cols);
  
  points->m[0][new_cols-1] = x;
  points->m[1][new_cols-1] = y;
  points->m[2][new_cols-1] = z;
  points->m[3][new_cols-1] = 1.0;
}

/*======== void add_edge() ==========
  Inputs:   struct matrix * points
  int x0, int y0, int z0, int x1, int y1, int z1
  Returns: 
  add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
  should use add_point
  ====================*/
void add_edge( struct matrix * points, 
	       double x0, double y0, double z0, 
	       double x1, double y1, double z1) {
  add_point(points, x0, y0, z0);
  add_point(points, x1, y1, z1);
}

/*======== void draw_lines() ==========
  Inputs:   struct matrix * points
  screen s
  color c 
  Returns: 
  Go through points 2 at a time and call draw_line to add that line
  to the screen
  ====================*/
void draw_lines( struct matrix * points, screen s, color c) {
  if(points->cols % 2 == 1) {
    printf("Error: Missing point in edge matrix!\n");
    exit(0);
  }

  int cc;
  for(cc=0; cc<points->cols; cc+=2)    
    draw_line(points->m[0][cc], points->m[1][cc], points->m[0][cc+1], points->m[1][cc+1], s, c);
}



void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
  if((x1 - x0) < 0) {
    int oldx1 = x1, oldy1 = y1;

    x1 = x0;
    y1 = y0;
    x0 = oldx1;
    y0 = oldy1;    
  }
  
  int cy = y1 - y0;
  int cx = x1 - x0;
  int x = x0, y = y0;  
  int a = y1 - y0;
  int b = -(x1 - x0);
  
  int oct;  
  
  if(cy >= 0) {
    if(cy >= cx)
      oct = 2;    
    else if (cy < cx)
      oct = 1;    
  } else {
    if(cy >= -cx)
      oct = 8;
    else if (cy < -cx)
      oct = 7;    
  }

  int d;
  switch(oct) {
  case 1:
    d = 2*a + b;
    while(x <= x1) {
      plot(s, c, x, y);
      if(d > 0) {
	y++;
	d+=2*b;
      }    
      x++;
      d+=2*a;
    }
    break;
  case 2:
    d = a + 2*b;
    while(y <= y1) {
      plot(s, c, x, y);
      if(d < 0) {
	x++;
	d+=2*a;
      }    
      y++;
      d+=2*b;      
    }
    break;
  case 7:
    d = a - 2*b;
    while(y >= y1) {
      plot(s, c, x, y);
      if(d > 0) {
	x++;
	d+=2*a;
      }    
      y--;
      d-=2*b;      
    }
    break;    
  case 8:
    d = 2*a - b;
    while(x <= x1) {
      plot(s, c, x, y);
      if(d < 0) {
	y--;
	d-=2*b;
      }    
      x++;
      d+=2*a;
    }    
    break; 
  }
} //end draw_line
