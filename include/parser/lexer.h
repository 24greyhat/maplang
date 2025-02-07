#ifndef __LEXER__
#define __LEXER__

#include "tokens.h"
#include <stdbool.h>


typedef struct {
  Token_t** tokens;
  unsigned int ti; // tokens index
  unsigned int si; // source index
  char* source;
  int line;
  unsigned int slen; // source size 
  bool status_ok;
} Lexer_t;


Lexer_t* init_lexer(char* path);


void advance(Lexer_t* lexer); // si++

void skip_whitespace(Lexer_t* lexer); // \n + \t + whitespace

void skip_comment(Lexer_t* lexer); // //

void skip_multiline_comment(Lexer_t* lexer); // /* */

void collect_string(Lexer_t* lexer, char schar); // schar= '' or ""

void collect_id(Lexer_t* lexer); // identifiers

void collect_char_tok(Lexer_t* lexer, unsigned int type); // +, -, = etc...

void collect_digit_tok(Lexer_t* lexer); // ints and floats

void lexer_next(Lexer_t* lexer); // fetch next token




#endif
