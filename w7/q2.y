%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;
extern int yylineno;

void yyerror(char *s);

int return_type;
int has_return;
%}

%token INT FLOAT VOID RETURN
%token ID NUM FNUM

%%

program:
    functions
    ;

functions:
      function
    | functions function
    ;

function:
    type ID '(' parameters ')' 
    {
        return_type = $1;
        has_return = 0;
    }
    '{' statements '}'
    {
        if (return_type != VOID && !has_return)
        {
            printf("Error: Missing return statement at line %d\n",
                   yylineno);
        }
        else
        {
            printf("Function parsed successfully.\n");
        }
    }
    ;

type:
      INT   { $$ = INT; }
    | FLOAT { $$ = FLOAT; }
    | VOID  { $$ = VOID; }
    ;

parameters:
      /* empty */
    | parameter_list
    ;

parameter_list:
      parameter
    | parameter_list ',' parameter
    ;

parameter:
      type ID
    | ID
      {
          printf("Error: Missing parameter type at line %d\n",
                 yylineno);
          YYERROR;
      }
    ;

statements:
      /* empty */
    | statements statement
    ;

statement:
      declaration ';'
    | RETURN expression ';'
      {
          has_return = 1;

          if (return_type == VOID)
          {
              printf("Error: Void function cannot return a value\n");
          }
      }

    | RETURN ';'
      {
          if (return_type != VOID)
          {
              printf("Error: Missing return value at line %d\n",
                     yylineno);
          }

          has_return = 1;
      }

    | declaration
      {
          printf("Error: Missing semicolon in function body at line %d\n",
                 yylineno);
          YYERROR;
      }

    | nested_function
    ;

declaration:
      type ID
    | type ID '=' expression
    ;

nested_function:
      type ID '(' parameters ')' '{' statements '}'
      {
          printf("Error: Invalid nested function definition at line %d\n",
                 yylineno);
          YYERROR;
      }
    ;

expression:
      ID
    | NUM
    | FNUM
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    ;

%%

void yyerror(char *s)
{
    printf("Syntax error at line %d: %s\n", yylineno, s);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./q2 input.c\n");
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if (!yyin)
    {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }

    yyparse();

    fclose(yyin);

    return 0;
}