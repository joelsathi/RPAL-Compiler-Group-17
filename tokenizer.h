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
            return;
        }
        
    private:
        string input;
};

#endif