#ifndef __MPL_BRIDGE__
#define __MPL_BRIDGE__

#include "../interpreter/memory.h"

// creating a bridge between the stdlib c functions and the interpreter, so the interpreter can essentially call stdlib functions via the callInstruction


#define FUNCS 10

struct {
  MemObject_t** args;
  int count; // args count
} typedef Args_t;


struct StdFunction_s {
  char* name;
  MemObject_t func(Args_t*); // pointer to the stdlib function
} typedef StdFunction_t;


struct {
  StdFunction_t functions[FUNCS]; // all the functions in the standard library
  int size; // size of the functions array of this bridge between the interpreter and the stdlib in c
} typedef Bridge_t;


Bridge_t* init_bridge();

MemObject_t call_func(char* name, Args_t args);


// 1 = error; 0 = registered successfully
int registerFunction(Bridge_t* bridge, const char* functionName, MemObject_t function(Args_t*));



// NOTE: this bridge can be used to add more functions to the standard library of the language
// make a function in c that takes Args_t as it's only argument and MemObject_t as it's return type.


#endif
