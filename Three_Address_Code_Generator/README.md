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
*(On Windows MinGW, run `.\main.exe`)*

## Input Format & Example
The program prompts: `Enter expression:`. It supports arithmetic assignments or expressions involving `+`, `-`, `*`, `/`, and parentheses `( )`.

### Example Input:
```text
x = (a + b) * (c - d) / e
```

### Expected Output:
```text
Tokens:
x = ( a + b ) * ( c - d ) / e 

Three Address Code:
t1 = a + b
t2 = c - d
t3 = t1 * t2
t4 = t3 / e
x = t4

Quadruples Table:
Index	Op	Arg1	Arg2	Result
0	+	a	b	t1
1	-	c	d	t2
2	*	t1	t2	t3
3	/	t3	e	t4
4	=	t4	-	x

Triples Table:
Index	Op		Arg1	Arg2
0	+		a	b
1	-		c	d
2	*		(0)	(1)
3	/		(2)	e
4	=		x	(3)
```
