%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);

int error_flag = 0;
%}

/* Token types */
%token NUMBER

/* Operator precedence and associativity */
%left '+' '-'
%left '*' '/' '%'

%%

input:
      expr '\n'
        {
            if (!error_flag)
                printf("Result = %d\n", $1);
        }
    ;

expr:
      expr '+' expr
        {
            $$ = $1 + $3;
        }

    | expr '-' expr
        {
            $$ = $1 - $3;
        }

    | expr '*' expr
        {
            $$ = $1 * $3;
        }

    | expr '/' expr
        {
            if ($3 == 0)
            {
                printf("Runtime Error: Division by zero\n");
                error_flag = 1;
                $$ = 0;
            }
            else
            {
                $$ = $1 / $3;
            }
        }

    | expr '%' expr
        {
            if ($3 == 0)
            {
                printf("Runtime Error: Modulus by zero\n");
                error_flag = 1;
                $$ = 0;
            }
            else
            {
                $$ = $1 % $3;
            }
        }

    | '(' expr ')'
        {
            $$ = $2;
        }

    | NUMBER
        {
            $$ = $1;
        }
    ;

%%

int yylex(void)
{
    int c;

    /* Skip spaces and tabs */
    while ((c = getchar()) == ' ' || c == '\t')
        ;

    /* Number */
    if (c >= '0' && c <= '9')
    {
        int num = 0;

        do
        {
            num = num * 10 + (c - '0');
            c = getchar();
        }
        while (c >= '0' && c <= '9');

        ungetc(c, stdin);

        yylval = num;
        return NUMBER;
    }

    /* Valid operators and parentheses */
    if (c == '+' || c == '-' ||
        c == '*' || c == '/' ||
        c == '%' || c == '(' ||
        c == ')' || c == '\n')
    {
        return c;
    }

    /* Invalid symbol */
    if (c != EOF)
    {
        printf("Lexical Error: Invalid symbol '%c'\n", c);

        /* Consume remaining input */
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        error_flag = 1;
        return '\n';
    }

    return 0;
}

void yyerror(const char *s)
{
    if (!error_flag)
    {
        printf("Syntax Error: Incomplete expression\n");
        error_flag = 1;
    }
}

int main(void)
{
    printf("Enter expression: ");

    yyparse();

    return 0;
}