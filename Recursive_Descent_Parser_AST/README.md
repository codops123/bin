# Recursive-Descent Parser with AST

Parses arithmetic expressions with operator precedence and parentheses:
- Builds an Abstract Syntax Tree (AST)
- Visualizes tree structure
- Evaluates values using a symbol table

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program parses an arithmetic expression with operators `+`, `-`, `*`, `/`, parentheses `( )`, and operands named `id` and `num`.
It prompts for:
1. `Enter expression:` (use tokens `id` and `num` as operands)
2. `Enter id value:` (numerical value substituted for `id`)
3. `Enter num value:` (numerical value substituted for `num`)

### Example Input:
```text
(id + num) * id
5
2
```

### Step-by-Step Prompt Walkthrough:
```text
Enter expression: (id + num) * id
Enter id value: 5
Enter num value: 2
```

### Expected Output:
```text
Valid Expression

AST Tree:
*
   +
      id
      num
   id

Result: 35
```
