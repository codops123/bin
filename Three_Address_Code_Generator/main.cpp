#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>

using namespace std;

// -------------------------------------------------------------
// Intermediate Code Representations
// -------------------------------------------------------------

// Three-Address Code Instruction
struct TACInstruction {
    string op;
    string arg1;
    string arg2;
    string result;
};

// Quadruple: (op, arg1, arg2, result)
struct Quadruple {
    int index;
    string op;
    string arg1;
    string arg2;
    string result;
};

// Triple: (op, arg1, arg2) where references to previous results use (i)
struct Triple {
    int index;
    string op;
    string arg1;
    string arg2;
};

// -------------------------------------------------------------
// Lexical Analyzer (Tokens)
// -------------------------------------------------------------
enum TokenType {
    TOK_ID,
    TOK_NUM,
    TOK_ASSIGN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_SEMICOLON,
    TOK_EOF
};

struct Token {
    TokenType type;
    string text;
};

class Lexer {
private:
    string src;
    size_t pos;

public:
    Lexer(const string& input) : src(input), pos(0) {}

    Token getNextToken() {
        while (pos < src.length() && isspace(src[pos])) pos++;

        if (pos >= src.length()) {
            return {TOK_EOF, ""};
        }

        char ch = src[pos];

        if (isalpha(ch) || ch == '_') {
            string id = "";
            while (pos < src.length() && (isalnum(src[pos]) || src[pos] == '_')) {
                id += src[pos++];
            }
            return {TOK_ID, id};
        }

        if (isdigit(ch)) {
            string num = "";
            while (pos < src.length() && (isdigit(src[pos]) || src[pos] == '.')) {
                num += src[pos++];
            }
            return {TOK_NUM, num};
        }

        pos++;
        switch (ch) {
            case '=': return {TOK_ASSIGN, "="};
            case '+': return {TOK_PLUS, "+"};
            case '-': return {TOK_MINUS, "-"};
            case '*': return {TOK_MUL, "*"};
            case '/': return {TOK_DIV, "/"};
            case '(': return {TOK_LPAREN, "("};
            case ')': return {TOK_RPAREN, ")"};
            case ';': return {TOK_SEMICOLON, ";"};
            default:
                cerr << "Error: Unknown character '" << ch << "'\n";
                return {TOK_EOF, ""};
        }
    }
};

// -------------------------------------------------------------
// Recursive-Descent Parser & Code Generator
// -------------------------------------------------------------
class TACGenerator {
private:
    Lexer lexer;
    Token currentToken;
    int tempCount;

    vector<TACInstruction> tacList;
    vector<Quadruple> quadruples;
    vector<Triple> triples;

    void advance() {
        currentToken = lexer.getNextToken();
    }

    void match(TokenType expected) {
        if (currentToken.type == expected) {
            advance();
        } else {
            cerr << "Syntax Error: Unexpected token '" << currentToken.text << "'\n";
            exit(1);
        }
    }

    string newTemp() {
        return "t" + to_string(++tempCount);
    }

    // Factor: F -> ( E ) | id | num
    string parseFactor(string& tripleRef) {
        if (currentToken.type == TOK_LPAREN) {
            match(TOK_LPAREN);
            string res = parseExpression(tripleRef);
            match(TOK_RPAREN);
            return res;
        } else if (currentToken.type == TOK_ID) {
            string id = currentToken.text;
            match(TOK_ID);
            tripleRef = id;
            return id;
        } else if (currentToken.type == TOK_NUM) {
            string num = currentToken.text;
            match(TOK_NUM);
            tripleRef = num;
            return num;
        } else {
            cerr << "Syntax error in Factor\n";
            exit(1);
        }
    }

    // Term: T -> F ( (* | /) F )*
    string parseTerm(string& tripleRef) {
        string tRef1;
        string left = parseFactor(tRef1);

        while (currentToken.type == TOK_MUL || currentToken.type == TOK_DIV) {
            string op = currentToken.text;
            advance();

            string tRef2;
            string right = parseFactor(tRef2);

            string temp = newTemp();

            // Record TAC
            tacList.push_back({op, left, right, temp});

            // Record Quadruple
            quadruples.push_back({(int)quadruples.size(), op, left, right, temp});

            // Record Triple
            int tripIdx = (int)triples.size();
            triples.push_back({tripIdx, op, tRef1, tRef2});

            tRef1 = "(" + to_string(tripIdx) + ")";
            left = temp;
        }

        tripleRef = tRef1;
        return left;
    }

