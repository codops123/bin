#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

/*
 * Problem:
 * Transform the following non-LL(1) / left-recursive grammar into equivalent LL(1) form:
 * 
 * Original Grammar:
 * E -> E + T | T
 * T -> T * F | F
 * F -> ( E ) | id
 * 
 * Standard Left Recursion Elimination Formula:
 * For A -> A α1 | A α2 | ... | β1 | β2 | ...
 * Replace with:
 * A  -> β1 A' | β2 A' | ...
 * A' -> α1 A' | α2 A' | ... | ε
 */

struct GrammarRule {
    string lhs;
    vector<string> rhsList;
};

class LeftRecursionEliminator {
private:
    vector<GrammarRule> originalGrammar;
    vector<GrammarRule> transformedGrammar;

public:
    LeftRecursionEliminator() {
        originalGrammar.push_back({"E", {"E + T", "T"}});
        originalGrammar.push_back({"T", {"T * F", "F"}});
        originalGrammar.push_back({"F", {"( E )", "id"}});
    }

    void eliminateDirectLeftRecursion() {
        for (const auto& rule : originalGrammar) {
            string A = rule.lhs;
            vector<string> alphas;
            vector<string> betas;

            for (const string& prod : rule.rhsList) {
                // Check if starts with non-terminal A
                if (prod.length() >= A.length() && prod.substr(0, A.length()) == A) {
                    // Left recursive: A -> A alpha
                    // Extract alpha (skip A and possible spaces)
                    string alpha = prod.substr(A.length());
                    while (!alpha.empty() && alpha.front() == ' ') alpha.erase(0, 1);
                    alphas.push_back(alpha);
                } else {
                    betas.push_back(prod);
                }
            }

            if (!alphas.empty()) {
                // Left recursion detected
                string APrime = A + "'";
                
                // New rule: A -> beta A'
                vector<string> newA_RHS;
                for (const string& b : betas) {
                    newA_RHS.push_back(b + " " + APrime);
                }
                transformedGrammar.push_back({A, newA_RHS});

                // New rule: A' -> alpha A' | ε
                vector<string> newAPrime_RHS;
                for (const string& a : alphas) {
                    newAPrime_RHS.push_back(a + " " + APrime);
                }
                newAPrime_RHS.push_back("ε");
                transformedGrammar.push_back({APrime, newAPrime_RHS});
            } else {
                // No left recursion, keep as is
                transformedGrammar.push_back(rule);
            }
        }
    }

    void display() {
        cout << "====================================================================\n";
        cout << "    Original Non-LL(1) (Immediate Left-Recursive) Grammar\n";
        cout << "====================================================================\n";
        for (const auto& rule : originalGrammar) {
            cout << "  " << rule.lhs << " -> ";
            for (size_t i = 0; i < rule.rhsList.size(); i++) {
                cout << rule.rhsList[i] << (i + 1 < rule.rhsList.size() ? " | " : "");
            }
            cout << "\n";
        }

        cout << "\n====================================================================\n";
        cout << "       Transformed Equivalent LL(1) Grammar (No Left-Recursion)\n";
        cout << "====================================================================\n";
        for (const auto& rule : transformedGrammar) {
            cout << "  " << setw(3) << left << rule.lhs << " -> ";
            for (size_t i = 0; i < rule.rhsList.size(); i++) {
                cout << rule.rhsList[i] << (i + 1 < rule.rhsList.size() ? " | " : "");
            }
            cout << "\n";
        }
        cout << "====================================================================\n";

        cout << "\nWhy the Transformed Grammar is LL(1):\n";
        cout << "1. Left Recursion has been completely eliminated.\n";
        cout << "2. For E' -> + T E' | ε:\n";
        cout << "   FIRST(+ T E') = { '+' }, FOLLOW(E') = { ')', '$' }\n";
        cout << "   FIRST ∩ FOLLOW = ∅ (Disjoint -> No parsing conflict).\n";
        cout << "3. For T' -> * F T' | ε:\n";
        cout << "   FIRST(* F T') = { '*' }, FOLLOW(T') = { '+', ')', '$' }\n";
        cout << "   FIRST ∩ FOLLOW = ∅ (Disjoint -> No parsing conflict).\n";
        cout << "====================================================================\n";
    }
};

int main() {
    LeftRecursionEliminator eliminator;
    eliminator.eliminateDirectLeftRecursion();
    eliminator.display();

    return 0;
}
