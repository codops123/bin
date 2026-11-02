# Left Recursion Elimination

Converts direct left-recursive expressions into equivalent LL(1) grammars:
- Splits into $\alpha$ and $\beta$ parts
- Introduces new non-terminals ($E'$, $T'$) and $\varepsilon$-rules

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program prompts for:
1. Number of productions $n$
2. For each production: the non-terminal symbol, followed by all alternative right-hand-side rules separated by `|` without spaces.

### Example Input:
```text
2
E
E+T|T
T
T*F|F
```

### Step-by-Step Prompt Walkthrough:
```text
enter number of productions: 2

enter non-terminal: E
enter productions: E+T|T

enter non-terminal: T
enter productions: T*F|F
```

### Expected Output:
```text
output:
E -> TE'
E' -> +TE' | eps
T -> FT'
T' -> *FT' | eps
```
