# Bison Parentheses & Math Structural Validator

Flex + Bison tool that:
- Validates balanced nested parentheses: `()`, `{}`, `[]`
- Validates math syntax formatting
- Evaluates valid expressions

## How to Run
```bash
bison -d parser.y
flex lexer.l
g++ parser.tab.c lex.yy.c -o validator -lm
./validator
```
*(On Windows MinGW, run `.\validator.exe`)*

## Input Format & Example
The program reads mathematical expressions interactively from standard input. Expressions **must terminate with a semicolon (`;`)**.

Supported features:
- Parentheses: nested `()`, `{}`, and `[]`
- Arithmetic operations: `+`, `-`, `*`, `/`, exponent `^`, and unary minus `-`
- Semicolon `;` at the end to trigger evaluation and display result.

### Example Input:
```text
(10 + [5 * {3 - 1}]) / 2;
```

### Expected Output:
```text
Enter mathematical expression:
Result = 10
```

### Another Example:
```text
4 ^ 2 + 3 * (5 - 2);
```

### Expected Output:
```text
Result = 25
```
