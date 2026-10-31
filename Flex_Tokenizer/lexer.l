%{
#include <stdio.h>
#include <string.h>

int line_number = 1;
%}

/* Definitions / Regular Expressions */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
INT_CONST   {DIGIT}+
FLOAT_CONST {DIGIT}+"."{DIGIT}+
STRING      \"([^\\\"]|\\.)*\"
WS          [ \t\r]+

%%

\n              { line_number++; }
{WS}            { /* Ignore whitespace */ }

"//".*          { /* Ignore single-line comments */ }
"/*"([^*]|\*+[^*/])*\*+"/" { 
    /* Ignore multi-line comments and count lines */
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') line_number++;
    }
}

    /* Keywords */
"if"            { printf("%-5d %-25s %-20s\n", line_number, yytext, "KEYWORD_IF"); }
"else"          { printf("%-5d %-25s %-20s\n", line_number, yytext, "KEYWORD_ELSE"); }
"while"         { printf("%-5d %-25s %-20s\n", line_number, yytext, "KEYWORD_WHILE"); }
"for"           { printf("%-5d %-25s %-20s\n", line_number, yytext, "KEYWORD_FOR"); }
"return"        { printf("%-5d %-25s %-20s\n", line_number, yytext, "KEYWORD_RETURN"); }
"int"           { printf("%-5d %-25s %-20s\n", line_number, yytext, "TYPE_INT"); }
"float"         { printf("%-5d %-25s %-20s\n", line_number, yytext, "TYPE_FLOAT"); }
"char"          { printf("%-5d %-25s %-20s\n", line_number, yytext, "TYPE_CHAR"); }
"void"          { printf("%-5d %-25s %-20s\n", line_number, yytext, "TYPE_VOID"); }

    /* Relational Operators */
"=="            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_REL_EQ"); }
"!="            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_REL_NEQ"); }
"<="            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_REL_LE"); }
">="            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_REL_GE"); }
"<"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_REL_LT"); }
">"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_REL_GT"); }

    /* Arithmetic Operators */
"+"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_ARITH_ADD"); }
"-"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_ARITH_SUB"); }
"*"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_ARITH_MUL"); }
"/"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_ARITH_DIV"); }
"="             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_ASSIGN"); }

    /* Logical & Inc/Dec Operators */
"&&"            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_LOGIC_AND"); }
"||"            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_LOGIC_OR"); }
"!"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_LOGIC_NOT"); }
"++"            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_INCREMENT"); }
"--"            { printf("%-5d %-25s %-20s\n", line_number, yytext, "OP_DECREMENT"); }

    /* Delimiters & Separators */
";"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "DELIM_SEMICOLON"); }
","             { printf("%-5d %-25s %-20s\n", line_number, yytext, "DELIM_COMMA"); }
"("             { printf("%-5d %-25s %-20s\n", line_number, yytext, "DELIM_LPAREN"); }
")"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "DELIM_RPAREN"); }
"{"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "DELIM_LBRACE"); }
"}"             { printf("%-5d %-25s %-20s\n", line_number, yytext, "DELIM_RBRACE"); }

    /* Constants, Literals, Identifiers */
{STRING}        { printf("%-5d %-25s %-20s\n", line_number, yytext, "STRING_LITERAL"); }
{FLOAT_CONST}   { printf("%-5d %-25s %-20s\n", line_number, yytext, "CONST_FLOAT"); }
{INT_CONST}     { printf("%-5d %-25s %-20s\n", line_number, yytext, "CONST_INT"); }
{ID}            { printf("%-5d %-25s %-20s\n", line_number, yytext, "IDENTIFIER"); }

    /* Unrecognized Characters */
.               { printf("%-5d %-25s %-20s\n", line_number, yytext, "ERROR_UNKNOWN"); }

%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Error opening file");
            return 1;
        }
        yyin = fp;
    }

    printf("===============================================================\n");
    printf("%-5s %-25s %-20s\n", "Line", "Lexeme", "Token Type");
    printf("===============================================================\n");
    
    yylex();
    
    printf("===============================================================\n");
    return 0;
}
