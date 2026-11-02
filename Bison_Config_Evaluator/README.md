# Bison Configuration Language Evaluator

Flex + Bison implementation of a simple configuration/scripting language supporting:
- Variable declarations & assignments (numeric & strings)
- Dynamic expression evaluation
- Print statements

## How to Run
```bash
bison -d parser.y
flex lexer.l
g++ parser.tab.c lex.yy.c -o evaluator
./evaluator config.txt
Enter configuration statements:
a = 10;
b = a + 5;
cd Bison_Config_Evaluator
g++ parser.tab.c lex.yy.c -o evaluator.exe

.\evaluator.exe
```


