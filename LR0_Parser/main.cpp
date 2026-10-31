#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <iomanip>

using namespace std;

/*
 * Augmented Grammar:
 * (0) S' -> S
 * (1) S  -> A
 * (2) A  -> A b
 * (3) A  -> a
 * 
 * Canonical Collection of LR(0) Items:
 * 
 * I0:
 *   S' -> .S
 *   S  -> .A
 *   A  -> .A b
 *   A  -> .a
 * 
 * GOTO(I0, S) = I1:
 *   S' -> S.   (Accept on all terminals for LR(0), specifically $)
 * 
 * GOTO(I0, A) = I2:
 *   S  -> A.   (Reduce by (1): S -> A)
 *   A  -> A.b
 * 
 * GOTO(I0, a) = I3:
 *   A  -> a.   (Reduce by (3): A -> a)
 * 
 * GOTO(I2, b) = I4:
 *   A  -> A b. (Reduce by (2): A -> A b)
 * 
 * Note on LR(0) vs SLR(1):
 * In State 2:
 * S -> A. is a reduction by rule 1: S -> A.
 * A -> A.b has a shift on 'b'.
 * In pure LR(0), state 2 has a Shift-Reduce conflict on terminal 'b'.
 * Under SLR(1), FOLLOW(S) = { $ }, so reduce rule (1) is only on $, resolving the conflict (Shift on b, Reduce 1 on $).
 * We display both the pure LR(0) analysis and resolve it deterministically using SLR(1) so parsing succeeds smoothly!
 */

struct LRAction {
    enum Type { SHIFT, REDUCE, ACCEPT, ERROR, CONFLICT } type;
    int stateOrRule; // state if shift, rule number if reduce
    string desc;
};

struct Production {
    char lhs;
    string rhs;
    int len;
};

class LR0Parser {
private:
    vector<Production> grammar = {
        {' ', "S", 1},  // (0) S' -> S
        {'S', "A", 1},  // (1) S -> A
        {'A', "Ab", 2}, // (2) A -> Ab
        {'A', "a", 1}   // (3) A -> a
    };

    vector<char> terminals = {'a', 'b', '$'};
    vector<char> nonTerminals = {'S', 'A'};

    // Parsing Table:
    // Action[State][Terminal]
    map<pair<int, char>, LRAction> actionTable;
    // Goto[State][NonTerminal]
    map<pair<int, char>, int> gotoTable;

public:
    LR0Parser() {
        buildTable();
    }

    void buildTable() {
        // State 0
        actionTable[{0, 'a'}] = {LRAction::SHIFT, 3, "s3"};
        gotoTable[{0, 'S'}] = 1;
        gotoTable[{0, 'A'}] = 2;

        // State 1
        actionTable[{1, '$'}] = {LRAction::ACCEPT, 0, "acc"};

        // State 2:
        // A -> A . b (Shift on b to State 4)
        // S -> A . (Reduce rule 1: S -> A, on FOLLOW(S) = {$})
        actionTable[{2, 'b'}] = {LRAction::SHIFT, 4, "s4"};
        actionTable[{2, '$'}] = {LRAction::REDUCE, 1, "r1"};

        // State 3: A -> a. (Reduce rule 3: A -> a)
        actionTable[{3, 'a'}] = {LRAction::REDUCE, 3, "r3"};
        actionTable[{3, 'b'}] = {LRAction::REDUCE, 3, "r3"};
        actionTable[{3, '$'}] = {LRAction::REDUCE, 3, "r3"};

        // State 4: A -> A b . (Reduce rule 2: A -> A b)
        actionTable[{4, 'a'}] = {LRAction::REDUCE, 2, "r2"};
        actionTable[{4, 'b'}] = {LRAction::REDUCE, 2, "r2"};
        actionTable[{4, '$'}] = {LRAction::REDUCE, 2, "r2"};
    }

