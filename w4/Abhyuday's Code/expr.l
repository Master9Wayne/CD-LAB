%{
#include "y.tab.h"
#include <stdlib.h>
#include <stdio.h>
%}

%%
[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[+\-*/%()]  { return yytext[0]; }
\n          { return '\n'; }
[ \t]       { /* skip spaces */ }
.           { printf("Lexical Error: Invalid symbol '%s'\n", yytext); exit(0); }
%%

int yywrap() { return 1; }
