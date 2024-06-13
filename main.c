#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "lexer.h"

int main(){
    FILE *file = fopen("test.unn", "r");
    print_tokens(file);
    getTokenNum();
    return 0;
}