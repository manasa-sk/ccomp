#ifndef PARSER_H_
#define PARSER_H_
#include "lexer.h"

#define MAX_CURLY_STACK_LENGTH 128

typedef struct Node {
    char *value;
    TokenType type;
    struct Node *right;
    struct Node *left;
} Node;

typedef struct {
    Node *content[MAX_CURLY_STACK_LENGTH];
    int top;
} curly_stack;

void print_tree(Node *node, int indent, char *identifier);
Node *parse_expression(Token *current_token, Node *current_node);
Token *generate_operation_nodes(Token *current_token, Node *current_node);
Node *handle_exit_syscall(Node *root, Token *current_token, Node *current);
void handle_token_errors(char *error_text, Token *current_token, TokenType type);
Node *create_variable_reusage(Token *current_token, Node *current);
Node *create_variables(Token *current_token, Node *current);
Token *generate_if_operation_nodes(Token *current_token, Node *current_node);
Token *generate_if_operation_nodes_right(Token *current_token, Node *current_node);
Node *create_if_statement(Token *current_token, Node *current);
Node *handle_write_node(Token *current_token, Node *current);
Node *parser(Token *tokens);

#endif