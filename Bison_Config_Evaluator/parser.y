%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_no = 1;
void yyerror(const char *s);
int yylex(void);

// Simple Symbol Table Structure
struct Symbol {
    char *name;
    double numValue;
    char *strValue;
    int isString; // 1 if string, 0 if number
    struct Symbol *next;
};

struct Symbol *symTable = NULL;

void setNumericVariable(const char *name, double val) {
    struct Symbol *curr = symTable;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            curr->numValue = val;
            curr->isString = 0;
            return;
        }
        curr = curr->next;
    }
    struct Symbol *newSym = (struct Symbol*)malloc(sizeof(struct Symbol));
    newSym->name = strdup(name);
    newSym->numValue = val;
    newSym->isString = 0;
    newSym->strValue = NULL;
    newSym->next = symTable;
    symTable = newSym;
}

void setStringVariable(const char *name, const char *val) {
    struct Symbol *curr = symTable;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            if (curr->strValue) free(curr->strValue);
            curr->strValue = strdup(val);
            curr->isString = 1;
            return;
        }
        curr = curr->next;
    }
    struct Symbol *newSym = (struct Symbol*)malloc(sizeof(struct Symbol));
    newSym->name = strdup(name);
    newSym->strValue = strdup(val);
    newSym->isString = 1;
    newSym->next = symTable;
    symTable = newSym;
}

struct Symbol* getVariable(const char *name) {
    struct Symbol *curr = symTable;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
%}

%union {
    double numVal;
    char *strVal;
}

%token <strVal> TOK_IDENTIFIER TOK_STRING
%token <numVal> TOK_NUMBER
%token TOK_PRINT

%type <numVal> expr

%left '+' '-'
%left '*' '/'

%%

program:
    statements
    ;

statements:
    statements statement
    | /* empty */
    ;

statement:
    TOK_IDENTIFIER '=' expr ';' {
        setNumericVariable($1, $3);
        printf("[ASSIGN] Variable '%s' = %.2f\n", $1, $3);
        free($1);
    }
    | TOK_IDENTIFIER '=' TOK_STRING ';' {
        setStringVariable($1, $3);
        printf("[ASSIGN] Variable '%s' = \"%s\"\n", $1, $3);
        free($1);
        free($3);
    }
    | TOK_PRINT TOK_IDENTIFIER ';' {
        struct Symbol *sym = getVariable($2);
        if (sym) {
            if (sym->isString) {
                printf("[OUTPUT] %s = \"%s\"\n", sym->name, sym->strValue);
            } else {
                printf("[OUTPUT] %s = %.2f\n", sym->name, sym->numValue);
            }
        } else {
            printf("[RUNTIME ERROR] Undefined variable '%s' at line %d\n", $2, line_no);
        }
        free($2);
    }
    | TOK_PRINT expr ';' {
        printf("[OUTPUT EVAL] Result = %.2f\n", $2);
    }
    ;

expr:
    expr '+' expr   { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if ($3 == 0) {
            printf("[RUNTIME ERROR] Division by zero at line %d!\n", line_no);
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | '(' expr ')'  { $$ = $2; }
    | TOK_NUMBER    { $$ = $1; }
    | TOK_IDENTIFIER {
        struct Symbol *sym = getVariable($1);
        if (sym) {
            if (sym->isString) {
                printf("[TYPE ERROR] Cannot use string variable '%s' in math expression at line %d\n", $1, line_no);
                $$ = 0;
            } else {
                $$ = sym->numValue;
            }
        } else {
            printf("[RUNTIME ERROR] Undefined variable '%s' used at line %d\n", $1, line_no);
            $$ = 0;
        }
        free($1);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_no, s);
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
    printf("=================================================================\n");
    printf("   Bison/Flex Configuration Language Syntax Checker & Evaluator\n");
    printf("=================================================================\n");
    yyparse();
    printf("=================================================================\n");
    printf("Finished execution successfully.\n");
    return 0;
}
