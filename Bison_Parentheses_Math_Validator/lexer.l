%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern int line_no;
extern int col_no;
%}

%option noyywrap

%%

[ \t\r]+        { col_no += yyleng; }
\n              { line_no++; col_no = 1; }

[0-9]+(\.[0-9]+)? {
                    yylval.dval = atof(yytext);
                    col_no += yyleng;
                    return NUMBER;
                }

"+"             { col_no++; return '+'; }
"-"             { col_no++; return '-'; }
"*"             { col_no++; return '*'; }
"/"             { col_no++; return '/'; }
"^"             { col_no++; return '^'; }

"("             { col_no++; return '('; }
")"             { col_no++; return ')'; }
"{"             { col_no++; return '{'; }
"}"             { col_no++; return '}'; }
"["             { col_no++; return '['; }
"]"             { col_no++; return ']'; }
";"             { col_no++; return ';'; }

.               {
                    printf("Lexical Error at [%d:%d]: Invalid character '%s'\n", line_no, col_no, yytext);
                    col_no += yyleng;
                }

%%
