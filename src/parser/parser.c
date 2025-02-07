#include "../../include/parser/parser.h"
#include "../../include/parser/types.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





Parser_t* init_parser(Lexer_t* lexer)
{
  Parser_t* parser = (Parser_t*)malloc(sizeof(Parser_t));

  parser->lexer = lexer;
  parser->ti = 0;

  // current token
  parser->prev_tok = parser->tok;
  parser->tok = parser->lexer->tokens[parser->ti++];




  return parser;
}


void forward(Parser_t* parser, int token_type)
{

  if(parser->ti==parser->lexer->ti)
    return (void)NULL;

  // check if token_type is equal to current token type
  // token_type is what we expect and the current type is what we get
  if(parser->tok->type == token_type)
  {
    parser->prev_tok = parser->lexer->tokens[parser->ti-1];
    parser->tok = parser->lexer->tokens[parser->ti++];
  }
  else{
    printf("woops expected: %s, got: %s, value: %s\n", getTokenName(token_type), getTokenName(parser->tok->type), parser->tok->value);
    // handle unexpected token type: parser error
    exit(1);
    // token is not what we expect it to be.
  }
}


AstNode_t* parse(Parser_t* parser)
{
  return parse_statements(parser);
}



AstNode_t* parse_statement(Parser_t* parser)
{
  // each statement is an expression
  return parse_expr(parser);
}


AstNode_t* parse_statements(Parser_t* parser)
{

  // ast holds our array of ast nodes
  AstNode_t* ast = init_ast(ast_type);


  ast->node_value = (AstNode_t**)malloc(sizeof(AstNode_t*));


  AstNode_t* ast_statement = parse_statement(parser); // parse the first statement

  ast->node_size += 1;

  ast->node_value[0] = ast_statement; // assign first statement to the index 0 of the compound array of statements
  

  // semicolone indicates the end of a statement
  // while we have semi colons parse next statement
  while(parser->tok->type == semi_tok)
  {
    if(parser->ti==parser->lexer->ti)
    {
      return ast;
    }

    forward(parser, semi_tok); // forward to next token, tokens index is incremented (ti++)

    AstNode_t* ast_statement = parse_statement(parser); // parse current statement
    
    ast->node_size+=1;

    ast->node_value = realloc(ast->node_value, ast->node_size*sizeof(AstNode_t*)); // reallocate memory to the list of statements

    ast->node_value[ast->node_size-1] = ast_statement;  

  }


  return ast;
}




AstNode_t* parse_function_call(Parser_t* parser)
{
  AstNode_t* func_call = init_ast(ast_func_call);

  func_call->func_call_name = parser->prev_tok->value;

  forward(parser, lparen_tok); // skip lparen


  func_call->func_call_arguments = malloc(sizeof(AstNode_t*));

  AstNode_t* ast_expr = parse_expr(parser);

  func_call->func_call_arguments = malloc(sizeof(AstNode_t*));
  func_call->func_call_arg_size += 1;
  func_call->func_call_arguments[func_call->func_call_arg_size-1] = ast_expr;

  while(parser->tok->type == coma_tok)
  {
    forward(parser, coma_tok);

    AstNode_t* ast_expr = parse_expr(parser); // parse arg

    func_call->func_call_arg_size += 1;

    func_call->func_call_arguments = realloc(func_call->func_call_arguments, func_call->func_call_arg_size*sizeof(AstNode_t*));

    func_call->func_call_arguments[func_call->func_call_arg_size-1] = ast_expr;  

    func_call->func_call_arg_size++;

  }

  forward(parser, rparen_tok); // skip right parenthesis

  return func_call;
}


