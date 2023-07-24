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
        void lexer(){
            // Go through the input string and tokenize it
        }
};

#endif