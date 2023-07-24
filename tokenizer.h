#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>

using namespace std;

class Tokenizer{
    public:
        Tokenizer(string input){
            this->input = input;
        }

        vector<string> tokenize(){
            return this->tokens;
        }
        
    private:
        string input;
        vector<string> tokens;

        vector<char> operator_symbols = {
            '+', '-', '*', '<', '>', '&', '.', 
            '@', '/', ':', '=', '~', '|', '$', 
            '!', '#', '%', '^', '[', ']', 
            '{', '}', '?', '(', ')', ','
        };

        // Ama
        void lexer()
{
        // Go through the input string and tokenize it
        string currentToken = "";
        for (char ch : input)
        {
        // Check if the current character is a whitespace
            if (isspace(ch))
            {
                // If the current token is not empty, add it to the list of tokens
                if (!currentToken.empty())
                {
                    tokens.push_back(currentToken);
                    currentToken = ""; // Reset the temporary storage
                }
                // Add the operator symbol as a separate token
                tokens.push_back(string(1, ch));
            }
            else 
            {
            // If the current character is not a whitespace or operator symbol, add it to the current token
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
