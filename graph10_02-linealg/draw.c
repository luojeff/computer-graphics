#include <stdio.h>
#include <stdlib.h>
#include "ml6.h"
#include "display.h"
#include "draw.h"

//Insert your line algorithm here
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
}
