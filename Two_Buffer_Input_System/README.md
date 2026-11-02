# Two-Buffer Input System

Simulates the compiler front-end input buffering scheme:
- Two buffer pairs with end-of-buffer sentinels (`\0`)
- `forward` pointer and lookahead retract mechanism
- Lexeme recognition from `input.txt`

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program reads source text directly from a file named `input.txt` located in the same directory, buffering it in chunks of 10 bytes (`SIZE = 10`).

### Example `input.txt`:
```c
int main() {
    int count = 10;
    float rate = 5.5;
    if (count >= 10) {
        return count;
    } else {
        return 0;
    }
}
```

### Expected Output:
```text
Line	Lexeme		Token
--------------------------------
1	int		Keyword
1	main		Identifier
2	int		Keyword
2	count		Identifier
3	float		Keyword
3	rate		Identifier
4	if		Keyword
4	count		Identifier
5	return		Keyword
5	count		Identifier
6	else		Keyword
7	return		Keyword
```
