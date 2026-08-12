%{
#include "y.tab.h"
%}

%%
0     { return ZERO; }
1     { return ONE; }
\n    { return '\n'; }
.     { return INVALID; }
%%

int yywrap() { return 1; }
