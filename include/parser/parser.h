#ifndef __PARSER__
#define __PARSER__

#include "lexer.h"
#include "ast.h"


typedef struct PARSER_S {

  Lexer_t* lexer;

  Token_t* tok; // current token
  Token_t* prev_tok; // previous token
  int ti; // tokens index

} Parser_t;


Parser_t* init_parser(Lexer_t* lexer);


void forward(Parser_t* parser, int token_type);


AstNode_t* parse(Parser_t* parser);  


AstNode_t* parse_statement(Parser_t* parser); // parse a single statement

AstNode_t* parse_statements(Parser_t* parser); // parse all statements


AstNode_t* parse_expr(Parser_t* parser); // parse an expression

AstNode_t* parse_function_def(Parser_t* parser); // parse func def

AstNode_t* parse_function_call(Parser_t* parser); // parse func call

AstNode_t* parse_var(Parser_t* parser); // parse var reference
                                        
AstNode_t* parse_return(Parser_t* parser); // parse function return statement
                                           
AstNode_t* parse_list(Parser_t* parser); // parse list
                                         
AstNode_t* parse_obj(Parser_t* parser); // parse object

AstNode_t* parse_specialChar(Parser_t* parser); // +, -, / etc...

AstNode_t* parse_string(Parser_t* parser);

AstNode_t* parse_float(Parser_t* parser);

AstNode_t* parse_int(Parser_t* parser);

AstNode_t* parse_id(Parser_t* parser); // parse identifier

AstNode_t* parse_var_def(Parser_t* parser);


void printAst(AstNode_t* root);


#endif