AstNode_t* parse_function_def(Parser_t* parser)
{
  AstNode_t* ast = init_ast(ast_func_def);

  forward(parser, id_tok); // function

  char* function_name = parser->tok->value;

  ast->func_def_name = malloc(sizeof(char)*strlen(function_name));

  strcpy(ast->func_def_name, function_name);

  forward(parser, id_tok);

  forward(parser, lparen_tok);

  ast->func_def_args = malloc(sizeof(AstNode_t*));

  AstNode_t* arg = parse_var(parser);

  ast->func_def_args_size += 1;

  ast->func_def_args[ast->func_def_args_size-1] = arg;

  while (parser->tok->type == coma_tok){
    forward(parser, coma_tok);

    ast->func_def_args_size += 1;

    ast->func_def_args = realloc(ast->func_def_args, ast->func_def_args_size * sizeof(struct AstNode_t*));

    ast->func_def_args[ast->func_def_args_size-1] = parse_id(parser);

  }


  forward(parser, rparen_tok); // )

  forward(parser, lbrace_tok); // {

  AstNode_t* ast_statement = parse_statement(parser); // parse the first statement
  ast->func_def_body_size += 1;
  ast->func_def_body = malloc(sizeof(AstNode_t*));
  ast->func_def_body[ast->func_def_body_size-1] = ast_statement;

  while(parser->tok->type==semi_tok) // while the current token isn't } keep parsing the function body
  {
    forward(parser, semi_tok);

    AstNode_t* ast_statement = parse_statement(parser); // parse the first statement
    
    ast->func_def_body_size += 1;
    ast->func_def_body = realloc(ast->func_def_body, sizeof(AstNode_t*)*ast->func_def_body_size);
    ast->func_def_body[ast->func_def_body_size-1] = ast_statement;
  }

  forward(parser, rbrace_tok); // }

  return ast;
}




AstNode_t* parse_id(Parser_t* parser)
{
  // identify the type of identifier and parse accordingly
  
  if(strcmp(parser->tok->value, VAR_DEF_KEYWORD) == 0)
  {
    return parse_var_def(parser);
  }

  else if(strcmp(parser->tok->value, FUNCTION_DEF_KEYWORD) == 0)
  {
    return parse_function_def(parser);
  }

  else if(strcmp(parser->tok->value, RETURN_KEYWORD) == 0)
  {
    return parse_return(parser);
  }

  else{
    return parse_var(parser); // parse_var will try to first parse tokens as var if there are parenthesis it will deviert to parse_func();
  }
}



AstNode_t* parse_var_def(Parser_t* parser)
{
  forward(parser, id_tok); // expected var identifier
  //
  char* var_name = parser->tok->value; // get the value of the current token which is the var name

  forward(parser, id_tok); // expected variable name
  forward(parser, eq_tok); // expected =

  AstNode_t* var_value = parse_expr(parser); // expected var value is an expr

  // creating the variable definition ast node
  AstNode_t* var_def = init_ast(ast_var);

  var_def->var_def_name = malloc(sizeof(char)*strlen(var_name));
  var_def->var_def_name = var_name;

  var_def->var_def_value = malloc(sizeof(AstNode_t*)); 
  var_def->var_def_value = var_value; 

  return var_def;
}



AstNode_t* parse_var(Parser_t* parser)
{
  char* value = parser->tok->value; // save value of current token before forwarding to next token
  // value is the name of the variable

  forward(parser, id_tok); // move forward from id_tok to next one

  // if current tok is ( then parse a function call
  if(parser->tok->type == lparen_tok)
  {
    return parse_function_call(parser);
  }

  else if(parser->tok->type == eq_tok)
  {
    forward(parser, eq_tok);

    AstNode_t* ast_variable = init_ast(ast_var);

    ast_variable->var_name = malloc(sizeof(char)*strlen(value)); 
    ast_variable->var_name = value;

    return ast_variable;
  }

  AstNode_t* ast_variable = init_ast(ast_var);

  ast_variable->var_name = malloc(sizeof(char)*strlen(value));
  ast_variable->var_name = value;

  return ast_variable;
}


AstNode_t* parse_return(Parser_t* parser)
{
  forward(parser, id_tok); // move forward from "return" to next tok

  AstNode_t* ast_ret = init_ast(ast_return);

  ast_ret->return_value = parse_expr(parser);

  return ast_ret;
}


AstNode_t* parse_list(Parser_t* parser)
{
  AstNode_t* ast = init_ast(ast_list);

  forward(parser, lsbrack_tok); // skip [

  ast->list_body = malloc(sizeof(AstNode_t*));

  AstNode_t* value = parse_expr(parser);

  ast->list_size += 1;

  ast->list_body[ast->list_size-1] = value;

  while (parser->tok->type == coma_tok){
    forward(parser, coma_tok); // skip ,

    ast->list_size += 1;

    ast->list_body = realloc(ast->list_body, ast->list_size * sizeof(struct AstNode_t*));

    ast->list_body[ast->list_size-1] = parse_expr(parser);

  }

  forward(parser, rsbrack_tok); // skip ]

  return ast;
}


