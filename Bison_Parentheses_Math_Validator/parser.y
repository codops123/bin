%{
#include <bits/stdc++.h>
using namespace std;

void yyerror(const char *s);
int yylex();
%}

%union {
    double num;
}

%token <num> NUM
%type <num> expr

%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS

%%

input:
    | input line
    ;

line:
    expr ';' {
        cout << "Result = " << $1 << endl;
        return 0;
    }
    ;

expr:
    NUM {
        $$ = $1;
    }
    | expr '+' expr {
        $$ = $1 + $3;
    }
    | expr '-' expr {
        $$ = $1 - $3;
    }
    | expr '*' expr {
        $$ = $1 * $3;
    }
    | expr '/' expr {
        if($3 == 0) {
            cout << "Semantic Error: Division by zero!" << endl;
            YYERROR;
        }
        else {
            $$ = $1 / $3;
        }
    }
    | expr '^' expr {
        $$ = pow($1, $3);
    }
    | '(' expr ')' {
        $$ = $2;
    }
    | '[' expr ']' {
        $$ = $2;
    }
    | '{' expr '}' {
        $$ = $2;
    }
    | '-' expr %prec UMINUS {
        $$ = -$2;
    }
    ;

%%

void yyerror(const char *s) {
    cout << "Invalid Expression: Syntax Error!" << endl;
}

int main() {
    cout << "Enter mathematical expression:" << endl;
    yyparse();
    return 0;
}
