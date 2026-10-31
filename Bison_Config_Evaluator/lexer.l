%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern int line_no;
%}

%option noyywrap

%%

[ \t\r]+                { /* Skip spaces */ }
\n                      { line_no++; }
"#".*                   { /* Comments */ }

    /* Data Types / Keywords */
"print"                 { return TOK_PRINT; }

    /* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]*  { 
                            yylval.strVal = strdup(yytext); 
                            return TOK_IDENTIFIER; 
                        }

    /* Floating Point and Integer Numbers */
[0-9]+\.[0-9]+          { 
                            yylval.numVal = atof(yytext); 
                            return TOK_NUMBER; 
                        }
[0-9]+                  { 
                            yylval.numVal = atof(yytext); 
                            return TOK_NUMBER; 
                        }

    /* Strings */
\"([^\\\"]|\\.)*\"      { 
                            yytext[strlen(yytext) - 1] = '\0';
                            yylval.strVal = strdup(yytext + 1); 
                            return TOK_STRING; 
                        }

    /* Operators */
"="                     { return '='; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
";"                     { return ';'; }
"("                     { return '('; }
")"                     { return ')'; }

.                       { printf("Lexical Error at line %d: Unexpected character '%s'\n", line_no, yytext); }

%%
