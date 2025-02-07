#ifndef __EXCEPTIONS__
#define __EXCEPTIONS__


#include "interpreter/memory.h"
#include "parser/ast.h"


void raiseBinExpException(MemObject_t* left, int op, MemObject_t* right);

void raiseUnexpectedToken(int token_type); // unexpected token when parsing and calling forward


#endif
