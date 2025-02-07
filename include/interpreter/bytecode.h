#ifndef __BYTECODE__
#define __BYTECODE__

#include "memory.h"
#include <stddef.h>


enum Bytecodes {
  push_i, // push a memory object = push(void* object, int type)
  pop_i, // pop a memory object = pop()
  add_i, // add the last 2 memory objects and push results = add()
  div_i, // divide the last 2 memory objects and push results = div()
  sub_i, // subtract... = sub()
  modulo_i, // % = modulo()
  mul_i, // * = multiply()
  callFunc_i, // call a function and push return value = callFunc(string functionName, void* object, int type) (the object is a funcArgs memory object)
  getVar_i, // get a variable's value and push it onto the stack = getVar(string varName)
};


struct {
  int bytecode; // push_i
  MemObject_t* argument; // instruction args is a memory object
} typedef Instruction_t;


Instruction_t* inst_i(int bytecode); // insts with no args
Instruction_t* inst(int bytecode, MemObject_t* argument); // insts with args




/*
   ------- NOTES ------


   callFunc instruction will check if the functionName corresponds with a builtin function first, if so it will call the builtin function.

   the builtin functions are defined in the stdlib and bridge.h.


}*/

#endif
