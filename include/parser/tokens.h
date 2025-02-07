#ifndef __TOKENS__
#define __TOKENS__

enum {
  id_tok = 1, // identifiers: print, include, etc...
  eq_tok, // =
  plus_tok, // +
  minus_tok,// -
  div_tok, // /
  mul_tok, // *
  mod_tok, // %
  str_tok, // '' ""
  lparen_tok, // (
  rparen_tok, // )
  lbrace_tok, // {
  rbrace_tok, // }
  coma_tok, // ,
  int_tok, // 0-9 int
  float_tok, // 1.22 float
  lsbrack_tok, // [
  rsbrack_tok, // ]
  gt_tok, // > 
  lt_tok, // <
  not_tok, // !
  neg_tok, // ~
  and_tok, // &
  or_tok, // |
  xor_tok, // ^ = interpreted as sqr in a binary expression
  assign_tok, // :
  semi_tok, // ;
};


typedef struct {
  unsigned int line;
  unsigned int type;
  char* value;
} Token_t;


Token_t* init_tok(unsigned int type, unsigned int line, char* value);


const char* getTokenName(int token_type);

#endif
