#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

char* keywords[] = { "exit", "int", "float", "if", "else", "for", "print"};
char comp_chars[] = {'<', '>', '!', 'e'};
char seperators[] = {';', '(', ')', '{', '}', ','};
char operators[] = {'+', '-', '=', '*', '/', '%'};

size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);
size_t num_comp_chars = sizeof(comp_chars) / sizeof(comp_chars[0]);
size_t num_seperators = sizeof(seperators) / sizeof(seperators[0]);
size_t num_operators = sizeof(operators) / sizeof(operators[0]);
size_t line_number=0;
size_t tokens_index;

Token *generate_num(char *current, int *current_index){
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    token->type = INT;
    char *value = malloc(sizeof(char) * 8);
    int value_index = 0;
    while((isdigit(current[*current_index]) || current[*current_index]=='.') && current[*current_index] != '\0'){
        if(current[*current_index]=='.') token->type = FLOAT;
        value[value_index] = current[*current_index];
        value_index++;
        *current_index+=1;
    }
    value[value_index] = '\0';
    token->value = value;   
    return token;
}

Token *generate_keyword_identifier(char *current, int *current_index){
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    token->type = KEYWORD;
    char *keyword = malloc(sizeof(char) * 8);
    int keyword_index = 0;
    while(isalpha(current[*current_index]) && current[*current_index] != '\0'){
        keyword[keyword_index] = current[*current_index];
        keyword_index++;
        *current_index += 1;
    }
    keyword[keyword_index] = '\0';
    bool valueKeyword = false;
    for(size_t i=0; i<num_keywords; i++){
        if(strcmp(keyword, keywords[i])==0){
            token->value = keywords[i];
            valueKeyword = true;
        }
    }
    if(!valueKeyword){
        token->type = IDENTIFIER;
        token->value = keyword;
    }
    return token;
}

Token *generate_string_token(char *current, int *current_index){
    Token *token = malloc(sizeof(Token));
    token->line_num = line_number;
    char *value = malloc(sizeof(char) * 64);
    int value_index = 0;
    *current_index += 1;
    while(current[*current_index] != '"'){
        value[value_index] = current[*current_index];
        value_index++;
        *current_index += 1;
    }
    value[value_index] = '\0';
    token->type = STRING;
    token->value = value;
    return token;
}

Token *generate_separator_operator_comp(char *current, int *current_index, TokenType type){
    Token *token = malloc(sizeof(Token));
    token->value = malloc(sizeof(char) * 2);
    token->value[0] = current[*current_index];
    token->value[1] = '\0';
    token->line_num = line_number;
    token->type = type;
    return token;
}

Token *lexer(FILE *file){
    int length;
    char *current = 0;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    current = malloc(sizeof(char)*(length+1));
    fread(current, 1, length, file);

    fclose(file);

    current[length] = '\0';
    int current_index = 0;

    int number_of_tokens = 12;
    int tokens_size = 0;
    Token *tokens = malloc(sizeof(Token) * number_of_tokens);
    tokens_index = 0;

    while(current[current_index]!='\0'){
        Token *token;
        tokens_size++;
        if(tokens_size > number_of_tokens){
        number_of_tokens *= 1.5;
        tokens = realloc(tokens, sizeof(Token) * number_of_tokens);
        }
        
        if(isalpha(current[current_index])){
            token = generate_keyword_identifier(current, &current_index);
            tokens[tokens_index] = *token;
            tokens_index++;
            current_index--;
        } else if(isdigit(current[current_index])){
            token = generate_num(current, &current_index); 
            tokens[tokens_index] = *token;
            tokens_index++;
            current_index--;
        } else if(current[current_index] == '"'){
            token = generate_string_token(current, &current_index);
            tokens[tokens_index] = *token;
            tokens_index++;
        } else if(isspace(current[current_index])){
            if(current[current_index] == '\n'){
                line_number++;
            }
        } else {
            bool tokenAssigned = false;
            for(size_t i=0; i<num_operators; i++){
                if(current[current_index]==operators[i]){
                    token = generate_separator_operator_comp(current, &current_index, OPERATOR);
                    tokenAssigned=true;
                    tokens[tokens_index] = *token;
                    tokens_index++;
                    break;
                }
            }
            if(!tokenAssigned){
                for (size_t i = 0; i < num_seperators; i++)
                {
                    if(current[current_index]==seperators[i]){
                        token=generate_separator_operator_comp(current, &current_index, SEPARATOR);
                        tokenAssigned=true;
                        tokens[tokens_index] = *token;
                        tokens_index++;
                        break;
                    }
                }
            }
            if(!tokenAssigned){
                for (size_t i = 0; i < num_comp_chars; i++)
                {
                    if(current[current_index]==comp_chars[i]){
                        token=generate_separator_operator_comp(current, &current_index, COMP);
                        tokenAssigned=true;
                        tokens[tokens_index] = *token;
                        tokens_index++;
                        break;
                    }
                }
            }
        }
        current_index++;
    }
    tokens_index--;
    tokens[tokens_index].value = malloc(sizeof(char));
    tokens[tokens_index].value[0] = '\0';
    tokens[tokens_index].type = END_OF_TOKENS;
    return tokens;
}

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case INT: return "INT";
        case KEYWORD: return "KEYWORD";
        case IDENTIFIER: return "IDENTIFIER";
        case OPERATOR: return "OPERATOR";
        case SEPARATOR: return "SEPARATOR";
        case FLOAT: return "FLOAT";
        case STRING: return "STRINGS";
        case COMP: return "COMP";
        case END_OF_TOKENS: return "END_OF_TOKENS";
        default: return "UNKNOWN";
    }
}

void getTokenNum(){
    printf("%zu\n", tokens_index);
}

void print_tokens(FILE *file){
    Token* tokens = lexer(file);
    for(size_t i=0; i<=tokens_index; i++){
        printf("Token: %s, %s\n", tokenTypeToString(tokens[i].type), tokens[i].value);
    }
}