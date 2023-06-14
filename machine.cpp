#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include "AST.h"
#include "grammer.h"
#include "ST.h"
#include "CSE.h"

using namespace std;

vector<string> test_cases(int case_num)
{
    vector<vector<string>> testcases = {
        {"let", "isNeg", "(", "x", ")", "=", "x", "ls", "0", "->", "'Negative'", "|", "'Positive'", "in", "Print", "(", "isNeg", "(", "4", ")", ")"},
        {"let", "x", "=", "+", "2", "in", "Print", "(", "+", "(", "x", "-", "1", ")", "/", "3", ")"},
        {"let", "f", "x", "y", "=", "x", "+", "y", "in", "Print", "(", "3", "@", "f", "2", ")"},
        {"let", "c", "=", "3", "within", "f", "x", "=", "x", "+", "c", "in", "f", "2"},
        {"let", "x", "=", "3", "in", "let", "y", "=", "4", "in", "x", "+", "y"},
        {"let", "f", "x", "y", "z", "=", "x", "+", "y", "+", "z", "in", "f", "1", "2", "3"},
    };
    return testcases[case_num - 1];
}

int main()
{
    std::cout << "Hello world!" << endl;
    return 0;
}