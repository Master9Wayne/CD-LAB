%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS 1000

struct Token {
    char lexeme[100];
    char category[50];
    int line_no;
    int frequency;
};

struct Token token_table[MAX_TOKENS];
int token_count = 0;
int line_number = 1;

void add_token(const char* lexeme, const char* category) {
    for (int i = 0; i < token_count; i++) {
        if (strcmp(token_table[i].lexeme, lexeme) == 0 && 
            strcmp(token_table[i].category, category) == 0) {
            token_table[i].frequency++;
            return;
        }
    }
    if (token_count < MAX_TOKENS) {
        strcpy(token_table[token_count].lexeme, lexeme);
        strcpy(token_table[token_count].category, category);
        token_table[token_count].line_no = line_number;
        token_table[token_count].frequency = 1;
        token_count++;
    }
}
%}

%%

\n                  { line_number++; }
[ \t\r]+            { /* Skip whitespace */ }
"//".*              { /* Skip comments */ }

"int"|"float"|"double"|"char"|"void"|"if"|"else"|"for"|"while"|"do"|"return"|"class"|"public"|"private"|"protected"|"include"|"using"|"namespace"|"std" {
                        add_token(yytext, "Keyword");
                    }

[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? { add_token(yytext, "Numeric Constant (Float)"); }
0[xX][0-9a-fA-F]+               { add_token(yytext, "Numeric Constant (Hex)"); }
0[0-7]+                         { add_token(yytext, "Numeric Constant (Octal)"); }
[0-9]+                          { add_token(yytext, "Numeric Constant (Decimal)"); }

"=="|"!="|"<="|">="["<"|">"]    { add_token(yytext, "Relational Operator"); }
"<"|">"                         { add_token(yytext, "Relational Operator"); }
"&&"|"||"|"!"                   { add_token(yytext, "Logical Operator"); }
"="|"+="|"-="|"*="|"/="         { add_token(yytext, "Assignment Operator"); }
"+"|"-"|"*"|"/"                 { add_token(yytext, "Arithmetic Operator"); }

","|";"|"("|")"|"{"|"}"|"["|"]"|"::" { add_token(yytext, "Delimiter"); }

[a-zA-Z_][a-zA-Z0-9_]*          { add_token(yytext, "Identifier"); }

.                               { /* Ignore other characters */ }

%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("File opening failed");
            return 1;
        }
        yyin = file;
    }
    
    yylex();

    printf("\n=================================================================================\n");
    printf("%-30s | %-25s | %-10s | %-10s\n", "Lexeme", "Token Category", "Line No.", "Frequency");
    printf("=================================================================================\n");
    for (int i = 0; i < token_count; i++) {
        printf("%-30s | %-25s | %-10d | %-10d\n", 
               token_table[i].lexeme, 
               token_table[i].category, 
               token_table[i].line_no, 
               token_table[i].frequency);
    }
    printf("=================================================================================\n");
    
    return 0;
}
