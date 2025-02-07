#include "../../include/parser/ast.h"
#include <stdlib.h>


AstNode_t* init_ast(int type)
{
  AstNode_t* ast = (AstNode_t*)malloc(sizeof(AstNode_t));
  ast->node_type = type;

  // ast var definition
  ast->var_def_name = (void*)0; // variable definition name
  ast->var_def_value = (void*)0;// variable definition value

  // ast func definition
  ast->func_def_name = (void*)0;
  ast->func_def_body = (void*)0;
  ast->func_def_args = (void*)0;
  ast->func_def_args_size = 0;


  // ast var
  ast->var_name = (void*)0; // variable reference name
  
  // ast func call
  ast->func_call_name = (void*)0;

  ast->func_call_arguments = (void*)0;
  ast->func_call_arg_size = 0;


  // ast_string
  ast->str_value = (void*)0;

  // ast integer
  ast->int_value = 0;

  // ast float
  ast->float_value = 0;

  ast->object_keys = (void*)0;

  ast->object_values = (void*)0;

  ast->object_size = 0; // make sure all fields are initialized


  ast->return_value = (void*)0; // expression

  // ast value and size
  ast->node_value=(void*)0;
  ast->node_size = 0;

  return ast;
}
