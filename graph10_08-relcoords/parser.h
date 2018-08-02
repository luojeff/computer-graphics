#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "ml6.h"
#include "stack.h"

void parse_file ( char * filename,
                  struct matrix * edges,
                  struct matrix * polygons,
		  struct stack * stk,
                  screen s);

#endif
