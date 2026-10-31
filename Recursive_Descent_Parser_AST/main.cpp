#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cctype>
#include <iomanip>

using namespace std;

// -------------------------------------------------------------
// Token Definitions & Lexer
// -------------------------------------------------------------
enum TokenType {
    TOK_ID,
    TOK_NUM,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_EOF
};

struct Token {
    TokenType type;
    string lexeme;
    double val; // for numbers
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
            return {TOK_EOF, "", 0.0};
        }

        char ch = src[pos];

        if (isalpha(ch) || ch == '_') {
            string id = "";
            while (pos < src.length() && (isalnum(src[pos]) || src[pos] == '_')) {
                id += src[pos++];
            }
            return {TOK_ID, id, 0.0};
        }

        if (isdigit(ch)) {
            string numStr = "";
            bool hasDot = false;
            while (pos < src.length() && (isdigit(src[pos]) || (src[pos] == '.' && !hasDot))) {
                if (src[pos] == '.') hasDot = true;
                numStr += src[pos++];
            }
            return {TOK_NUM, numStr, stod(numStr)};
        }

        pos++;
        switch (ch) {
            case '+': return {TOK_PLUS, "+", 0.0};
            case '-': return {TOK_MINUS, "-", 0.0};
            case '*': return {TOK_MUL, "*", 0.0};
            case '/': return {TOK_DIV, "/", 0.0};
            case '(': return {TOK_LPAREN, "(", 0.0};
            case ')': return {TOK_RPAREN, ")", 0.0};
            default:
                cerr << "Lexical error: Unexpected character '" << ch << "'\n";
                return {TOK_EOF, "", 0.0};
        }
    }
};

// -------------------------------------------------------------
// Abstract Syntax Tree (AST) Nodes
// -------------------------------------------------------------
enum NodeType { NODE_OP, NODE_NUM, NODE_ID };

struct ASTNode {
    NodeType nodeType;
    string value;
    double numValue;
    shared_ptr<ASTNode> left;
    shared_ptr<ASTNode> right;

    ASTNode(NodeType t, const string& val) 
        : nodeType(t), value(val), numValue(0.0), left(nullptr), right(nullptr) {}

    ASTNode(double numVal) 
        : nodeType(NODE_NUM), value(to_string(numVal)), numValue(numVal), left(nullptr), right(nullptr) {}
};

// -------------------------------------------------------------
// Recursive-Descent Parser
// Grammar:
//   E  -> T E'
//   E' -> + T E' | - T E' | ε
//   T  -> F T'
//   T' -> * F T' | / F T' | ε
//   F  -> ( E ) | id | num
// -------------------------------------------------------------
class RecursiveDescentParser {
private:
    Lexer lexer;
    Token currentToken;

    void advance() {
        currentToken = lexer.getNextToken();
    }

    void match(TokenType expected) {
        if (currentToken.type == expected) {
            advance();
        } else {
            cerr << "Syntax Error: Unexpected token '" << currentToken.lexeme << "'\n";
            exit(1);
        }
    }

    // F -> ( E ) | id | num
    shared_ptr<ASTNode> parseF() {
        if (currentToken.type == TOK_LPAREN) {
            match(TOK_LPAREN);
            auto node = parseE();
            match(TOK_RPAREN);
            return node;
        } else if (currentToken.type == TOK_ID) {
            auto node = make_shared<ASTNode>(NODE_ID, currentToken.lexeme);
            match(TOK_ID);
            return node;
        } else if (currentToken.type == TOK_NUM) {
            auto node = make_shared<ASTNode>(currentToken.val);
            node->value = currentToken.lexeme;
            match(TOK_NUM);
            return node;
        } else {
            cerr << "Syntax Error in Factor (F): Expected '(', id, or num\n";
            exit(1);
        }
    }

    // T' -> * F T' | / F T' | ε
    shared_ptr<ASTNode> parseTPrime(shared_ptr<ASTNode> inheritedLeft) {
        if (currentToken.type == TOK_MUL) {
            match(TOK_MUL);
            auto right = parseF();
            auto opNode = make_shared<ASTNode>(NODE_OP, "*");
            opNode->left = inheritedLeft;
            opNode->right = right;
            return parseTPrime(opNode);
        } else if (currentToken.type == TOK_DIV) {
            match(TOK_DIV);
            auto right = parseF();
            auto opNode = make_shared<ASTNode>(NODE_OP, "/");
            opNode->left = inheritedLeft;
            opNode->right = right;
            return parseTPrime(opNode);
        }
        // ε - epsilon production
        return inheritedLeft;
    }

