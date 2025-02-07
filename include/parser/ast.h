#ifndef __AST__
#define __AST__

#include <stddef.h>


enum {
  ast_null = 0,
  ast_func_call=100,
  ast_var,
  ast_str,
  ast_int,
  ast_float,
  ast_func_def,
  ast_type,
  ast_list, // []
  ast_object, // {}
  ast_import, // importing files
  ast_return, // func return
  ast_bin_exp, // (left + right) = a binary expression must be inside (), a sub expression is a child of the parent expression

};


typedef struct AstNode_S {

  char* var_def_name; // variable definition name
  struct AstNode_S* var_def_value; // variable definition value


  // ast func definition
  char* func_def_name;
  struct AstNode_S** func_def_body; // body 
  size_t func_def_body_size; // body size
  struct AstNode_S** func_def_args;
  size_t func_def_args_size;


  // ast var
  char* var_name; // referenced variable name
  

  // ast func call
  char* func_call_name;
  struct AstNode_S** func_call_arguments;
  size_t func_call_arg_size;


  // ast_string
  char* str_value;

  // ast integer
  long long int int_value;

  // ast float
  long double float_value;


  // ast object keys and values
  char** object_keys; 
  struct AstNode_S** object_values;
  size_t object_size;

  // ast list values
  struct AstNode_S** list_body;
  size_t list_size;



  // the ast of the imported module (imports can occur anywhere in the file (the imported code is reachable only following the import statement))
  struct AstNode_S** import_ast; 
  char* import_path; // the path of the imported module


  // function return
  struct AstNode_S* return_value; // the value of the function return is an expression

  // binary expression
  struct AstNode_S* bexp_left; // (left expType right)
  struct AstNode_S* bexp_right; // (left expType right)
  int bexp_op; // plus_tok, minus_tok, div_tok etc...
  // example expression: (100 + 2000)
  // an expression can also have children: (100+(200-(4+5))/2) 


  // ast node
  struct AstNode_S** node_value;
  size_t node_size;
  unsigned int node_type;


} AstNode_t;


AstNode_t* init_ast(int type);


#endif
