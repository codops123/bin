# Lexical Analyzer (Scanner)

Reads source code, skips comments/spaces, and outputs all recognized tokens (Keywords, Identifiers, Numbers, Literals, Operators, Delimiters) in tabular format.

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program directly reads from a file named `input.txt` in the same working directory.

### Example `input.txt`:
```c
int main() {
    int a = 10;
    float b = 20.5;
    // Calculate sum
    a = a + 5;
    if (a >= 15) {
        printf("Hello World");
    }
    return 0;
}
```

### Expected Output:
```text
Line	Lexeme		Token
-----------------------------------
1	int		Keyword
1	main		Keyword
1	(		Delimiter
1	)		Delimiter
1	{		Delimiter
2	int		Keyword
2	a		Identifier
2	=		Operator
2	10		Integer
2	;		Delimiter
3	float		Keyword
3	b		Identifier
3	=		Operator
3	20.5		Float
3	;		Delimiter
...
```
