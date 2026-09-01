%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%token IF ID NUM
%token LE GE EQ NE

%%

S:
      IF '(' condition ')' statement
    ;

statement:
      ID '=' expression ';'
    | IF '(' condition ')' statement
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
    ;

%%

void yyerror(const char *s)
{
    /* Do nothing */
}

int main(void)
{
    printf("Enter an if statement: ");

    if (yyparse() == 0)
        printf("Accepted\n");
    else
        printf("Rejected\n");

    return 0;
}