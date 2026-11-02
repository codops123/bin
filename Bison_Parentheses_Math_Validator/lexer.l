%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
%}

%%

[0-9]+(\.[0-9]+)? {
    yylval.num = atof(yytext);
    return NUM;
}

[ \t\r\n]+ ;

. return yytext[0];

%%

int yywrap() {
    return 1;
}