    // Expression: E -> T ( (+ | -) T )*
    string parseExpression(string& tripleRef) {
        string tRef1;
        string left = parseTerm(tRef1);

        while (currentToken.type == TOK_PLUS || currentToken.type == TOK_MINUS) {
            string op = currentToken.text;
            advance();

            string tRef2;
            string right = parseTerm(tRef2);

            string temp = newTemp();

            // Record TAC
            tacList.push_back({op, left, right, temp});

            // Record Quadruple
            quadruples.push_back({(int)quadruples.size(), op, left, right, temp});

            // Record Triple
            int tripIdx = (int)triples.size();
            triples.push_back({tripIdx, op, tRef1, tRef2});

            tRef1 = "(" + to_string(tripIdx) + ")";
            left = temp;
        }

        tripleRef = tRef1;
        return left;
    }

public:
    TACGenerator(const string& input) : lexer(input), tempCount(0) {
        advance();
    }

    // Statement -> id = Expression | Expression
    void parseStatement() {
        if (currentToken.type == TOK_ID) {
            string targetId = currentToken.text;
            advance();

            if (currentToken.type == TOK_ASSIGN) {
                match(TOK_ASSIGN);
                string tripleRef;
                string exprResult = parseExpression(tripleRef);

                // Assignment TAC: targetId = exprResult
                tacList.push_back({"=", exprResult, "", targetId});
                quadruples.push_back({(int)quadruples.size(), "=", exprResult, "-", targetId});
                
                int tripIdx = (int)triples.size();
                triples.push_back({tripIdx, "=", targetId, tripleRef});
            } else {
                cerr << "Error: Expected '=' in assignment statement.\n";
            }
        } else {
            string tripleRef;
            parseExpression(tripleRef);
        }

        if (currentToken.type == TOK_SEMICOLON) {
            advance();
        }
    }

    void displayTAC() {
        cout << "\n=================================================================\n";
        cout << "                  THREE-ADDRESS CODE (TAC)\n";
        cout << "=================================================================\n";
        for (const auto& tac : tacList) {
            if (tac.op == "=") {
                cout << "  " << setw(6) << left << tac.result << " = " << tac.arg1 << "\n";
            } else {
                cout << "  " << setw(6) << left << tac.result << " = " 
                     << tac.arg1 << " " << tac.op << " " << tac.arg2 << "\n";
            }
        }
        cout << "=================================================================\n";
    }

    void displayQuadruples() {
        cout << "\n=================================================================\n";
        cout << "                    QUADRUPLES TABLE\n";
        cout << "=================================================================\n";
        cout << left;
        cout << setw(8)  << "Index"
             << setw(10) << "Op"
             << setw(12) << "Arg1"
             << setw(12) << "Arg2"
             << setw(12) << "Result" << endl;
        cout << "-----------------------------------------------------------------\n";
        for (const auto& q : quadruples) {
            cout << setw(8)  << ("(" + to_string(q.index) + ")")
                 << setw(10) << q.op
                 << setw(12) << q.arg1
                 << setw(12) << (q.arg2 == "" ? "-" : q.arg2)
                 << setw(12) << q.result << endl;
        }
        cout << "=================================================================\n";
    }

    void displayTriples() {
        cout << "\n=================================================================\n";
        cout << "                      TRIPLES TABLE\n";
        cout << "=================================================================\n";
        cout << left;
        cout << setw(8)  << "Index"
             << setw(10) << "Op"
             << setw(16) << "Arg1"
             << setw(16) << "Arg2" << endl;
        cout << "-----------------------------------------------------------------\n";
        for (const auto& t : triples) {
            cout << setw(8)  << ("(" + to_string(t.index) + ")")
                 << setw(10) << t.op
                 << setw(16) << t.arg1
                 << setw(16) << (t.arg2 == "" ? "-" : t.arg2) << endl;
        }
        cout << "=================================================================\n";
    }
};

void runTest(const string& statement) {
    cout << "\n#################################################################\n";
    cout << " Input Statement: " << statement << "\n";
    cout << "#################################################################\n";

    TACGenerator generator(statement);
    generator.parseStatement();
    generator.displayTAC();
    generator.displayQuadruples();
    generator.displayTriples();
}

int main(int argc, char* argv[]) {
    // Test Case 1: As requested in prompt: a = b + c * d
    runTest("a = b + c * d;");

    // Test Case 2: Parenthesized expression: x = (a + b) * (c - d) / e
    runTest("x = (a + b) * (c - d) / e;");

    return 0;
}
