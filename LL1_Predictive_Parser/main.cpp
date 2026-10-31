#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <iomanip>

using namespace std;

// Grammar:
// S -> a A B e
// A -> b A c | d
// B -> f | #    (# represents epsilon)

struct Production {
    char lhs;
    string rhs;
};

class LL1Parser {
private:
    vector<char> nonTerminals = {'S', 'A', 'B'};
    vector<char> terminals = {'a', 'b', 'c', 'd', 'e', 'f', '$'};
    vector<Production> productions;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    map<pair<char, char>, string> parseTable; // (Non-Terminal, Terminal) -> RHS

    bool isNonTerminal(char c) {
        for (char nt : nonTerminals) {
            if (nt == c) return true;
        }
        return false;
    }

public:
    LL1Parser() {
        // Register productions
        productions.push_back({'S', "aABe"});
        productions.push_back({'A', "bAc"});
        productions.push_back({'A', "d"});
        productions.push_back({'B', "f"});
        productions.push_back({'B', "#"}); // # is epsilon

        computeFirst();
        computeFollow();
        buildParseTable();
    }

    void computeFirst() {
        // Manual computation strictly for this grammar:
        // FIRST(B) = { 'f', '#' }
        first['B'].insert('f');
        first['B'].insert('#');

        // FIRST(A) = { 'b', 'd' }
        first['A'].insert('b');
        first['A'].insert('d');

        // FIRST(S) = { 'a' }
        first['S'].insert('a');
    }

    void computeFollow() {
        // S is start symbol: FOLLOW(S) = { '$' }
        follow['S'].insert('$');

        // S -> a A B e:
        // FOLLOW(A) = FIRST(B e) = FIRST(B) without epsilon U FIRST(e)
        // FIRST(B) has 'f' and '#'. If B -> #, FIRST(e) = 'e'.
        // Also A -> b A c gives 'c'.
        // So FOLLOW(A) = { 'f', 'e', 'c' }
        follow['A'].insert('f');
        follow['A'].insert('e');
        follow['A'].insert('c');

        // S -> a A B e:
        // FOLLOW(B) = FIRST(e) = { 'e' }
        follow['B'].insert('e');
    }

    set<char> getFirstOfString(const string& str) {
        set<char> result;
        if (str.empty()) {
            result.insert('#');
            return result;
        }

        for (size_t i = 0; i < str.length(); i++) {
            char symbol = str[i];
            if (!isNonTerminal(symbol)) {
                result.insert(symbol);
                break;
            } else {
                set<char> f = first[symbol];
                bool hasEpsilon = false;
                for (char c : f) {
                    if (c == '#') hasEpsilon = true;
                    else result.insert(c);
                }
                if (!hasEpsilon) break;
                if (i == str.length() - 1) result.insert('#');
            }
        }
        return result;
    }

    void buildParseTable() {
        for (const auto& prod : productions) {
            char A = prod.lhs;
            string alpha = prod.rhs;

            if (alpha == "#") {
                // Rule 2: For each terminal 'b' in FOLLOW(A), M[A, b] = A -> #
                for (char b : follow[A]) {
                    parseTable[{A, b}] = alpha;
                }
            } else {
                // Rule 1: For each terminal 'a' in FIRST(alpha), M[A, a] = A -> alpha
                set<char> firstAlpha = getFirstOfString(alpha);
                for (char a : firstAlpha) {
                    if (a != '#') {
                        parseTable[{A, a}] = alpha;
                    }
                }
                // If FIRST(alpha) contains epsilon, add to FOLLOW(A)
                if (firstAlpha.count('#')) {
                    for (char b : follow[A]) {
                        parseTable[{A, b}] = alpha;
                    }
                }
            }
        }
    }

    void displaySetsAndTable() {
        cout << "====================================================================\n";
        cout << "                    FIRST & FOLLOW SETS\n";
        cout << "====================================================================\n";
        for (char nt : nonTerminals) {
            cout << "FIRST(" << nt << ")  = { ";
            for (char c : first[nt]) cout << (c == '#' ? "ε" : string(1, c)) << " ";
            cout << "}\n";

            cout << "FOLLOW(" << nt << ") = { ";
            for (char c : follow[nt]) cout << c << " ";
            cout << "}\n";
        }

        cout << "\n====================================================================\n";
        cout << "                   LL(1) PARSING TABLE\n";
        cout << "====================================================================\n";
        cout << setw(10) << "NT / T";
        for (char t : terminals) {
            cout << setw(10) << t;
        }
        cout << "\n--------------------------------------------------------------------\n";

        for (char nt : nonTerminals) {
            cout << setw(10) << nt;
            for (char t : terminals) {
                if (parseTable.find({nt, t}) != parseTable.end()) {
                    string prod = string(1, nt) + "->" + (parseTable[{nt, t}] == "#" ? "ε" : parseTable[{nt, t}]);
                    cout << setw(10) << prod;
                } else {
                    cout << setw(10) << "-";
                }
            }
            cout << "\n";
        }
        cout << "====================================================================\n";
    }

    void parseInput(string input) {
        if (input.back() != '$') input += '$';
        cout << "\n>>> Parsing Input String: \"" << input << "\"\n";
        cout << left;
        cout << setw(5) << "Step" 
             << setw(20) << "Stack" 
             << setw(20) << "Input" 
             << setw(25) << "Action" << endl;
        cout << "--------------------------------------------------------------------\n";

        stack<char> st;
        st.push('$');
        st.push('S'); // start symbol

        int ip = 0;
        int step = 1;

        while (!st.empty()) {
            char top = st.top();

            // Stack representation for printing
            stack<char> temp = st;
            string stackStr = "";
            while (!temp.empty()) {
                stackStr += temp.top();
                temp.pop();
            }

            string inputRemaining = input.substr(ip);

            cout << setw(5) << step++
                 << setw(20) << stackStr
                 << setw(20) << inputRemaining;

            char currentSymbol = input[ip];

            if (top == currentSymbol) {
                if (top == '$') {
                    cout << setw(25) << "Accept! Input Valid." << endl;
                    break;
                }
                cout << setw(25) << ("Matched terminal '" + string(1, top) + "'") << endl;
                st.pop();
                ip++;
            } else if (!isNonTerminal(top)) {
                cout << setw(25) << "Error: Terminal mismatch" << endl;
                return;
            } else {
                // Table Lookup
                if (parseTable.find({top, currentSymbol}) != parseTable.end()) {
                    string rhs = parseTable[{top, currentSymbol}];
                    st.pop();
                    string action = string(1, top) + " -> " + (rhs == "#" ? "ε" : rhs);
                    cout << setw(25) << action << endl;

                    // Push rhs in reverse order if not epsilon
                    if (rhs != "#") {
                        for (int k = rhs.length() - 1; k >= 0; k--) {
                            st.push(rhs[k]);
                        }
                    }
                } else {
                    cout << setw(25) << ("Error: No rule M[" + string(1, top) + ", " + currentSymbol + "]") << endl;
                    return;
                }
            }
        }
        cout << "====================================================================\n";
    }
};

int main() {
    LL1Parser parser;
    parser.displaySetsAndTable();

    // Valid tests:
    // S -> a A B e -> a d f e  (with A->d, B->f)
    parser.parseInput("adfe");

    // S -> a A B e -> a b d c e (with A->bAc->bdc, B->#)
    parser.parseInput("abdce");

    return 0;
}
