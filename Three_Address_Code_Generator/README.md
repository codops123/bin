# Three-Address Code (TAC) Generator

Parses arithmetic assignments and generates:
- Three-Address Code with temporary variables (`t1`, `t2`, ...)
- Quadruples table `(op, arg1, arg2, result)`
- Triples table `(op, arg1, arg2)`

## How to Run
```bash
g++ main.cpp -o main
./main
```
