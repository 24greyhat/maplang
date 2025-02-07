#include "../../include/parser/lexer.h"
#include "../../include/stdlib/io.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



Token_t* init_tok(unsigned int type, unsigned int line, char* value)
{
  int len = strlen(value);
  Token_t* tok = (Token_t*)malloc(sizeof(Token_t));

  tok->type = type;
  tok->value = (char*)malloc(sizeof(char)*len);
  tok->value = value;
  tok->line = line;

  return tok;
}



Lexer_t* init_lexer(char* path)
{
  Lexer_t* lexer = (Lexer_t*)malloc(sizeof(Lexer_t));

  lexer->si = 0;
  lexer->ti = 0;
  lexer->line = 1;
  lexer->status_ok = true;

  IoFile_t* io = readFile(path);

  lexer->slen = io->size;
  lexer->source = (char*)malloc(sizeof(char)*lexer->slen);
  lexer->source = io->data;

  free(io);


  lexer->tokens = (Token_t**)malloc(sizeof(Token_t*));

  lexer_next(lexer);

  return lexer;
}



void lexer_next(Lexer_t* lexer)
{
  while(lexer->source[lexer->si] != '\0' && lexer->si < lexer->slen)
  {
    if(isdigit(lexer->source[lexer->si]))
    {
      collect_digit_tok(lexer);
    }

    if(isalnum(lexer->source[lexer->si]))
    {
      collect_id(lexer);
    }

    switch(lexer->source[lexer->si])
    {

      case ' ':
        skip_whitespace(lexer);
        break;

      case '\n':
        skip_whitespace(lexer);
        break;

      case '\t':
        skip_whitespace(lexer);
        break;

      case '"':
        collect_string(lexer, '"');
        break;

      case '\'':
        collect_string(lexer, '\'');
        break;

      case '=':
        collect_char_tok(lexer, eq_tok);
        break;

      case '+':
        collect_char_tok(lexer, plus_tok);
        break;

      case '-':
        collect_char_tok(lexer, minus_tok);
        break;

      case '/': 
        if(lexer->si+1 < lexer->slen && lexer->source[lexer->si+1]=='/')
          skip_comment(lexer);

        else if(lexer->si+1 < lexer->slen && lexer->source[lexer->si+1]=='*')
          skip_multiline_comment(lexer);

        else
          collect_char_tok(lexer, div_tok);
        break;

      case '*':
        collect_char_tok(lexer, mul_tok);
        break;

      case '(':
        collect_char_tok(lexer, lparen_tok);
        break;

      case ')':
        collect_char_tok(lexer, rparen_tok);
        break;

      case '{':
        collect_char_tok(lexer, lbrace_tok);
        break;

      case '}':
        collect_char_tok(lexer, rbrace_tok);
        break;

      case ',':
        collect_char_tok(lexer, coma_tok);
        break;

      case '[':
        collect_char_tok(lexer, lsbrack_tok);
        break;

      case ']':
        collect_char_tok(lexer, rsbrack_tok);
        break;

      case '>':
        collect_char_tok(lexer, gt_tok);
        break;

      case '<':
        collect_char_tok(lexer, lt_tok);
        break;

      case '!':
        collect_char_tok(lexer, not_tok);
        break;
        
      case '~':
        collect_char_tok(lexer, neg_tok);
        break;

      case '&':
        collect_char_tok(lexer, and_tok);
        break;

      case '|':
        collect_char_tok(lexer, or_tok);
        break;

      case '^':
        collect_char_tok(lexer, xor_tok);
        break;

      case ':':
        collect_char_tok(lexer, assign_tok);
        break;

      case ';':
        collect_char_tok(lexer, semi_tok);
        break;

      default:
        advance(lexer);
        break;

    }
  }
}


void advance(Lexer_t* lexer)
{
  if(lexer->source[lexer->si]!='\0' && lexer->si < lexer->slen)
  {
    lexer->si++;
  }
  else{
    lexer_next(lexer);
  }
}


void skip_whitespace(Lexer_t* lexer)
{
  char c;

  while((c=lexer->source[lexer->si]) ==' ' || c=='\n' || c=='\t')
  {
    if(c=='\n')
        lexer->line++;

    advance(lexer);
    c = lexer->source[lexer->si];
  }

  lexer_next(lexer);
}
// \n + \t + whitespace



void skip_comment(Lexer_t* lexer)
{
  while(lexer->source[lexer->si]!='\n')
  {
    advance(lexer);
  }

  lexer_next(lexer);
}


