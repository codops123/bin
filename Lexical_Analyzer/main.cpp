#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cctype>
#include <iomanip>

using namespace std;

enum TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING_LITERAL,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_UNKNOWN
};

struct Token {
    string lexeme;
    TokenType type;
    int line;
};

class LexicalAnalyzer {
private:
    unordered_set<string> keywords = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };

    unordered_set<string> operators = {
        "+", "-", "*", "/", "%", "=", "+=", "-=", "*=", "/=",
        "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "++", "--"
    };

    unordered_set<char> delimiters = {
        ';', ',', '(', ')', '{', '}', '[', ']'
    };

    string tokenTypeToString(TokenType type) {
        switch (type) {
            case TOKEN_KEYWORD:        return "KEYWORD";
            case TOKEN_IDENTIFIER:     return "IDENTIFIER";
            case TOKEN_NUMBER:         return "NUMBER_CONSTANT";
            case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
            case TOKEN_OPERATOR:       return "OPERATOR";
            case TOKEN_DELIMITER:      return "DELIMITER";
            default:                   return "UNKNOWN";
        }
    }

public:
    vector<Token> tokenize(const string& source) {
        vector<Token> tokens;
        int i = 0;
        int n = source.length();
        int line = 1;

        while (i < n) {
            char ch = source[i];

            // Handle newline & track line numbers
            if (ch == '\n') {
                line++;
                i++;
                continue;
            }

            // Skip whitespaces
            if (isspace(ch)) {
                i++;
                continue;
            }

            // Skip single-line comments //...
            if (ch == '/' && i + 1 < n && source[i + 1] == '/') {
                i += 2;
                while (i < n && source[i] != '\n') i++;
                continue;
            }

            // Skip multi-line comments /* ... */
            if (ch == '/' && i + 1 < n && source[i + 1] == '*') {
                i += 2;
                while (i + 1 < n && !(source[i] == '*' && source[i + 1] == '/')) {
                    if (source[i] == '\n') line++;
                    i++;
                }
                i += 2; // skip */
                continue;
            }

            // String literals "..."
            if (ch == '"') {
                string lit = "\"";
                i++;
                while (i < n && source[i] != '"') {
                    if (source[i] == '\\' && i + 1 < n) {
                        lit += source[i++];
                    }
                    lit += source[i++];
                }
                if (i < n && source[i] == '"') {
                    lit += "\"";
                    i++;
                }
                tokens.push_back({lit, TOKEN_STRING_LITERAL, line});
                continue;
            }

            // Identifiers and Keywords
            if (isalpha(ch) || ch == '_') {
                string word = "";
                while (i < n && (isalnum(source[i]) || source[i] == '_')) {
                    word += source[i++];
                }
                if (keywords.find(word) != keywords.end()) {
                    tokens.push_back({word, TOKEN_KEYWORD, line});
                } else {
                    tokens.push_back({word, TOKEN_IDENTIFIER, line});
                }
                continue;
            }

            // Numbers (Integer & Floating point)
            if (isdigit(ch)) {
                string num = "";
                bool hasDot = false;
                while (i < n && (isdigit(source[i]) || (source[i] == '.' && !hasDot))) {
                    if (source[i] == '.') hasDot = true;
                    num += source[i++];
                }
                tokens.push_back({num, TOKEN_NUMBER, line});
                continue;
            }

            // Delimiters
            if (delimiters.find(ch) != delimiters.end()) {
                tokens.push_back({string(1, ch), TOKEN_DELIMITER, line});
                i++;
                continue;
            }

            // Multi-char & single-char operators
            if (i + 1 < n) {
                string twoChar = source.substr(i, 2);
                if (operators.find(twoChar) != operators.end()) {
                    tokens.push_back({twoChar, TOKEN_OPERATOR, line});
                    i += 2;
                    continue;
                }
            }

            if (operators.find(string(1, ch)) != operators.end()) {
                tokens.push_back({string(1, ch), TOKEN_OPERATOR, line});
                i++;
                continue;
            }

            // Unknown token
            tokens.push_back({string(1, ch), TOKEN_UNKNOWN, line});
            i++;
        }

        return tokens;
    }

    void printTokensTable(const vector<Token>& tokens) {
        cout << "\n+------+-------------------------+-------------------------+\n";
        cout << "| Line | Lexeme                  | Token Type              |\n";
        cout << "+------+-------------------------+-------------------------+\n";

        for (const auto& tok : tokens) {
            cout << "| " << setw(4) << right << tok.line << " | "
                 << setw(23) << left << tok.lexeme << " | "
                 << setw(23) << left << tokenTypeToString(tok.type) << " |\n";
        }

        cout << "+------+-------------------------+-------------------------+\n";
        cout << "Total Tokens Recognized: " << tokens.size() << "\n\n";
    }
};

int main(int argc, char* argv[]) {
    string filename = "input.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    string sourceCode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    LexicalAnalyzer scanner;
    vector<Token> tokens = scanner.tokenize(sourceCode);
    scanner.printTokensTable(tokens);

    return 0;
}
