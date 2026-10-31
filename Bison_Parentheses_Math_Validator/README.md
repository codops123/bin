# Bison Parentheses & Math Structural Validator

Flex + Bison tool that:
- Validates balanced nested parentheses: `()`, `{}`, `[]`
- Validates math syntax formatting
- Evaluates valid expressions

## How to Run
```bash
bison -d parser.y
flex lexer.l
gcc parser.tab.c lex.yy.c -o validator -lm
./validator input.txt
```
