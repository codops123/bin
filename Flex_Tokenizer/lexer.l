%{
#include<iostream>
using namespace std;
%}
%%

"int"|"if"|"float"|"main"|"else"|"while"|"return"|"for" {
   cout<<"KEYWORD"<< '\t' <<yytext<<endl;
}

[0-9]+ {
   cout<<"INT"<< '\t' <<yytext<<endl;
} 

[0-9]+(\.[0-9]+) {
   cout<<"FLOAT"<< '\t' <<yytext<<endl;
} 

[a-zA-Z_]*[a-zA-Z0-9_]* {
   cout<<"Identifier"<< '\t' <<yytext<<endl;
}

"=="|"+"|"-"|"*"|"/"|"%"|"="|"!"|"<"|"<="|">"|">="|"!="|"<<"|">>" {
   cout<<"Operator"<< '\t' <<yytext<<endl;
}

[(){};,] {
   cout<<"Deliminator"<< '\t' <<yytext<<endl;
}

[ \t]  ;

'\n'  cout<<'\n'<<endl;

. {
  cout<<yytext[0]<<endl;
  
  }
%%
  
int main(){
     cout<<"Enter your code:\n"<<endl;
     yylex();
       return 0;
}
int yywrap(){
  return 1;
}

/*

flex lexer.l
g++ lex.yy.c -o token
./token

*/
