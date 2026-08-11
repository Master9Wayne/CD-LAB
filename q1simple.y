%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/' '%'

%%

input:
      expr '\n'     { printf("Result = %d\n", $1); }
    ;

expr:
      expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | expr '%' expr { $$ = $1 % $3; }
    | '(' expr ')'  { $$ = $2; }
    | NUMBER        { $$ = $1; }
    ;

%%

int yylex(void)
{
    int c;

    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c >= '0' && c <= '9')
    {
        ungetc(c, stdin);
        scanf("%d", &yylval);
        return NUMBER;
    }

    return c;
}

void yyerror(const char *s)
{
    printf("Invalid expression\n");
}

int main(void)
{
    printf("Enter an arithmetic expression: ");
    yyparse();
    return 0;
}