    void displayTable() {
        cout << "====================================================================\n";
        cout << "                   LR CANONICAL ITEMS & PARSE TABLE\n";
        cout << "====================================================================\n";
        cout << "Grammar Rules:\n";
        cout << " (0) S' -> S\n";
        cout << " (1) S  -> A\n";
        cout << " (2) A  -> A b\n";
        cout << " (3) A  -> a\n\n";

        cout << "Canonical Sets of LR(0) Items:\n";
        cout << "  I0 = { S' -> .S, S -> .A, A -> .Ab, A -> .a }\n";
        cout << "  I1 = GOTO(I0, S) = { S' -> S. }\n";
        cout << "  I2 = GOTO(I0, A) = { S -> A., A -> A.b }\n";
        cout << "  I3 = GOTO(I0, a) = { A -> a. }\n";
        cout << "  I4 = GOTO(I2, b) = { A -> Ab. }\n\n";

        cout << "Parsing Table (SLR/LR0 with disambiguated FOLLOW(S)={$}):\n";
        cout << setw(8) << "State" 
             << setw(8) << "a" << setw(8) << "b" << setw(8) << "$" 
             << " | " << setw(8) << "S" << setw(8) << "A" << "\n";
        cout << "--------------------------------------------------------------------\n";

        for (int s = 0; s <= 4; s++) {
            cout << setw(8) << s;
            for (char t : terminals) {
                if (actionTable.find({s, t}) != actionTable.end()) {
                    cout << setw(8) << actionTable[{s, t}].desc;
                } else {
                    cout << setw(8) << "-";
                }
            }
            cout << " | ";
            for (char nt : nonTerminals) {
                if (gotoTable.find({s, nt}) != gotoTable.end()) {
                    cout << setw(8) << gotoTable[{s, nt}];
                } else {
                    cout << setw(8) << "-";
                }
            }
            cout << "\n";
        }
        cout << "====================================================================\n";
    }

    void parse(string input) {
        if (input.back() != '$') input += '$';
        cout << "\n>>> Parsing Input String: \"" << input << "\"\n";
        cout << left;
        cout << setw(6)  << "Step"
             << setw(20) << "Stack (States)"
             << setw(16) << "Symbols"
             << setw(16) << "Input"
             << setw(24) << "Action" << endl;
        cout << "--------------------------------------------------------------------\n";

        stack<int> stateStack;
        stack<char> symbolStack;
        stateStack.push(0);
        symbolStack.push('$');

        int ip = 0;
        int step = 1;

        while (true) {
            int currentState = stateStack.top();
            char currentSymbol = input[ip];

            // Format Stack Strings for output
            stack<int> tempState = stateStack;
            vector<int> stStates;
            while (!tempState.empty()) { stStates.push_back(tempState.top()); tempState.pop(); }
            string stateStr = "";
            for (int i = (int)stStates.size() - 1; i >= 0; i--) {
                stateStr += to_string(stStates[i]) + " ";
            }

            stack<char> tempSym = symbolStack;
            string symStr = "";
            while (!tempSym.empty()) { symStr = string(1, tempSym.top()) + symStr; tempSym.pop(); }

            string inRem = input.substr(ip);

            cout << setw(6)  << step++
                 << setw(20) << stateStr
                 << setw(16) << symStr
                 << setw(16) << inRem;

            if (actionTable.find({currentState, currentSymbol}) == actionTable.end()) {
                cout << setw(24) << "Error: No action defined" << endl;
                break;
            }

            LRAction act = actionTable[{currentState, currentSymbol}];

            if (act.type == LRAction::SHIFT) {
                cout << setw(24) << ("Shift to state " + to_string(act.stateOrRule)) << endl;
                symbolStack.push(currentSymbol);
                stateStack.push(act.stateOrRule);
                ip++;
            } else if (act.type == LRAction::REDUCE) {
                int ruleNum = act.stateOrRule;
                Production prod = grammar[ruleNum];
                string actionDesc = "Reduce by " + string(1, prod.lhs) + " -> " + prod.rhs;
                cout << setw(24) << actionDesc << endl;

                // Pop length of RHS from stacks
                for (int i = 0; i < prod.len; i++) {
                    stateStack.pop();
                    symbolStack.pop();
                }

                // Push LHS
                symbolStack.push(prod.lhs);
                int topState = stateStack.top();

                if (gotoTable.find({topState, prod.lhs}) == gotoTable.end()) {
                    cerr << "Error in GOTO lookup\n";
                    return;
                }
                int nextState = gotoTable[{topState, prod.lhs}];
                stateStack.push(nextState);
            } else if (act.type == LRAction::ACCEPT) {
                cout << setw(24) << "Accept! Input Valid." << endl;
                break;
            }
        }
        cout << "====================================================================\n";
    }
};

int main() {
    LR0Parser parser;
    parser.displayTable();
    
    // Parse input string "abb"
    parser.parse("abb");

    return 0;
}
