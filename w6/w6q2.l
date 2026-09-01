%{
#include "y.tab.h"
%}

%%

"for"           return FOR;
[a-zA-Z][a-zA-Z0-9]*    return ID;
[0-9]+          return NUM;

"<="            return LE;
">="            return GE;
"=="            return EQ;
"!="            return NE;

[()=<>;+\-]     return yytext[0];

[ \t]            ;
\n               return 0;

.                return yytext[0];

%%

int yywrap()
{
    return 1;
}