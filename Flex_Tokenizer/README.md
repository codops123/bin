# Flex Tokenizer

Lexical analyzer implemented using Flex (`lexer.l`) that parses source code tokens and displays them with line numbers.

## How to Run
```bash
flex lexer.l
gcc lex.yy.c -o lexer
./lexer input.txt
```
