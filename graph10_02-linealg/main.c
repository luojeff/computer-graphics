#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"

#define AMT 200
#define INC 0.1

int main() {
  screen s;
  color c;
  
  clear_screen(s);

  /*
  // Code for spiral

  int mc = MAX_COLOR/2.0;
  int i;

  for(i=0; i<AMT/INC; i+=1) {
    float mult = 0.8*(AMT - i*INC)/AMT;
    
    int sx = (XRES/2)*(1+cos(i*INC)*mult);
    int sy = (YRES/2)*(1+sin(i*INC)*mult);
    int ex = (XRES/2)*(1+cos(i*INC+INC)*mult);
    int ey = (YRES/2)*(1+sin(i*INC+INC)*mult);

    c.red = mc + 100*sin(INC*i);
    c.green = mc + 100*sin(INC*i+1);
    c.blue = mc + 100*sin(INC*i+2);
    
    draw_line(sx, sy, ex, ey, s, c);
  }
  */

  c.red = 0;
  c.green = MAX_COLOR;
  c.blue = 0;

  // All test cases
  draw_line(250, 250, 250, 400, s, c); // horizontal right
  draw_line(250, 250, 250, 50, s, c); // horizontal left
  draw_line(250, 250, 400, 250, s, c); // vertical up
  draw_line(250, 250, 100, 250, s, c); // vertical down	    
  draw_line(250, 250, 350, 300, s, c); // oct 1
  draw_line(250, 250, 300, 350, s, c); // oct 2
  draw_line(250, 250, 180, 400, s, c); // oct 3
  draw_line(250, 250, 100, 325, s, c); // oct 4
  draw_line(250, 250, 100, 175, s, c); // oct 5
  draw_line(250, 250, 130, 100, s, c); // oct 6
  draw_line(250, 250, 300, 150, s, c); // oct 7
  draw_line(250, 250, 350, 190, s, c); // oct 8

  display(s);
  save_extension(s, "lines.png");
}