    // T -> F T'
    shared_ptr<ASTNode> parseT() {
        auto left = parseF();
        return parseTPrime(left);
    }

    // E' -> + T E' | - T E' | ε
    shared_ptr<ASTNode> parseEPrime(shared_ptr<ASTNode> inheritedLeft) {
        if (currentToken.type == TOK_PLUS) {
            match(TOK_PLUS);
            auto right = parseT();
            auto opNode = make_shared<ASTNode>(NODE_OP, "+");
            opNode->left = inheritedLeft;
            opNode->right = right;
            return parseEPrime(opNode);
        } else if (currentToken.type == TOK_MINUS) {
            match(TOK_MINUS);
            auto right = parseT();
            auto opNode = make_shared<ASTNode>(NODE_OP, "-");
            opNode->left = inheritedLeft;
            opNode->right = right;
            return parseEPrime(opNode);
        }
        // ε - epsilon production
        return inheritedLeft;
    }

    // E -> T E'
    shared_ptr<ASTNode> parseE() {
        auto left = parseT();
        return parseEPrime(left);
    }

public:
    RecursiveDescentParser(const string& input) : lexer(input) {
        advance();
    }

    shared_ptr<ASTNode> parse() {
        auto root = parseE();
        if (currentToken.type != TOK_EOF) {
            cerr << "Warning: Extra tokens after parsing complete expression.\n";
        }
        return root;
    }
};

// -------------------------------------------------------------
// AST Visualizer & Evaluator
// -------------------------------------------------------------
void printAST(const shared_ptr<ASTNode>& root, const string& prefix = "", bool isLeft = true) {
    if (!root) return;

    cout << prefix;
    cout << (isLeft ? "├── " : "└── ");

    if (root->nodeType == NODE_OP) {
        cout << "[Op: " << root->value << "]\n";
    } else if (root->nodeType == NODE_ID) {
        cout << "(Id: " << root->value << ")\n";
    } else {
        cout << "(Num: " << root->value << ")\n";
    }

    printAST(root->left, prefix + (isLeft ? "│   " : "    "), true);
    printAST(root->right, prefix + (isLeft ? "│   " : "    "), false);
}

double evaluateAST(const shared_ptr<ASTNode>& root, const unordered_map<string, double>& symTab) {
    if (!root) return 0.0;

    if (root->nodeType == NODE_NUM) {
        return root->numValue;
    }

    if (root->nodeType == NODE_ID) {
        if (symTab.find(root->value) != symTab.end()) {
            return symTab.at(root->value);
        } else {
            cerr << "Runtime Error: Identifier '" << root->value << "' not found in symbol table.\n";
            return 0.0;
        }
    }

    // Node is an Operator
    double leftVal = evaluateAST(root->left, symTab);
    double rightVal = evaluateAST(root->right, symTab);

    if (root->value == "+") return leftVal + rightVal;
    if (root->value == "-") return leftVal - rightVal;
    if (root->value == "*") return leftVal * rightVal;
    if (root->value == "/") {
        if (rightVal == 0.0) {
            cerr << "Runtime Error: Division by zero!\n";
            return 0.0;
        }
        return leftVal / rightVal;
    }

    return 0.0;
}

void processExpression(const string& expr, const unordered_map<string, double>& symTab) {
    cout << "=================================================================\n";
    cout << " Input Expression: " << expr << "\n";
    cout << "=================================================================\n";

    RecursiveDescentParser parser(expr);
    shared_ptr<ASTNode> astRoot = parser.parse();

    cout << "\n[1] Abstract Syntax Tree (AST):\n";
    printAST(astRoot);

    cout << "\n[2] Evaluation using Symbol Table:\n";
    for (const auto& entry : symTab) {
        cout << "    " << entry.first << " = " << entry.second << "\n";
    }

    double result = evaluateAST(astRoot, symTab);
    cout << "\n>>> Evaluated Result = " << result << "\n\n";
}

int main() {
    // Expression 1: id + id * num
    // Let's set variables: a = 5, b = 4, num = 3 -> Result: 5 + (4 * 3) = 17
    unordered_map<string, double> symTab1 = {
        {"a", 5.0},
        {"b", 4.0}
    };
    processExpression("a + b * 3", symTab1);

    // Expression 2: (id - num) / id
    // Let's set variables: x = 20, num = 8, y = 4 -> Result: (20 - 8) / 4 = 12 / 4 = 3
    unordered_map<string, double> symTab2 = {
        {"x", 20.0},
        {"y", 4.0}
    };
    processExpression("(x - 8) / y", symTab2);

    return 0;
}
