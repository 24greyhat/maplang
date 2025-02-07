#ifndef __Interpreter__
#define __Interpreter__

#include "../parser/parser.h"
#include "bytecode.h"
#include "memory.h"
#include <stddef.h>




struct {
  Instruction_t** instructions;
  size_t size; // binary size
} typedef MplBin_t;

MplBin_t* init_mplBin();

void bin_append(MplBin_t* bin, Instruction_t* instruction);

MplBin_t* loadMplBin(void* obj);


struct Interpreter_s {
  AstNode_t* root; // root ast of the parsed program
  Memory_t* memory; // main program memory
  int memory_size;
  Memory_t* mem; // called function memory (used by currently executing functions and holds args as well as any data a function uses)
  int mem_size;
} typedef Interpreter_t;


Interpreter_t* init_interpreter(char* path);


// this function returns the ast node value as a memory objects
MemObject_t* AstToMemObj(AstNode_t* node); 

size_t getMemObjSize(MemObject_t* obj); // returns the size of obj struct type

MplBin_t* compile(AstNode_t* root); // iterate through the root ast node and append to the instructions array

void execute(Interpreter_t* vm, Instruction_t* inst, int functionMode);

void runProgram(Interpreter_t* vm, MplBin_t* bin);


/* 


   step (1): (compilation)

    - convert every ast statement into an instruction.


  step (2): (execution)

    - execute every instruction.


  step (3): (cleanup and exit)

    - free all memory objects.

    - exit swiftly.
  
}*/

#endif
