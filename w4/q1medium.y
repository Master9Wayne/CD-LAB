%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/' '%'

%%

input:
    expr '\n' { printf("Result = %d\n", $1); }
    ;

expr:
      expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr
        {
            if ($3 == 0)
                yyerror("Division by zero");
            else
                $$ = $1 / $3;
        }
    | expr '%' expr
        {
            if ($3 == 0)
                yyerror("Modulus by zero");
            else
                $$ = $1 % $3;
        }
    | '(' expr ')' { $$ = $2; }
    | NUMBER       { $$ = $1; }
    ;

%%

int yylex(void)
{
    int c;

    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c >= '0' && c <= '9')
    {
        int n = 0;

        do
        {
            n = n * 10 + (c - '0');
            c = getchar();
        } while (c >= '0' && c <= '9');

        ungetc(c, stdin);
        yylval = n;
        return NUMBER;
    }

    return c;
}

void yyerror(const char *s)
{
    printf("Error: %s\n", s);
}

int main(void)
{
    printf("Enter expression: ");
    yyparse();
    return 0;
}
