%{
#include <stdio.h>
#include <string.h>

int count_if=0, count_for=0, count_while=0, count_int=0, count_return=0;
%}

%%
"if"      { count_if++; }
"for"     { count_for++; }
"while"   { count_while++; }
"int"     { count_int++; }
"return"  { count_return++; }

[a-zA-Z_][a-zA-Z0-9_]*   ;
.|\n                     ;
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();

    printf("\nKeyword\tFrequency\n");
    printf("if\t%d\n", count_if);
    printf("for\t%d\n", count_for);
    printf("while\t%d\n", count_while);
    printf("int\t%d\n", count_int);
    printf("return\t%d\n", count_return);

    return 0;
}
