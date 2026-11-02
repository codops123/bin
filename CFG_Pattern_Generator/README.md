# CFG Pattern Generator

Analyzes the terminal string `aabaabaa`, constructs the CFG, expresses the language in terms of $n > 0$ (`(aab)^n aa`), and demonstrates derivation steps.

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program asks for a string and checks if it matches the pattern $(aab)^n aa$ for $n \ge 1$ (consisting of one or more repetitions of `aab` followed by `aa`).

### Example 1 (Accepted):
**Input:**
```text
aabaa
```
**Expected Output:**
```text
Enter string: Accepted
```

### Example 2 (Accepted):
**Input:**
```text
aabaabaa
```
**Expected Output:**
```text
Enter string: Accepted
```

### Example 3 (Rejected):
**Input:**
```text
aabaab
```
**Expected Output:**
```text
Enter string: Rejected
```
