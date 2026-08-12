%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
%}

%token ZERO ONE INVALID

%%

start:
    S '\n'   { printf("Accepted\n"); exit(0); }
    ;

/* Grammar: S -> 0S | 01   (one or more 0's followed by exactly one 1) */
S:
      ZERO S
    | ZERO ONE
    ;

%%

void yyerror(const char *s) {
    printf("Rejected\n");
    exit(0);
}

int main(void) {
    printf("Enter string: ");
    yyparse();
    return 0;
}
