#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "lexer.h"
#include "parser.h"

int main(){
    FILE *file = fopen("test.unn", "r");
    Token *tokens = lexer(file);
    Node *rootAst = parser(tokens);
    return 0;
}