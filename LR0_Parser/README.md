# LR(0) Parser & String Simulator

Builds canonical LR(0) items, constructs parsing table, and parses strings like `abb$` using stack simulation.

## How to Run
```bash
g++ main.cpp -o main
./main
```
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program displays the LR(0) and SLR(1) parsing tables for the grammar:
```text
S -> A
A -> Ab | a
```
It then prompts: `enter input string:`.

> **Note:** Enter the terminal string **without** `$`. The code automatically appends `$` internally.

### Valid Input Example:
```text
abb
```

### Execution Output:
```text
enter input string: abb

Stack		Input		Action
0		abb$		shift s3
0a3		bb$		reduce A -> a
0A2		bb$		shift s4
0A2b4		b$		reduce A -> Ab
0A2		b$		shift s4
0A2b4		$		reduce A -> Ab
0A2		$		reduce S -> A
0S1		$		ACCEPTED
```

### Invalid Input Example:
```text
ba
```
**Output:**
```text
enter input string: ba

Stack		Input		Action
0		ba$		REJECTED
```
