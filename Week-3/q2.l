%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 500

struct Symbol {
    char name[100];
    char category[20];         // Variable or Function
    char type[50];             // Data Type / Return Type
    char scope[20];            // Global or Local
    int param_count;           // Number of parameters
    int line_no;
};

struct Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
int line_number = 1;
int brace_depth = 0;

char current_type[50] = "";
char last_identifier[100] = "";
int expecting_identifier = 0;
int in_function_params = 0;
int current_param_count = 0;

// Temporary storage for function signature details
char func_name[100] = "";
char func_type[50] = "";
int func_line_no = 0;

void add_symbol(const char* name, const char* category, const char* type, const char* scope, int params, int lno) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0 && strcmp(symbol_table[i].scope, scope) == 0 && strcmp(symbol_table[i].category, category) == 0) {
            return; // Avoid duplicates in same scope
        }
    }
    if (symbol_count < MAX_SYMBOLS) {
        strcpy(symbol_table[symbol_count].name, name);
        strcpy(symbol_table[symbol_count].category, category);
        strcpy(symbol_table[symbol_count].type, type);
        strcpy(symbol_table[symbol_count].scope, scope);
        symbol_table[symbol_count].param_count = params;
        symbol_table[symbol_count].line_no = lno;
        symbol_count++;
    }
}
%}

%%

\n                  { line_number++; }
[ \t\r]+            { /* Skip whitespace */ }
"//".*              { /* Skip comments */ }
"/*".*"*/"          { /* Skip multi-line comments */ }

"#include".*        { /* Skip preprocessor directives */ }
"using".*           { /* Skip using directives */ }

"int"|"float"|"double"|"char"|"void"|"bool"|"string" { 
                        strcpy(current_type, yytext);
                        expecting_identifier = 1;
                    }

"{"                 { 
                        brace_depth++; 
                    }

"}"                 { 
                        if (brace_depth > 0) brace_depth--; 
                    }

"("                 { 
                        if (expecting_identifier && strlen(last_identifier) > 0) {
                            strcpy(func_name, last_identifier);
                            strcpy(func_type, current_type);
                            func_line_no = line_number;
                            
                            in_function_params = 1;
                            current_param_count = 0;
                            expecting_identifier = 0;
                            last_identifier[0] = '\0';
                        }
                    }

")"                 { 
                        if (in_function_params) {
                            // If there was a parameter right before closing parenthesis (e.g., int b in (int a, int b))
                            if (expecting_identifier && strlen(last_identifier) > 0) {
                                current_param_count++;
                                add_symbol(last_identifier, "Variable", current_type, "Local", 0, line_number);
                                last_identifier[0] = '\0';
                            }
                            in_function_params = 0;
                            expecting_identifier = 0;
                            char* scope = (brace_depth == 0) ? "Global" : "Local";
                            add_symbol(func_name, "Function", func_type, scope, current_param_count, func_line_no);
                            func_name[0] = '\0';
                        }
                    }

","                 { 
                        if (in_function_params) {
                            if (expecting_identifier && strlen(last_identifier) > 0) {
                                current_param_count++;
                                add_symbol(last_identifier, "Variable", current_type, "Local", 0, line_number);
                                last_identifier[0] = '\0';
                                expecting_identifier = 0; // Reset until next type keyword
                            }
                        } else if (expecting_identifier && strlen(last_identifier) > 0) {
                            char* scope = (brace_depth == 0) ? "Global" : "Local";
                            add_symbol(last_identifier, "Variable", current_type, scope, 0, line_number);
                            last_identifier[0] = '\0';
                        }
                    }

";"                 { 
                        if (expecting_identifier && strlen(last_identifier) > 0) {
                            char* scope = (brace_depth == 0) ? "Global" : "Local";
                            add_symbol(last_identifier, "Variable", current_type, scope, 0, line_number);
                            expecting_identifier = 0;
                            last_identifier[0] = '\0';
                        }
                        expecting_identifier = 0;
                        last_identifier[0] = '\0';
                    }

[a-zA-Z_][a-zA-Z0-9_]* { 
                        if (expecting_identifier) {
                            strcpy(last_identifier, yytext);
                        }
                    }

"="                 { 
                        if (expecting_identifier && strlen(last_identifier) > 0 && !in_function_params) {
                            char* scope = (brace_depth == 0) ? "Global" : "Local";
                            add_symbol(last_identifier, "Variable", current_type, scope, 0, line_number);
                            expecting_identifier = 0;
                            last_identifier[0] = '\0';
                        }
                    }

.                   { /* Ignore other characters */ }

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

    printf("\n=========================================================================================\n");
    printf("%-20s | %-12s | %-15s | %-10s | %-10s | %-10s\n", 
           "Name", "Category", "Type", "Scope", "Params", "Line No.");
    printf("=========================================================================================\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%-20s | %-12s | %-15s | %-10s | %-10d | %-10d\n", 
               symbol_table[i].name, 
               symbol_table[i].category, 
               symbol_table[i].type, 
               symbol_table[i].scope, 
               symbol_table[i].param_count, 
               symbol_table[i].line_no);
    }
    printf("=========================================================================================\n");
    
    return 0;
}
