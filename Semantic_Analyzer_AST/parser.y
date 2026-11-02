
%code requires {
    #include <string>
    using namespace std;

    struct Node {
        string type;
        string value;
        Node *left, *right;

        Node(string t, string v = "", Node *l = NULL, Node *r = NULL) {
            type = t;
            value = v;
            left = l;
            right = r;
        }
    };
}

%{
#include <iostream>
#include <map>
#include <string>
using namespace std;

void yyerror(const char *s);
int yylex();

enum DataType { INT, FLOAT, CHAR, ERROR };

map<string, DataType> symbolTable;

string typeName(DataType type) {
    if (type == INT) return "int";
    if (type == FLOAT) return "float";
    if (type == CHAR) return "char";
    return "error";
}

DataType analyze(Node *node) {
    if (!node) return ERROR;

    if (node->type == "NUMBER") {
        return node->value.find('.') != string::npos ? FLOAT : INT;
    }

    if (node->type == "VARIABLE") {
        if (symbolTable.find(node->value) == symbolTable.end()) {
            cout << "Semantic Error: Undeclared variable "
                 << node->value << endl;
            return ERROR;
        }

        return symbolTable[node->value];
    }

    if (node->type == "BINARY") {
        DataType left = analyze(node->left);
        DataType right = analyze(node->right);

        if (left == ERROR || right == ERROR)
            return ERROR;

        if (left == CHAR || right == CHAR) {
            cout << "Semantic Error: Invalid arithmetic operation on char"
                 << endl;
            return ERROR;
        }

        if (left == FLOAT || right == FLOAT)
            return FLOAT;

        return INT;
    }

    return ERROR;
}

void declareVariable(string name, DataType type) {
    if (symbolTable.count(name)) {
        cout << "Semantic Error: Variable "
             << name << " already declared!" << endl;
    }
    else {
        symbolTable[name] = type;
        cout << "Declaration Valid: " << name
             << " (" << typeName(type) << ")" << endl;
    }
}

void assignVariable(string name, Node *node) {
    if (!symbolTable.count(name)) {
        cout << "Semantic Error: Undeclared variable "
             << name << endl;
        return;
    }

    DataType left = symbolTable[name];
    DataType right = analyze(node);

    if (right == ERROR)
        return;

    if (left != right) {
        cout << "Semantic Error: Type mismatch in assignment to "
             << name << " ("
             << typeName(right) << " to "
             << typeName(left) << ")" << endl;
    }
    else {
        cout << "Assignment Valid: " << name << endl;
    }
}

%}

%union {
    int type;
    char *id;
    Node *node;
}

%token <id> ID NUMBER
%token <type> TYPE

%type <node> expr

%left '+' '-'
%left '*' '/'

%%

program:
      /* empty */
    | program statement
    ;

statement:
      TYPE ID ';' {
          declareVariable($2, (DataType)$1);
      }
    | ID '=' expr ';' {
          assignVariable($1, $3);
      }
    ;

expr:
      NUMBER {
          $$ = new Node("NUMBER", $1);
      }
    | ID {
          $$ = new Node("VARIABLE", $1);
      }
    | expr '+' expr {
          $$ = new Node("BINARY", "+", $1, $3);
      }
    | expr '-' expr {
          $$ = new Node("BINARY", "-", $1, $3);
      }
    | expr '*' expr {
          $$ = new Node("BINARY", "*", $1, $3);
      }
    | expr '/' expr {
          $$ = new Node("BINARY", "/", $1, $3);
      }
    | '(' expr ')' {
          $$ = $2;
      }
    ;

%%

void yyerror(const char *s) {
    cout << "Syntax Error!" << endl;
}

int main() {
    cout << "Enter declarations and assignments:\n";
    yyparse();
    return 0;
}

