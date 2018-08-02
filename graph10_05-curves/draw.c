#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "math.h"

/*======== void add_circle() ==========
  Inputs:   struct matrix * points
  double cx
  double cy
  double r
  double step
  Returns:
  Adds the circle at (cx, cy) with radius r to points
  ====================*/
void add_circle( struct matrix * points,
                 double cx, double cy, double cz,
                 double r, double step ) {
  double t;
  double xcor1 = r+cx, ycor1 = cy, zcor1 = cz;
  double xcor2 = r+cx, ycor2 = cy, zcor2 = cz; // for sake of initializing
  
  step *= 100;
  
  for(t=step; t<=100; t+=step) {
    
    // Obtain end points
    xcor2 = r*cos(2*M_PI*t/100.0) + cx;
    ycor2 = r*sin(2*M_PI*t/100.0) + cy;
    
    add_edge(points, xcor1, ycor1, zcor1, xcor2, ycor2, zcor2);

    // Update starting points
    xcor1 = xcor2;
    ycor1 = ycor2;
    zcor1 = zcor2;
  }
}

/*======== void add_curve() ==========
  Inputs:   struct matrix *points
  double x0
  double y0
  double x1
  double y1
  double x2
  double y2
  double x3
  double y3
  double step
  int type
  Returns:
  Adds the curve bounded by the 4 points passsed as parameters
  of type specified in type (see matrix.h for curve type constants)
  to the matrix points
  ====================*/
void add_curve( struct matrix *points, 
                double x0, double y0, 
                double x1, double y1,
                double x2, double y2, 
                double x3, double y3, 
                double step, int type ) {
  struct matrix *xs = generate_curve_coefs(x0, x1, x2, x3, type);
  struct matrix *ys = generate_curve_coefs(y0, y1, y2, y3, type);

  double t;
  double **xm = xs->m, **ym = ys->m;
  double xcor1 = x0, ycor1 = y0;
  double xcor2 = x0, ycor2 = y0;

  step *= 100;
      
  for(t=step; t<=100; t+=step) {
    
    // Obtain end points
    xcor2 = xm[0][0]*pow(t/100.0, 3) + xm[1][0]*pow(t/100.0, 2) + xm[2][0]*(t/100.0) + xm[3][0];
    ycor2 = ym[0][0]*pow(t/100.0, 3) + ym[1][0]*pow(t/100.0, 2) + ym[2][0]*(t/100.0) + ym[3][0];
    add_edge(points, xcor1, ycor1, 0, xcor2, ycor2, 0);

    // Update start points
    xcor1 = xcor2;
    ycor1 = ycor2;
  }  

  free_matrix(xs);
  free_matrix(ys);
}


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
  if ( points->lastcol == points->cols )
    grow_matrix( points, points->lastcol + 100 );
  
  points->m[0][ points->lastcol ] = x;
  points->m[1][ points->lastcol ] = y;
  points->m[2][ points->lastcol ] = z;
  points->m[3][ points->lastcol ] = 1;
  points->lastcol++;
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
