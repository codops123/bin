# Flex Tokenizer

Lexical analyzer implemented using Flex (`lexer.l`) that parses source code tokens and displays them with line numbers.

## How to Run
```bash
flex lexer.l
g++ lex.yy.c -o token
./token
```
*(On Windows MinGW, run `.\token.exe`)*

## Input Format & Example
The executable takes code lines interactively from standard input (`stdin`) or piped from a file.

### Example Interactive Input:
Type your statements and press Enter:
```c
int a = 10;
float b = 20.5;
if (a <= 10) return a;
```
*(To finish input, press `Ctrl+D` on Linux/macOS or `Ctrl+Z` then `Enter` on Windows).*

### Or Redirecting from `input.txt`:
```powershell
Get-Content input.txt | .\token.exe
```

### Expected Output:
```text
Enter your code:

KEYWORD	int
Identifier	a
Operator	=
INT	10
Deliminator	;
KEYWORD	float
Identifier	b
Operator	=
FLOAT	20.5
Deliminator	;
KEYWORD	if
Deliminator	(
Identifier	a
Operator	<=
INT	10
Deliminator	)
KEYWORD	return
Identifier	a
Deliminator	;
```
