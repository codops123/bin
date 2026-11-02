
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

vector<string> tokens;
int pos = 0;
int tempCount = 1;

struct Instruction {
    string op, arg1, arg2, result;
};

vector<Instruction> tac;

// Function declarations
string expression();
string term();
string factor();

// Lexical Analysis
void tokenize(string s) {

    for (int i = 0; i < s.length(); i++) {

        if (isspace(s[i]))
            continue;

        string word = "";

        if (isalnum(s[i]) || s[i] == '_') {

            while (i < s.length() &&
                  (isalnum(s[i]) || s[i] == '_')) {

                word += s[i];
                i++;
            }

            i--;
            tokens.push_back(word);
        }
        else {
            word += s[i];
            tokens.push_back(word);
        }
    }
}

// Generate temporary variable
string newTemp() {

    string t = "t" + to_string(tempCount);
    tempCount++;

    return t;
}

// Add instruction
string generate(string op, string a, string b) {

    string temp = newTemp();

    tac.push_back({op, a, b, temp});

    return temp;
}

// Factor: variable, number or brackets
string factor() {

    if (pos >= tokens.size())
        throw 1;

    if (tokens[pos] == "(") {

        pos++;

        string result = expression();

        if (pos >= tokens.size() || tokens[pos] != ")")
            throw 1;

        pos++;

        return result;
    }

    if (tokens[pos] == ")" ||
        tokens[pos] == "+" ||
        tokens[pos] == "-" ||
        tokens[pos] == "*" ||
        tokens[pos] == "/" ||
        tokens[pos] == "=")
        throw 1;

    return tokens[pos++];
}

// Multiplication and Division
string term() {

    string left = factor();

    while (pos < tokens.size() &&
          (tokens[pos] == "*" || tokens[pos] == "/")) {

        string op = tokens[pos++];

        string right = factor();

        left = generate(op, left, right);
    }

    return left;
}

// Addition and Subtraction
string expression() {

    string left = term();

    while (pos < tokens.size() &&
          (tokens[pos] == "+" || tokens[pos] == "-")) {

        string op = tokens[pos++];

        string right = term();

        left = generate(op, left, right);
    }

    return left;
}

// Generate Quadruple and Triple
void generateTables() {

    cout << "\nQuadruple Representation:\n";

    cout << "Operator\tArg1\tArg2\tResult\n";

    for (int i = 0; i < tac.size(); i++) {

        cout << tac[i].op << "\t\t"
             << tac[i].arg1 << "\t"
             << tac[i].arg2 << "\t"
             << tac[i].result << endl;
    }

    cout << "\nTriple Representation:\n";

    cout << "Index\tOperator\tArg1\tArg2\n";

    for (int i = 0; i < tac.size(); i++) {

        string a = tac[i].arg1;
        string b = tac[i].arg2;

        // Replace temporary variables with index
        for (int j = 0; j < i; j++) {

            if (a == tac[j].result)
                a = "(" + to_string(j) + ")";

            if (b == tac[j].result)
                b = "(" + to_string(j) + ")";
        }

        cout << i << "\t"
             << tac[i].op << "\t\t"
             << a << "\t"
             << b << endl;
    }
}

int main() {

    string input;

    cout << "Enter expression: ";
    getline(cin, input);

    try {

        // Lexical Analysis
        tokenize(input);

        cout << "\nTokens:\n";

        for (int i = 0; i < tokens.size(); i++)
            cout << tokens[i] << " ";

        cout << endl;

        string lhs = "";
        bool assignment = false;

        // Check assignment
        if (tokens.size() >= 2 && tokens[1] == "=") {

            lhs = tokens[0];

            if (lhs == "=" || lhs == "+" ||
                lhs == "-" || lhs == "*" ||
                lhs == "/")
                throw 1;

            assignment = true;
            pos = 2;
        }

        // Parse expression
        string result = expression();

        // Check remaining tokens
        if (pos != tokens.size())
            throw 1;

        // Final assignment
        if (assignment) {
            tac.push_back({"=", result, "-", lhs});
        }

        // Three Address Code
        cout << "\nThree Address Code:\n";

        for (int i = 0; i < tac.size(); i++) {

            if (tac[i].op == "=") {

                cout << tac[i].result << " = "
                     << tac[i].arg1 << endl;
            }
            else {

                cout << tac[i].result << " = "
                     << tac[i].arg1 << " "
                     << tac[i].op << " "
                     << tac[i].arg2 << endl;
            }
        }

        // Quadruple and Triple
        generateTables();
    }

    catch (...) {
        cout << "\nInvalid Expression!" << endl;
    }

    return 0;
}

