#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <unordered_set>

using namespace std;

// Token categories
enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INT_CONST,
    FLOAT_CONST,
    OPERATOR,
    DELIMITER,
    UNKNOWN
};

struct TokenRule {
    string name;
    TokenType type;
    regex pattern;
    int priority; // In case length matches equally, higher priority wins
};

struct Token {
    string lexeme;
    string tokenName;
    TokenType type;
    int startIndex;
    int length;
};

class MaximalMunchLexer {
private:
    vector<TokenRule> rules;
    unordered_set<string> keywords = {
        "if", "else", "while", "for", "int", "float", "return", "void"
    };

public:
    MaximalMunchLexer() {
        // Define token rules with regex patterns (anchored with ^)
        // Order / Priority matters when length of two matches is identical
        rules.push_back({"FLOAT_CONST", FLOAT_CONST, regex("^([0-9]+\\.[0-9]+)"), 10});
        rules.push_back({"INT_CONST", INT_CONST, regex("^([0-9]+)"), 9});
        
        // Multi-char operators must compete with single char operators
        // Maximal Munch automatically prefers longer ones: '>>=' over '>>' over '>'
        rules.push_back({"RIGHT_SHIFT_ASSIGN", OPERATOR, regex("^(>>=)"), 8});
        rules.push_back({"LEFT_SHIFT_ASSIGN", OPERATOR, regex("^(<<=)"), 8});
        rules.push_back({"RIGHT_SHIFT", OPERATOR, regex("^(>>)"), 7});
        rules.push_back({"LEFT_SHIFT", OPERATOR, regex("^(<<)"), 7});
        rules.push_back({"INCREMENT", OPERATOR, regex("^(\\+\\+)"), 7});
        rules.push_back({"DECREMENT", OPERATOR, regex("^(--)"), 7});
        rules.push_back({"PLUS_ASSIGN", OPERATOR, regex("^(\\+=)"), 7});
        rules.push_back({"MINUS_ASSIGN", OPERATOR, regex("^(-=)"), 7});
        rules.push_back({"EQ_COMPARE", OPERATOR, regex("^(==)"), 7});
        rules.push_back({"NEQ_COMPARE", OPERATOR, regex("^(!=)"), 7});
        rules.push_back({"LE_COMPARE", OPERATOR, regex("^(<=)"), 7});
        rules.push_back({"GE_COMPARE", OPERATOR, regex("^(>=)"), 7});
        rules.push_back({"LOGICAL_AND", OPERATOR, regex("^(&&)"), 7});
        rules.push_back({"LOGICAL_OR", OPERATOR, regex("^(\\|\\|)"), 7});
        
        // Single character operators
        rules.push_back({"ASSIGN", OPERATOR, regex("^(=)"), 5});
        rules.push_back({"PLUS", OPERATOR, regex("^(\\+)"), 5});
        rules.push_back({"MINUS", OPERATOR, regex("^(-)"), 5});
        rules.push_back({"MULTIPLY", OPERATOR, regex("^(\\*)"), 5});
        rules.push_back({"DIVIDE", OPERATOR, regex("^(/)"), 5});
        rules.push_back({"LESS_THAN", OPERATOR, regex("^(<)"), 5});
        rules.push_back({"GREATER_THAN", OPERATOR, regex("^(>)"), 5});
        rules.push_back({"LOGICAL_NOT", OPERATOR, regex("^(!)"), 5});

        // Delimiters
        rules.push_back({"SEMICOLON", DELIMITER, regex("^(;)"), 4});
        rules.push_back({"COMMA", DELIMITER, regex("^(,)"), 4});
        rules.push_back({"LPAREN", DELIMITER, regex("^(\\()"), 4});
        rules.push_back({"RPAREN", DELIMITER, regex("^(\\))"), 4});
        rules.push_back({"LBRACE", DELIMITER, regex("^(\\{)"), 4});
        rules.push_back({"RBRACE", DELIMITER, regex("^(\\})"), 4});

        // Identifiers and Keywords
        rules.push_back({"IDENTIFIER", IDENTIFIER, regex("^([a-zA-Z_][a-zA-Z0-9_]*)"), 6});
    }

    vector<Token> tokenize(const string& input) {
        vector<Token> tokens;
        int index = 0;
        int n = input.length();

        while (index < n) {
            // Skip whitespace
            if (isspace(input[index])) {
                index++;
                continue;
            }

            string remaining = input.substr(index);
            int longestMatchLength = 0;
            TokenRule* bestRule = nullptr;
            string bestLexeme = "";

            // Check every rule to find the LONGEST match (Maximal Munch)
            for (auto& rule : rules) {
                smatch match;
                if (regex_search(remaining, match, rule.pattern)) {
                    int matchLen = match[0].length();
                    // Maximal Munch Principle: Pick longest match. 
                    // Tie-breaker: rule priority.
                    if (matchLen > longestMatchLength || 
                       (matchLen == longestMatchLength && bestRule && rule.priority > bestRule->priority)) {
                        longestMatchLength = matchLen;
                        bestRule = &rule;
                        bestLexeme = match[0].str();
                    }
                }
            }

            if (longestMatchLength > 0 && bestRule != nullptr) {
                // If it's an IDENTIFIER match, check if it's actually a KEYWORD
                TokenType finalType = bestRule->type;
                string finalName = bestRule->name;

                if (finalType == IDENTIFIER && keywords.find(bestLexeme) != keywords.end()) {
                    finalType = KEYWORD;
                    finalName = "KEYWORD_" + bestLexeme;
                }

                tokens.push_back({bestLexeme, finalName, finalType, index, longestMatchLength});
                index += longestMatchLength; // Advance by the longest match
            } else {
                // Unrecognized character
                tokens.push_back({string(1, input[index]), "UNKNOWN", UNKNOWN, index, 1});
                index++;
            }
        }

        return tokens;
    }

    void display(const vector<Token>& tokens) {
        cout << "\n======================================================================\n";
        cout << "   Maximal Munch (Longest Match) Lexical Analysis Result\n";
        cout << "======================================================================\n";
        cout << left;
        cout << setw(10) << "Index" 
             << setw(10) << "Length" 
             << setw(25) << "Lexeme" 
             << setw(25) << "Token Type / Name" << endl;
        cout << "----------------------------------------------------------------------\n";

        for (const auto& tok : tokens) {
            cout << setw(10) << tok.startIndex
                 << setw(10) << tok.length
                 << setw(25) << ("\"" + tok.lexeme + "\"")
                 << setw(25) << tok.tokenName << endl;
        }
        cout << "======================================================================\n";
    }
};

int main() {
    MaximalMunchLexer lexer;

    // Ambiguity test string:
    // 1) "a+++b" -> should be parsed as 'a', '++', '+', 'b' (NOT 'a', '+', '++', 'b')
    // 2) "x>>=10" -> should be 'x', '>>=', '10' (NOT '>', '>=')
    // 3) "12.34" -> single float token (NOT integer '12' followed by '.' and '34')
    // 4) "ifelse" -> identifier (NOT keyword 'if' and keyword 'else')
    string testCode = "int counter = 10; counter>>=2; a+++b; float val = 12.34; ifelse = 5;";

    cout << "Input Code:\n" << testCode << "\n";
    vector<Token> tokens = lexer.tokenize(testCode);
    lexer.display(tokens);

    return 0;
}
