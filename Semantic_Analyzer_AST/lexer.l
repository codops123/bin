
%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "parser.tab.h"
using namespace std;
%}

%%

"int" {
    yylval.type = 0;
    return TYPE;
}

"float" {
    yylval.type = 1;
    return TYPE;
}

"char" {
    yylval.type = 2;
    return TYPE;
}

[0-9]+(\.[0-9]+)? {
    yylval.id = strdup(yytext);
    return NUMBER;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.id = strdup(yytext);
    return ID;
}

[ \t\r\n]+ ;

. {
    return yytext[0];
}

%%

int yywrap() {
    return 1;
}

