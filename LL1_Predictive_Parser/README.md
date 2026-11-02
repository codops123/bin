# LL(1) Predictive Parser

Implements:
- FIRST & FOLLOW set calculations
- LL(1) parse table construction
- Stack-based table-driven predictive parsing trace

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program first prints the computed `FIRST` and `FOLLOW` sets along with the LL(1) parsing table for the grammar:
```text
S -> aABe
A -> bAc | d
B -> f | eps
```
Then, it prompts for an input string to parse against the table.

### Valid Input Example:
```text
adfe
```
*(Or strings such as `abdce`, `ade`, `abdcfe`)*

### Execution Trace:
```text
Enter input string: adfe
Matched: a
S -> aABe
A -> d
Matched: d
B -> f
Matched: f
Matched: e
Accepted
```

### Invalid Input Example:
```text
abce
```
**Expected Output:**
```text
Enter input string: abce
...
Rejected
```
