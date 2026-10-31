#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using namespace std;

enum DataType { TYPE_INT, TYPE_FLOAT, TYPE_STRING, TYPE_BOOL, TYPE_ERROR };

string typeToString(DataType t) {
    switch (t) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_STRING: return "string";
        case TYPE_BOOL: return "bool";
        default: return "type_error";
    }
}

enum NodeKind {
    NODE_VAR_DECL,
    NODE_ASSIGN,
    NODE_BIN_OP,
    NODE_VAR_REF,
    NODE_INT_LIT,
    NODE_FLOAT_LIT,
    NODE_STR_LIT,
    NODE_BLOCK
};

struct ASTNode {
    NodeKind kind;
    string text;
    DataType declaredType;
    vector<shared_ptr<ASTNode>> children;

    ASTNode(NodeKind k, string val = "", DataType dt = TYPE_ERROR)
        : kind(k), text(val), declaredType(dt) {}
};

class SemanticAnalyzer {
private:
    unordered_map<string, DataType> symbolTable;
    int errorCount = 0;

    void logError(const string& msg) {
        cout << "[SEMANTIC ERROR]: " << msg << "\n";
        errorCount++;
    }

public:
    DataType analyze(shared_ptr<ASTNode> node) {
        if (!node) return TYPE_ERROR;

        switch (node->kind) {
            case NODE_INT_LIT:
                return TYPE_INT;

            case NODE_FLOAT_LIT:
                return TYPE_FLOAT;

            case NODE_STR_LIT:
                return TYPE_STRING;

            case NODE_VAR_DECL: {
                string varName = node->text;
                if (symbolTable.count(varName)) {
                    logError("Variable '" + varName + "' is already declared in this scope.");
                } else {
                    symbolTable[varName] = node->declaredType;
                    cout << "[DECL] Registered '" << varName << "' as " << typeToString(node->declaredType) << "\n";
                }

                if (!node->children.empty()) {
                    DataType initType = analyze(node->children[0]);
                    if (initType != TYPE_ERROR && initType != node->declaredType) {
                        logError("Type mismatch initializing '" + varName + "'. Expected " +
                                 typeToString(node->declaredType) + ", got " + typeToString(initType));
                    }
                }
                return TYPE_ERROR;
            }

            case NODE_VAR_REF: {
                string varName = node->text;
                if (!symbolTable.count(varName)) {
                    logError("Variable '" + varName + "' used before declaration.");
                    return TYPE_ERROR;
                }
                return symbolTable[varName];
            }

            case NODE_ASSIGN: {
                string varName = node->text;
                if (!symbolTable.count(varName)) {
                    logError("Assignment to undeclared variable '" + varName + "'.");
                    return TYPE_ERROR;
                }
                DataType expectedType = symbolTable[varName];
                DataType rhsType = analyze(node->children[0]);

                if (rhsType != TYPE_ERROR && rhsType != expectedType) {
                    logError("Cannot assign " + typeToString(rhsType) + " to '" + varName +
                             "' of type " + typeToString(expectedType) + ".");
                } else if (rhsType != TYPE_ERROR) {
                    cout << "[OK] Valid assignment to '" << varName << "'.\n";
                }
                return expectedType;
            }

            case NODE_BIN_OP: {
                DataType leftType = analyze(node->children[0]);
                DataType rightType = analyze(node->children[1]);

                if (leftType == TYPE_ERROR || rightType == TYPE_ERROR) {
                    return TYPE_ERROR;
                }

                string op = node->text;
                if (op == "+" || op == "-" || op == "*" || op == "/") {
                    if (leftType == TYPE_STRING && rightType == TYPE_STRING && op == "+") {
                        return TYPE_STRING; // string concatenation
                    }
                    if ((leftType == TYPE_INT || leftType == TYPE_FLOAT) &&
                        (rightType == TYPE_INT || rightType == TYPE_FLOAT)) {
                        if (leftType == TYPE_FLOAT || rightType == TYPE_FLOAT) return TYPE_FLOAT;
                        return TYPE_INT;
                    }
                    logError("Operator '" + op + "' cannot be applied to " +
                             typeToString(leftType) + " and " + typeToString(rightType) + ".");
                    return TYPE_ERROR;
                }

                if (op == "==" || op == "!=" || op == "<" || op == ">") {
                    if (leftType != rightType) {
                        logError("Comparison between incompatible types: " +
                                 typeToString(leftType) + " and " + typeToString(rightType) + ".");
                        return TYPE_ERROR;
                    }
                    return TYPE_BOOL;
                }
                return TYPE_ERROR;
            }

            case NODE_BLOCK: {
                for (auto& child : node->children) {
                    analyze(child);
                }
                return TYPE_ERROR;
            }
        }
        return TYPE_ERROR;
    }

