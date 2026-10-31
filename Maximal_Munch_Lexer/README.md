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
