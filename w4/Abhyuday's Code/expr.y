%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

/* Precedence: lowest to highest, all left-associative */
%left '+' '-'
%left '*' '/' '%'

%%

program:
    expr '\n'   { printf("Result = %d\n", $1); exit(0); }
    ;

expr:
      expr '+' expr   { $$ = $1 + $3; }
    | expr '-' expr   { $$ = $1 - $3; }
    | expr '*' expr   { $$ = $1 * $3; }
    | expr '/' expr   {
          if ($3 == 0) { printf("Runtime Error: Division by zero\n"); exit(0); }
          $$ = $1 / $3;
      }
    | expr '%' expr   {
          if ($3 == 0) { printf("Runtime Error: Modulus by zero\n"); exit(0); }
          $$ = $1 % $3;
      }
    | '(' expr ')'    { $$ = $2; }
    | NUMBER          { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    printf("Syntax Error: Incomplete expression\n");
    exit(0);
}

int main(void) {
    printf("Enter an expression: ");
    yyparse();
    return 0;
}
