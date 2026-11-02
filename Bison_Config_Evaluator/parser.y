%{
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
using namespace std;

int yylex();
void yyerror(const char *s);

map<string, double> variables;
%}

%union {
    double num;
    char *id;
}

%token <num> NUM
%token <id> ID
%type <num> expr

%left '+' '-'
%left '*' '/'

%%

input:
    | input statement
    ;

statement:
    ID '=' expr ';' {
        variables[$1] = $3;
        cout << $1 << " = " << $3 << endl;
        free($1);
    }
    ;

expr:
    NUM {
        $$ = $1;
    }
    | 
    ID {
        string n = $1;

        if(variables.find(n) == variables.end()) {
            cout << "Error: " << n << " not defined\n";
            $$ = 0;
        }
        else {
            $$ = variables[n];
        }

        free($1);
    }
    | 
    expr '+' expr {
        $$ = $1 + $3;
    }
    | 
    expr '-' expr {
        $$ = $1 - $3;
    }
    | expr '*' expr {
        $$ = $1 * $3;
    }
    | 
    expr '/' expr {
        if($3 == 0) {
            cout << "Division by zero!\n";
            $$ = 0;
        }
        else {
            $$ = $1 / $3;
        }
    }
    | '(' expr ')' {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s) {
    cout << "Syntax Error!\n";
}

int main() {
    cout << "Enter configuration statements:\n";
    yyparse();
    return 0;
}
