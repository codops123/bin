# Semantic Analyzer on AST

Performs type checking and semantic validation over an AST:
- Undeclared variable detection
- Variable re-declaration checks
- Type compatibility & assignment mismatch
- Incompatible operand operations

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program asks for:
1. Number of variables
2. Variable names and their types (`int` or `float`)
3. The assignment target variable name
4. The arithmetic expression to validate

### Example 1 (Valid Assignment):
**Inputs:**
```text
2
a int
b int
a
a + b * 2
```

**Walkthrough:**
```text
Enter number of variables: 2
Enter variable name and type: a int
Enter variable name and type: b int

Enter assignment variable: a
Enter expression: a + b * 2

Type of a = int
Expression type = int
Assignment Valid
```

### Example 2 (Type Mismatch):
**Inputs:**
```text
2
x int
y float
x
x + y
```

**Output:**
```text
Type of x = int
Expression type = float
Type Mismatch
```
