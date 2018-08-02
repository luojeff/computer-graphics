#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename 
  struct matrix * transform, 
  struct matrix * pm,
  screen s
  Returns: 
  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix - 
  takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
  ident: set the transform matrix to the identity matrix - 
  scale: create a scale matrix, 
  then multiply the transform matrix by the scale matrix - 
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix, 
  then multiply the transform matrix by the translation matrix - 
  takes 3 arguments (tx, ty, tz)
  rotate: create an rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 2 arguments (axis, theta) axis should be x y or z
  apply: apply the current transformation matrix to the 
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing
  See the file script for an example of the file format
  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);

    if(strcmp(line, "line") == 0) {
      fgets(line, 255, f);

      char **points = parse_input(line, " ");
      
      add_edge(edges, atof(points[0]), atof(points[1]), atof(points[2]), atof(points[3]), atof(points[4]), atof(points[5]));
      
      free(points);      
    } else if(strcmp(line, "ident") == 0) {
      ident(transform);

    } else if(strcmp(line, "scale") == 0) {      
      fgets(line, 255, f);
      
      char **s_fs = parse_input(line, " ");
      struct matrix *transform_m = make_scale(atof(s_fs[0]), atof(s_fs[1]), atof(s_fs[2]));
      
      matrix_mult(transform_m, transform);

      free(transform_m);
      free(s_fs);
    } else if(strcmp(line, "move") == 0) {
      fgets(line, 255, f);
      
      char **s_fs = parse_input(line, " ");
      struct matrix *transform_m = make_translate(atof(s_fs[0]), atof(s_fs[1]), atof(s_fs[2]));
      
      matrix_mult(transform_m, transform);

      free(transform_m);
      free(s_fs);
    } else if(strcmp(line, "rotate") == 0) {
      fgets(line, 255, f);
      
      char **args = parse_input(line, " ");
      struct matrix *transform_m;

      switch(args[0][0]) {
      case 'x':
	transform_m = make_rotX(atof(args[1]) * M_PI/180);
	break;
      case 'y':
	transform_m = make_rotY(atof(args[1]) * M_PI/180);
	break;
      case 'z':
	transform_m = make_rotZ(atof(args[1]) * M_PI/180);
	break;
      }

      matrix_mult(transform_m, transform);
      
      free(transform_m);
      free(args);
    } else if(strcmp(line, "apply") == 0) {      
      matrix_mult(transform, edges);
    } else if(strcmp(line, "display") == 0) {
      color c;
      c.red = 155;
      c.green = 155;
      c.blue = 0;

      clear_screen(s);
      draw_lines(edges, s, c);
      display(s);
    } else if(strcmp(line, "save") == 0) {
      save_extension(s, "pic.png");
    }
  }
}

/*
  Parses string line by delimiter, returning an array to pointers.
*/
char **parse_input(char *line, char *delimiter){
  int i=0;
  char *curr_arg = NULL;
  char **ret_args = (char **)malloc(sizeof(char *) * (count_occur(line, delimiter)+2));
  
  while((curr_arg = strsep(&line, delimiter))){
    ret_args[i] = curr_arg;
    i++;
  }

  ret_args[i] = NULL;

  return ret_args;
}

/* 
   Returns the number of occurrences of substring occ in string s
*/
int count_occur(char *s, char *occ){
  int count = 0;
  char *rem = s;

  while((rem = strstr(rem, occ))) {
    count++;
    rem++;
  }

  return count;
}
