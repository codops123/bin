
#include<iostream>
#include<string>
#include<vector>
#include<cctype>
#include<stdexcept>
using namespace std;

enum TokType{ID,NUM,PLUS,MINUS,MUL,DIV,ASSIGN,LPAREN,RPAREN,END};

struct Token{
    TokType type;
    string text;
};

vector<Token> tokenize(string s){
    vector<Token> tokens;
    int i=0;

    while(i<s.size()){
        char c=s[i];

        if(isspace(c)){
            i++;
            continue;
        }

        if(isalpha(c)||c=='_'){
            string w="";
            while(i<s.size()&&(isalnum(s[i])||s[i]=='_'))
                w+=s[i++];
            tokens.push_back({ID,w});
        }
        else if(isdigit(c)||c=='.'){
            string n="";
            while(i<s.size()&&(isdigit(s[i])||s[i]=='.'))
                n+=s[i++];
            tokens.push_back({NUM,n});
        }
        else{
            if(c=='+')tokens.push_back({PLUS,"+"});
            else if(c=='-')tokens.push_back({MINUS,"-"});
            else if(c=='*')tokens.push_back({MUL,"*"});
            else if(c=='/')tokens.push_back({DIV,"/"});
            else if(c=='=')tokens.push_back({ASSIGN,"="});
            else if(c=='(')tokens.push_back({LPAREN,"("});
            else if(c==')')tokens.push_back({RPAREN,")"});
            else throw runtime_error("Invalid character!");
            i++;
        }
    }

    tokens.push_back({END,""});
    return tokens;
}

struct Quad{
    string op,arg1,arg2,result;
};

vector<Quad> quads;
int tempCount=0;

string newTemp(){
    tempCount++;
    return "t"+to_string(tempCount);
}

void emit(string op,string a,string b,string result){
    quads.push_back({op,a,b,result});
}

class Parser{
    vector<Token> toks;
    int pos=0;

    Token cur(){
        return toks[pos];
    }

    void advance(){
        if(pos<toks.size()-1)
            pos++;
    }

    string factor(){
        if(cur().type==ID||cur().type==NUM){
            string v=cur().text;
            advance();
            return v;
        }

        if(cur().type==LPAREN){
            advance();
            string v=expr();

            if(cur().type!=RPAREN)
                throw runtime_error("Missing closing bracket!");

            advance();
            return v;
        }

        if(cur().type==MINUS){
            advance();
            string v=factor();
            string t=newTemp();
            emit("uminus",v,"",t);
            return t;
        }

        throw runtime_error("Invalid expression!");
    }

    string term(){
        string left=factor();

        while(cur().type==MUL||cur().type==DIV){
            string op=cur().text;
            advance();

            string right=factor();
            string t=newTemp();

            emit(op,left,right,t);
            left=t;
        }

        return left;
    }

    string expr(){
        string left=term();

        while(cur().type==PLUS||cur().type==MINUS){
            string op=cur().text;
            advance();

            string right=term();
            string t=newTemp();

            emit(op,left,right,t);
            left=t;
        }

        return left;
    }

public:
    Parser(vector<Token> t){
        toks=t;
    }

    void parse(){
        if(cur().type==END)
            throw runtime_error("Empty input!");

        if(cur().type==ID&&toks[pos+1].type==ASSIGN){
            string lhs=cur().text;
            advance();
            advance();

            string rhs=expr();
            emit("=",rhs,"",lhs);
        }
        else{
            expr();
        }

        if(cur().type!=END)
            throw runtime_error("Extra tokens found!");
    }
};

void printTAC(){
    cout<<"\nThree Address Code:\n";

    for(int i=0;i<quads.size();i++){
        if(quads[i].op=="=")
            cout<<quads[i].result<<" = "<<quads[i].arg1<<endl;
        else if(quads[i].op=="uminus")
            cout<<quads[i].result<<" = -"<<quads[i].arg1<<endl;
        else
            cout<<quads[i].result<<" = "<<quads[i].arg1<<" "<<quads[i].op<<" "<<quads[i].arg2<<endl;
    }
}

void printQuadruples(){
    cout<<"\nQuadruples:\n";
    cout<<"No.\tOp\tArg1\tArg2\tResult\n";

    for(int i=0;i<quads.size();i++){
        cout<<i<<"\t"<<quads[i].op<<"\t"<<quads[i].arg1<<"\t";

        if(quads[i].arg2=="")
            cout<<"-\t";
        else
            cout<<quads[i].arg2<<"\t";

        cout<<quads[i].result<<endl;
    }
}

void printTriples(){
    cout<<"\nTriples:\n";
    cout<<"No.\tOp\tArg1\tArg2\n";

    vector<string> tempNames;
    vector<int> tempIndex;

    for(int i=0;i<quads.size();i++){
        string a=quads[i].arg1;
        string b=quads[i].arg2;

        for(int j=0;j<tempNames.size();j++){
            if(a==tempNames[j])
                a="("+to_string(tempIndex[j])+")";

            if(b==tempNames[j])
                b="("+to_string(tempIndex[j])+")";
        }

        if(quads[i].op=="="){
            cout<<i<<"\t=\t"<<quads[i].result<<"\t"<<a<<endl;
        }
        else{
            cout<<i<<"\t"<<quads[i].op<<"\t"<<a<<"\t";

            if(b=="")
                cout<<"-"<<endl;
            else
                cout<<b<<endl;

            tempNames.push_back(quads[i].result);
            tempIndex.push_back(i);
        }
    }
}

int main(){
    string line;

    cout<<"COMPILER INTERMEDIATE CODE GENERATOR\n";
    cout<<"Example: a = b + c * d\n";
    cout<<"Press Enter on an empty line to exit.\n";

    while(true){
        cout<<"\nEnter expression: ";
        getline(cin,line);

        if(line.find_first_not_of(" \t")==string::npos){
            cout<<"Program ended.\n";
            break;
        }

        quads.clear();
        tempCount=0;

        try{
            vector<Token> toks=tokenize(line);

            cout<<"\nTokens:\n";

            for(int i=0;i<toks.size();i++){
                if(toks[i].type==END)
                    break;

                string kind="Operator/Symbol";

                if(toks[i].type==ID)
                    kind="Identifier";
                else if(toks[i].type==NUM)
                    kind="Number";

                cout<<toks[i].text<<" ["<<kind<<"]"<<endl;
            }

            Parser p(toks);
            p.parse();

            printTAC();
            printQuadruples();
            printTriples();
        }
        catch(const exception &e){
            cout<<"Error: "<<e.what()<<endl;
        }
    }

    return 0;
}