void skip_multiline_comment(Lexer_t* lexer)
{
  /*
   
 */
  advance(lexer); // skip /
  while(lexer->si+1 < lexer->slen)
  {
    if(lexer->source[lexer->si]!='/')
      advance(lexer);
    else if(lexer->source[lexer->si-1]=='*')
    {
      advance(lexer);
      lexer_next(lexer);
    }
    else
      advance(lexer);
  }

}


void collect_string(Lexer_t* lexer, char schar)
{
  advance(lexer); // skip first schar encounter

  char* str = (char*)malloc(sizeof(char));
  int slen = 1;
  int i = 0;

  while(lexer->source[lexer->si]!=schar)
  {
    str = (char*)realloc(str, sizeof(char)*(++slen));
    str[i++] = lexer->source[lexer->si];

    advance(lexer);
  }

  advance(lexer); // skip second schar encounter
  
  str[i]='\0';



  lexer->tokens = (Token_t**)realloc(lexer->tokens, sizeof(Token_t)*lexer->ti+1);
  lexer->tokens[lexer->ti++] = init_tok(str_tok, lexer->line, str);


  lexer_next(lexer);

}// '' or ""


void collect_id(Lexer_t* lexer)
{
  char* id = (char*)malloc(sizeof(char));
  int ilen = 1;
  int i = 0;

  while(isalnum(lexer->source[lexer->si]))
  {
    id = (char*)realloc(id, sizeof(char)*(++ilen));
    id[i++] = lexer->source[lexer->si];

    advance(lexer);
  }
  
  id[i]='\0';

  lexer->tokens = (Token_t**)realloc(lexer->tokens, sizeof(Token_t*)*lexer->ti+1);
  lexer->tokens[lexer->ti++] = init_tok(id_tok, lexer->line, id);

  lexer_next(lexer);

}// identifiers


void collect_char_tok(Lexer_t* lexer, unsigned int type)
{
  lexer->tokens = (Token_t**)realloc(lexer->tokens, sizeof(Token_t*)*lexer->ti+1);

  lexer->tokens[lexer->ti++] = init_tok(type, lexer->line, "");
  advance(lexer);
  

  lexer_next(lexer);
}// +, -, = etc...



void collect_digit_tok(Lexer_t* lexer)
{
  char* digit = (char*)malloc(sizeof(char));
  int dlen = 1;
  int i = 0;

  bool isfloat = false;

  while(isdigit(lexer->source[lexer->si]) || lexer->source[lexer->si]=='.')
  {
    if(lexer->source[lexer->si]=='.')
    {
      isfloat = true;
    }

    digit = (char*)realloc(digit, sizeof(char)*(++dlen));
    digit[i++] = lexer->source[lexer->si];

    advance(lexer);
  }
  
  digit[i]='\0';

  lexer->tokens = (Token_t**)realloc(lexer->tokens, sizeof(Token_t*)*lexer->ti+1);

  if(isfloat)
  {
    lexer->tokens[lexer->ti++] = init_tok(float_tok, lexer->line, digit);
  }
  else{
    lexer->tokens[lexer->ti++] = init_tok(int_tok, lexer->line, digit);
  }

  lexer_next(lexer);
}// ints and floats



const char* getTokenName(int token_type)
{
  switch(token_type)
  {
    case id_tok:
      return "identifier";

    case eq_tok: 
      return "equal";

    case plus_tok:
        return "plus";

    case minus_tok:
        return "minus";

    case div_tok:
        return "divide";

    case mul_tok:
        return "multiply";

    case mod_tok:
        return "modulo";

    case str_tok:
        return "string";

    case lparen_tok:
        return "(";

    case rparen_tok:
        return ")";

    case lbrace_tok:
        return "{";

    case rbrace_tok:
        return "}";

    case coma_tok:
        return ",";

    case int_tok:
        return "integer";

    case float_tok:
        return "float";

    case lsbrack_tok:
        return "[";

    case rsbrack_tok:
        return "]";


    case gt_tok:
        return ">";

    case lt_tok:
        return "<";

    case not_tok:
      return "!";

    case neg_tok:
      return "~";

    case and_tok:
      return "&";

    case or_tok:
      return "|";

    case xor_tok:
      return "^";

    case assign_tok:
      return ":";

    case semi_tok:
      return ";";

    default:
      return "unknown";
  }

}

