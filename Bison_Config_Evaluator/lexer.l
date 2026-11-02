%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

[0-9]+(\.[0-9]+)? {
    yylval.num = atof(yytext);
    return NUM;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.id = strdup(yytext);
    return ID;
}

[ \t\r\n]+ ;
. return yytext[0];

%%

int yywrap() {
    return 1;
}

//command
/*
bison -d parser.y
flex lexer.l
g++ parser.tab.c lex.yy.c -o lab 
./lab
*/
