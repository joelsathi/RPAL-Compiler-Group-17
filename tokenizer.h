#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>

using namespace std;

class Tokenizer
{
public:
    Tokenizer(string input)
    {
        this->input = input;
    }

    vector<string> tokenize()
    {
        return this->tokens;
    }

private:
    string input;
    vector<string> tokens;

    vector<char> operator_symbols = {
        '+', '-', '*', '<', '>', '&', '.',
        '@', '/', ':', '=', '~', '|', '$',
        '!', '#', '%', '^', '[', ']',
        '{', '}', '?', '(', ')', ','};

    // Ama
    void lexer()

    {
        // Temporary storage for building a token
        string currentToken = "";

        // State variables to keep track of special cases
        bool isComment = false;
        bool isString = false;
        char stringDelimiter; // To store the string delimiter (single quote character)
        // Loop through each character in the input string
        for (char ch : input)
        {
            // Check if we are inside a comment
            if (isComment)
            {
                if (ch == '\n')
                {
                    // If a newline is encountered, the comment ends
                    isComment = false;
                }
                continue;
            }

            // Check if we are inside a string
            if (isString)
            {
                if (ch == stringDelimiter)
                {
                    // If the string delimiter is encountered, the string ends
                    isString = false;
                }
                currentToken += ch; // Add the character to the current token
                continue;
            }

            // Check if the current character is a whitespace
            if (isspace(ch))
            {
                // If the current token is not empty, add it to the list of tokens
                if (!currentToken.empty())
                {
                    tokens.push_back(currentToken);
                    currentToken = ""; // Reset the temporary storage
                }
            }
            // Check if the current character starts a comment
            else if (ch == '\\' && currentToken.empty())
            {
                isComment = true; // Set the state to inside comment
            }
            // Check if the current character starts a string
            else if (ch == '\'')
            {
                isString = true;      // Set the state to inside string
                stringDelimiter = ch; // Store the string delimiter
                currentToken += ch;   // Add the character to the current token
            }
            else
            {
                // If the current character is not part of a comment or string, add it to the current token
                currentToken += ch;
            }
        }

        // Add the last token (if any) after the loop finishes
        if (!currentToken.empty())
        {
            tokens.push_back(currentToken);
        }
    }
};

#endif
