/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser,
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  push: push a new origin matrix onto the origin stack

  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix
                     based on the provided values, then
                     multiply the current top of the
                     origins stack by it.

  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a
                    temporary matrix, multiply it by the
                    current top of the origins stack, then
                    call draw_polygons.

  line: create a line based on the provided values. Store
        that in a temporary matrix, multiply it by the
        current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the screen
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"
#include "gmath.h"

void my_main() {

  struct matrix *tmp;
  struct stack *systems;
  screen t;
  zbuffer zb;
  color g;
  double step_3d = 20;
  double theta;

  //Lighting values here for easy access
  color ambient;
  double light[2][3];
  double view[3];
  double areflect[3];
  double dreflect[3];
  double sreflect[3];

  ambient.red = 50;
  ambient.green = 50;
  ambient.blue = 50;

  light[LOCATION][0] = 0.5;
  light[LOCATION][1] = 0.75;
  light[LOCATION][2] = 1;

  light[COLOR][RED] = 0;
  light[COLOR][GREEN] = 255;
  light[COLOR][BLUE] = 255;

  view[0] = 0;
  view[1] = 0;
  view[2] = 1;

  areflect[RED] = 0.1;
  areflect[GREEN] = 0.1;
  areflect[BLUE] = 0.1;

  dreflect[RED] = 0.5;
  dreflect[GREEN] = 0.5;
  dreflect[BLUE] = 0.5;

  sreflect[RED] = 0.5;
  sreflect[GREEN] = 0.5;
  sreflect[BLUE] = 0.5;

  systems = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen(t);
  clear_zbuffer(zb);
  g.red = 0;
  g.green = 0;
  g.blue = 0;

  struct command curr;
  double *args, *args2;
  
  // Iterate through all commands
  int i;
  for(i=0; i<lastop; i++) {
    curr = op[i];
   
    if(curr.opcode == PUSH) {
      push(systems);
      
    } else if(curr.opcode == POP) {
      pop(systems);
      
    } else if(curr.opcode == MOVE) {
      args = curr.op.move.d;
      tmp = make_translate(args[0], args[1], args[2]);
      matrix_mult(peek(systems), tmp);
      copy_matrix(tmp, peek(systems));
      tmp->lastcol = 0;
      
    } else if(curr.opcode == ROTATE) {
      double axis;
      axis = curr.op.rotate.axis;
      theta = curr.op.rotate.degrees * (M_PI / 180);
      
      if(axis == 0)
        tmp = make_rotX(theta);
      else if(axis == 1)
        tmp = make_rotY(theta);
      else
        tmp = make_rotZ(theta);
      matrix_mult(peek(systems), tmp);
      copy_matrix(tmp, peek(systems));
      tmp->lastcol = 0;
      
    } else if(curr.opcode == SCALE) {
      args = curr.op.scale.d;
      
      tmp = make_scale(args[0], args[1], args[2]);
      matrix_mult(peek(systems), tmp);
      copy_matrix(tmp, peek(systems));
      tmp->lastcol = 0;
      
    } else if(curr.opcode == BOX) {
      args = curr.op.box.d0;
      args2 = curr.op.box.d1;
      
      add_box(tmp, args[0], args[1], args[2],
              args2[0], args2[1], args2[2]);
      matrix_mult(peek(systems), tmp);
      draw_polygons(tmp, t, zb,
                    view, light, ambient, areflect, dreflect, sreflect);
      tmp->lastcol = 0;
      
    } else if(curr.opcode == SPHERE) {
      double r;
      args = curr.op.sphere.d;
      r = curr.op.sphere.r;
      
      add_sphere(tmp, args[0], args[1], args[2], r, step_3d);
      matrix_mult(peek(systems), tmp);
      draw_polygons(tmp, t, zb,
                    view, light, ambient, areflect, dreflect, sreflect);
      tmp->lastcol = 0;
      
    } else if(curr.opcode == TORUS) {
      double r0, r1;
      args = curr.op.torus.d;
      r0 = curr.op.torus.r0;
      r1 = curr.op.torus.r1;
      
      add_torus(tmp, args[0], args[1], args[2], r0, r1, step_3d);
      matrix_mult(peek(systems), tmp);
      draw_polygons(tmp, t, zb,
                    view, light, ambient, areflect, dreflect, sreflect);
      tmp->lastcol = 0;
      
    } else if(curr.opcode == LINE) {
      args = curr.op.line.p0;
      args2 = curr.op.line.p1;
      
      add_edge(tmp, args[0], args[1], args[2],
	       args2[0], args2[1], args2[2]);
      matrix_mult(peek(systems), tmp);
      draw_lines(tmp, t, zb, g);
      tmp->lastcol = 0;
      
    } else if(curr.opcode == SAVE) {
      char *ext = curr.op.save.p->name;
      save_extension(t, ext);
      
    } else if(curr.opcode == DISPLAY) {
      display(t);
      
    } 
  }  
}
