%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%token FOR ID NUM
%token LE GE EQ NE

%%

S:
      FOR '(' initialization ';' condition ';' increment ')' statement
    ;

statement:
      ID '=' expression ';'
    | FOR '(' initialization ';' condition ';' increment ')' statement
    ;

initialization:
      ID '=' expression
    ;

increment:
      ID '=' expression
    ;

condition:
      expression '<' expression
    | expression '>' expression
    | expression LE expression
    | expression GE expression
    | expression EQ expression
    | expression NE expression
    ;

expression:
      ID
    | NUM
    | expression '+' expression
    | expression '-' expression
    ;

%%

void yyerror(const char *s)
{
    /* Do nothing */
}

int main(void)
{
    printf("Enter a for statement: ");

    if (yyparse() == 0)
        printf("Accepted\n");
    else
        printf("Rejected\n");

    return 0;
}