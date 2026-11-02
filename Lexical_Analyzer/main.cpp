
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

bool isKeyword(string word) {
    string keywords[] = {
        "int", "float", "char", "double", "void",
        "if", "else", "while", "for", "return",
        "main", "break", "continue"
    };

    for (string keyword : keywords) {
        if (word == keyword)
            return true;
    }
    return false;
}

int main() {
    ifstream file("input.txt");

    if (!file) {
        cout << "File not found!";
        return 1;
    }

    string s, word;
    int line = 0;
    int i, n;

    cout << "Line\tLexeme\t\tToken\n";
    cout << "-----------------------------------\n";

    while (getline(file, s)) {
        line++;
        i = 0;
        n = s.length();

        while (i < n) {

            if (isspace(s[i])) {
                i++;
                continue;
            }

            // Comments
            if (s[i] == '/' && i + 1 < n && s[i + 1] == '/')
                break;

            if (s[i] == '/' && i + 1 < n && s[i + 1] == '*') {
                i += 2;

                while (i + 1 < n && !(s[i] == '*' && s[i + 1] == '/'))
                    i++;

                if (i + 1 < n)
                    i += 2;

                continue;
            }

            // String literal
            if (s[i] == '"') {
                word = "";
                word += s[i++];

                while (i < n) {
                    word += s[i];

                    if (s[i] == '\\' && i + 1 < n) {
                        i++;
                        word += s[i];
                    }
                    else if (s[i] == '"') {
                        i++;
                        break;
                    }

                    i++;
                }

                cout << line << "\t" << word << "\tString Literal\n";
                continue;
            }

            // Character literal
            if (s[i] == '\'') {
                word = "";
                word += s[i++];

                while (i < n) {
                    word += s[i];

                    if (s[i] == '\\' && i + 1 < n) {
                        i++;
                        word += s[i];
                    }
                    else if (s[i] == '\'') {
                        i++;
                        break;
                    }

                    i++;
                }

                cout << line << "\t" << word << "\tCharacter Literal\n";
                continue;
            }

            // Keyword or Identifier
            if (isalpha(s[i]) || s[i] == '_') {
                word = "";

                while (i < n && (isalnum(s[i]) || s[i] == '_'))
                    word += s[i++];

                if (isKeyword(word))
                    cout << line << "\t" << word << "\t\tKeyword\n";
                else
                    cout << line << "\t" << word << "\t\tIdentifier\n";

                continue;
            }

            // Number
            if (isdigit(s[i])) {
                word = "";

                while (i < n && isdigit(s[i]))
                    word += s[i++];

                if (i < n && s[i] == '.') {
                    word += s[i++];

                    while (i < n && isdigit(s[i]))
                        word += s[i++];
                }

                cout << line << "\t" << word << "\t\tNumber\n";
                continue;
            }

            // Two-character operators
            if (i + 1 < n &&
                (s[i + 1] == '=' ||
                (s[i] == s[i + 1] &&
                (s[i] == '+' || s[i] == '-' ||
                 s[i] == '&' || s[i] == '|')))) {

                word = s.substr(i, 2);
                cout << line << "\t" << word << "\t\tOperator\n";
                i += 2;
                continue;
            }

            // Single-character operators
            if (s[i] == '+' || s[i] == '-' || s[i] == '*' ||
                s[i] == '/' || s[i] == '%' || s[i] == '=' ||
                s[i] == '<' || s[i] == '>' || s[i] == '!' ||
                s[i] == '&' || s[i] == '|') {

                cout << line << "\t" << s[i] << "\t\tOperator\n";
                i++;
                continue;
            }

            // Special symbols
            if (s[i] == '(' || s[i] == ')' ||
                s[i] == '{' || s[i] == '}' ||
                s[i] == '[' || s[i] == ']' ||
                s[i] == ';' || s[i] == ',' ||
                s[i] == '.' || s[i] == '#') {

                cout << line << "\t" << s[i]
                     << "\t\tSpecial Symbol\n";
                i++;
                continue;
            }

            cout << line << "\t" << s[i] << "\t\tUnknown Token\n";
            i++;
        }
    }

    file.close();
    return 0;
}

