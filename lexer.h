#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>

typedef enum {
  BEGINNING,
  END_OF_TOKENS,
  INT,
  FLOAT,
  STRING,
  OPERATOR,
  KEYWORD,
  SEPARATOR,
  COMP,
  IDENTIFIER,
} TokenType;

typedef struct {
  TokenType type;
  char *value;
  size_t line_num;
} Token;


void print_tokens(FILE *file);
Token *generate_num(char *current, int *current_index);
Token *generate_keyword_identifier(char *current, int *current_index);
Token *generate_string_token(char *current, int *current_index);
Token *generate_separator_operator_comp(char *current, int *current_index, TokenType type);
Token *lexer(FILE *file);
void getTokenNum();

#endif