    void printSummary() {
        cout << "\n----------------------------------------\n";
        if (errorCount == 0) {
            cout << "Result: Semantic Analysis Passed (0 errors).\n";
        } else {
            cout << "Result: Analysis failed with " << errorCount << " error(s).\n";
        }
        cout << "----------------------------------------\n";
    }
};

int main() {
    cout << "=== Semantic Analyzer & Type Checker ===\n\n";

    // Building sample AST:
    // int age = 21;
    // float height = 5.9;
    // string name = "John";
    // age = height;           <-- type mismatch error
    // total = 100;            <-- undeclared variable error
    // int age = 30;           <-- redeclaration error
    // age = age + 5;          <-- valid
    // height = height + name; <-- invalid operation error

    auto root = make_shared<ASTNode>(NODE_BLOCK);

    auto d1 = make_shared<ASTNode>(NODE_VAR_DECL, "age", TYPE_INT);
    d1->children.push_back(make_shared<ASTNode>(NODE_INT_LIT, "21"));
    root->children.push_back(d1);

    auto d2 = make_shared<ASTNode>(NODE_VAR_DECL, "height", TYPE_FLOAT);
    d2->children.push_back(make_shared<ASTNode>(NODE_FLOAT_LIT, "5.9"));
    root->children.push_back(d2);

    auto d3 = make_shared<ASTNode>(NODE_VAR_DECL, "name", TYPE_STRING);
    d3->children.push_back(make_shared<ASTNode>(NODE_STR_LIT, "John"));
    root->children.push_back(d3);

    auto a1 = make_shared<ASTNode>(NODE_ASSIGN, "age");
    a1->children.push_back(make_shared<ASTNode>(NODE_VAR_REF, "height"));
    root->children.push_back(a1);

    auto a2 = make_shared<ASTNode>(NODE_ASSIGN, "total");
    a2->children.push_back(make_shared<ASTNode>(NODE_INT_LIT, "100"));
    root->children.push_back(a2);

    auto d4 = make_shared<ASTNode>(NODE_VAR_DECL, "age", TYPE_INT);
    root->children.push_back(d4);

    auto a3 = make_shared<ASTNode>(NODE_ASSIGN, "age");
    auto bin1 = make_shared<ASTNode>(NODE_BIN_OP, "+");
    bin1->children.push_back(make_shared<ASTNode>(NODE_VAR_REF, "age"));
    bin1->children.push_back(make_shared<ASTNode>(NODE_INT_LIT, "5"));
    a3->children.push_back(bin1);
    root->children.push_back(a3);

    auto a4 = make_shared<ASTNode>(NODE_ASSIGN, "height");
    auto bin2 = make_shared<ASTNode>(NODE_BIN_OP, "+");
    bin2->children.push_back(make_shared<ASTNode>(NODE_VAR_REF, "height"));
    bin2->children.push_back(make_shared<ASTNode>(NODE_VAR_REF, "name"));
    a4->children.push_back(bin2);
    root->children.push_back(a4);

    SemanticAnalyzer analyzer;
    analyzer.analyze(root);
    analyzer.printSummary();

    return 0;
}
