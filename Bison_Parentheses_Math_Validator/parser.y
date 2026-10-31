%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int line_no = 1;
int col_no = 1;
int valid_expressions = 0;
int syntax_errors = 0;

void yyerror(const char *s);
int yylex(void);
%}

%union {
    double dval;
}

%token <dval> NUMBER

%type <dval> expr factor term

%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS

%%

input:
    lines
    ;

lines:
    lines line
    | /* empty */
    ;

line:
    expr ';' {
        valid_expressions++;
        printf("[VALID & EVALUATED] Expression %d Value = %.4f (Parentheses & Math formatting valid)\n", 
                valid_expressions, $1);
    }
    | error ';' {
        syntax_errors++;
        yyerrok;
    }
    ;

expr:
    expr '+' term           { $$ = $1 + $3; }
    | expr '-' term         { $$ = $1 - $3; }
    | term                  { $$ = $1; }
    ;

term:
    term '*' factor         { $$ = $1 * $3; }
    | term '/' factor       { 
        if ($3 == 0.0) {
            printf("[SEMANTIC ERROR at line %d]: Division by zero!\n", line_no);
            $$ = 0.0;
        } else {
            $$ = $1 / $3;
        }
    }
    | factor                { $$ = $1; }
    ;

factor:
    NUMBER                  { $$ = $1; }
    | '-' factor %prec UMINUS { $$ = -$2; }
    | factor '^' factor     { $$ = pow($1, $3); }
    /* Perfectly Matched Nested Parentheses of all standard types */
    | '(' expr ')'          { $$ = $2; }
    | '{' expr '}'          { $$ = $2; }
    | '[' expr ']'          { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[SYNTAX/STRUCTURAL ERROR at line %d, col %d]: %s (Unbalanced parenthesis or invalid math syntax)\n", 
            line_no, col_no, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Error opening file");
            return 1;
        }
        extern FILE *yyin;
        yyin = f;
    }

    printf("===================================================================\n");
    printf(" Bison/Flex: Nested Parentheses & Math Structural Validator\n");
    printf("===================================================================\n");
    yyparse();
    printf("===================================================================\n");
    printf("Summary: %d Valid Expressions, %d Syntax/Structural Errors\n", valid_expressions, syntax_errors);
    printf("===================================================================\n");

    return 0;
}
