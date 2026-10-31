# Left Recursion Elimination

Converts direct left-recursive expressions into equivalent LL(1) grammars:
- Splits into $\alpha$ and $\beta$ parts
- Introduces new non-terminals ($E'$, $T'$) and $\varepsilon$-rules

## How to Run
```bash
g++ main.cpp -o main
./main
```
