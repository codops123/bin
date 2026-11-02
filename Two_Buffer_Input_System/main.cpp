
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

const int SIZE = 10;

ifstream file;
char buffer1[SIZE], buffer2[SIZE];

int count1 = 0, count2 = 0;
int pos = 0;
int current = 1;
int line = 1;

void loadBuffer(int n) {
    if (n == 1) {
        file.read(buffer1, SIZE);
        count1 = file.gcount();
    }
    else {
        file.read(buffer2, SIZE);
        count2 = file.gcount();
    }
}

char getChar() {
    if (current == 1 && pos >= count1) {
        current = 2;
        pos = 0;
        loadBuffer(2);
    }
    else if (current == 2 && pos >= count2) {
        current = 1;
        pos = 0;
        loadBuffer(1);
    }

    if (current == 1) {
        if (pos >= count1)
            return EOF;

        char ch = buffer1[pos++];

        if (ch == '\n')
            line++;

        return ch;
    }
    else {
        if (pos >= count2)
            return EOF;

        char ch = buffer2[pos++];

        if (ch == '\n')
            line++;

        return ch;
    }
}

bool isKeyword(string word) {
    string keywords[] = {
        "int", "float", "char",
        "if", "else", "while", "return"
    };

    for (string keyword : keywords) {
        if (word == keyword)
            return true;
    }

    return false;
}

int main() {
    file.open("input.txt");

    if (!file) {
        cout << "File not found!";
        return 1;
    }

    loadBuffer(1);

    char ch;
    string lexeme;

    cout << "Line\tLexeme\t\tToken\n";
    cout << "--------------------------------\n";

    while ((ch = getChar()) != EOF) {

        if (isspace(ch))
            continue;

        int tokenLine = line;

        if (isalpha(ch) || ch == '_') {
            lexeme = ch;

            while (true) {
                ch = getChar();

                if (ch == EOF || (!isalnum(ch) && ch != '_'))
                    break;

                lexeme += ch;
            }

            if (isKeyword(lexeme))
                cout << tokenLine << "\t" << lexeme << "\t\tKeyword\n";
            else
                cout << tokenLine << "\t" << lexeme << "\t\tIdentifier\n";

            if (ch == EOF)
                break;
        }
    }

    file.close();
    return 0;
}