// [.] Implementing Object 
AstNode_t* parse_obj(Parser_t* parser)
{
  AstNode_t* ast = init_ast(ast_object);

  forward(parser, lbrace_tok); // skip {


  AstNode_t* key = parse_string(parser);

  forward(parser, assign_tok);

  AstNode_t* value = parse_expr(parser);
  ast->object_size += 1;


  ast->object_keys = malloc(sizeof(char*)*ast->object_size);
  ast->object_keys[ast->object_size-1] = malloc(sizeof(char)*strlen(key->str_value));
  ast->object_keys[ast->object_size-1] = key->str_value;

  ast->object_values = malloc(sizeof(struct AstNode_t*));
  ast->object_values[ast->object_size-1] = value;


  while (parser->tok->type == coma_tok){
    forward(parser, coma_tok); // skip the ,

    ast->object_size += 1;
    
    AstNode_t* key = parse_string(parser);

    forward(parser, assign_tok);

    ast->object_keys = realloc(ast->object_keys, sizeof(char*)*ast->object_size);
    ast->object_keys[ast->object_size-1] = malloc(sizeof(char)*strlen(key->str_value));
    ast->object_keys[ast->object_size-1] = key->str_value;

    ast->object_values = realloc(ast->object_values, ast->object_size * sizeof(struct AstNode_t*));
    ast->object_values[ast->object_size-1] = parse_expr(parser);
  }

  forward(parser, rbrace_tok); // skip }


  return ast;
}



AstNode_t* parse_string(Parser_t* parser)
{
  AstNode_t* ast_string = init_ast(ast_str);
  ast_string->str_value = malloc(sizeof(char)*strlen(parser->tok->value));
  ast_string->str_value = parser->tok->value;

  forward(parser, str_tok);

  return ast_string;
}


AstNode_t* parse_int(Parser_t* parser)
{
  AstNode_t* ast_integer = init_ast(ast_int);
  char*p;

  ast_integer->int_value = strtoll(parser->tok->value, &p, 10);

  if(ast_integer->int_value == LONG_MAX || ast_integer->int_value == LONG_MIN)
  {
    // raiseError(range_error, parser);
    exit(1);
  }

  forward(parser, int_tok);

  return ast_integer;
}


AstNode_t* parse_float(Parser_t* parser)
{
  AstNode_t* ast_f = init_ast(ast_float);

  char*p;

  ast_f->float_value = strtold(parser->tok->value, &p);

  forward(parser, float_tok);

  return ast_f;
}


AstNode_t* parse_binExp(Parser_t* parser)
{
  AstNode_t* ast = init_ast(ast_bin_exp);

  forward(parser, lparen_tok);

  // (left operator right)

  ast->bexp_left = parse_expr(parser);

  ast->bexp_op = parser->tok->type;

  forward(parser, parser->tok->type);

  ast->bexp_right = parse_expr(parser);

  forward(parser, rparen_tok);

  return ast;
}



AstNode_t* parse_expr(Parser_t* parser)
{
  switch(parser->tok->type)
  {
    case str_tok:
      return parse_string(parser);

    case int_tok:
      return parse_int(parser);

    case float_tok:
      return parse_float(parser);

    case id_tok:
      return parse_id(parser);

    case lsbrack_tok:
      return parse_list(parser);

    case lbrace_tok:
      return parse_obj(parser);

    case lparen_tok:
      return parse_binExp(parser);

    default:
      return (AstNode_t*)NULL;
      // return raiseException(parser, unexpected_token_exception);
  }
}



const char* getNodeName(int nodeType) {
  switch(nodeType){

    case ast_type:
      return "root";

    case ast_func_def:
      return "function definition";

    case ast_func_call:
      return "function call";

    case ast_object:
      return "object";

    case ast_list: return "list";

    case ast_return: return "return";

    case ast_var: return "variable";

    case ast_str: return "string";

    case ast_int: return "integer";

    case ast_float: return "float";


    default: return "unknown";
  }
}



void printAst(AstNode_t* root)
{
  AstNode_t* p = malloc(sizeof(AstNode_t*)); // temporary pointer
  
  int size = 0;
  
  while(root->node_size-1 > size)
  {
    p = root->node_value[size++];

    printf("node_type: %s\n", getNodeName(p->node_type));

  }
}
