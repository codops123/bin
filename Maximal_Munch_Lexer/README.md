# Maximal Munch Lexer

Demonstrates the Longest Match / Maximal Munch rule to disambiguate tokens such as:
- `a+++b` -> `a`, `++`, `+`, `b`
- `>>=` as a single token
- Float vs int constant handling

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program prompts: `Enter expression:`. It uses the maximal munch (longest match) rule to greedily partition consecutive operator symbols and identifiers.

### Example 1:
**Input:**
```text
a+++b
```
**Expected Output:**
```text
a
++
+
b
```

### Example 2:
**Input:**
```text
x>>=y+1
```
**Expected Output:**
```text
x
>>=
y
+
1
```
