# Two-Buffer Input System

Simulates the compiler front-end input buffering scheme:
- Two buffer pairs with end-of-buffer sentinels (`\0`)
- `forward` pointer and lookahead retract mechanism
- Lexeme recognition from `input.txt`

## How to Run
```bash
g++ main.cpp -o main
./main input.txt
```
