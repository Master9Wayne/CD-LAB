%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;
extern int yylineno;

void yyerror(char *s);
%}

%token FOR WHILE INT FLOAT ID NUM FNUM
%token LE GE EQ NE INC DEC
%token PLUS MINUS MUL DIV LT GT ASSIGN

%%

program:
    statements
    {
        printf("\nParsing successful!\n");
    }
    ;

statements:
      /* empty */
    | statements statement
    ;

statement:
      declaration ';'
    | assignment ';'
    | loop
    | '{' statements '}'
    ;

declaration:
      INT ID
    | FLOAT ID
    | INT ID ASSIGN expression
    | FLOAT ID ASSIGN expression
    ;

assignment:
    ID ASSIGN expression
    ;

loop:
      FOR '(' for_init ';' condition ';' update ')' statement
    | WHILE '(' condition ')' statement
    ;

for_init:
      /* empty */
    | declaration
    | assignment
    ;

condition:
      expression LT expression
    | expression GT expression
    | expression LE expression
    | expression GE expression
    | expression EQ expression
    | expression NE expression
    ;

update:
      /* empty */
    | ID INC
    | ID DEC
    | assignment
    ;

expression:
      ID
    | NUM
    | FNUM
    | expression PLUS expression
    | expression MINUS expression
    | expression MUL expression
    | expression DIV expression
    ;

%%

void yyerror(char *s)
{
    printf("\nSyntax error at line %d: %s\n", yylineno, s);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./q1 input.c\n");
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if (yyin == NULL)
    {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }

    yyparse();

    fclose(yyin);

    return 0;
}