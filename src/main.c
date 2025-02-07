#include "../include/interpreter/interpreter.h"


int main(void)
{

  Interpreter_t* mpl = init_interpreter("./examples/test.mpl");

  MplBin_t* program = compile(mpl->root);

  runProgram(mpl, program);

  return 0;
}
