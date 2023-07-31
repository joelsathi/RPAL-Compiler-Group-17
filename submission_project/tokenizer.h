#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <fstream> 

using namespace std;

class Tokenizer
{
public:
    Tokenizer()
    {
        // this->input = input;
    }

    vector<string> tokenize(int argc, char **argv)
    {
        if (argc != 2)
        {
            cout << "Please provide the input file path" << endl;
            exit(1);
        }
        string file_path = argv[1];
        read_file(file_path);
        this->lexer();
        // print_tokens();
        this->validate_tokens();
        return this->tokens;
    }

    // FIXME: change this to read the file by getting the file path from command line
    void read_file(string file_path)
    {
        // Read the input file from Test_cases folder
        ifstream input_file(file_path);
        string line;
        string input = "";
        if (input_file.is_open())
        {
            while (getline(input_file, line))
            {
                // remove the spaces or tabs that are in the beginning of the line
                string new_line = "";
                bool found = false;
                for (int i = 0; i < line.length(); i++)
                {
                    // int val = line[i];
                    if (line[i] == ' ' || line[i] == '\t')
                    {
                        if (found)
                        {
                            new_line += line[i];
                        }
                        // found = true;
                    }
                    else
                    {
                        int val = line[i];
                        // std::cout << val << '\n';
                        // break;
                        found = true;
                        new_line += line[i];
                    }
                }
                // std::cout << new_line << '\n';
                input += line;
                input += "\n";
            }
            input_file.close();
            this->input = input;
            // std::cout << input << endl;
        }
        else
        {
            std::cout << "Unable to open file";
        }
    }

private:
    string input;
    vector<string> tokens;
    vector<string> lex;

    vector<char> operator_symbols = {
        '+', '-', '*', '<', '>', '&', '.',
        '@', '/', ':', '=', '~', '|', '$',
        '!', '#', '%', '^', '[', ']',
        '{', '}', '?', '(', ')', ','};
    
    void print_tokens(){
        for (string token : tokens){
            cout << token << endl;
        }
    }

    void push_token(string token)
    {
        int val = token[0];
        if (token == "" || val == 13|| val == 0)
        {
            return;
        }
        this->tokens.push_back(token);
    }

    // Ravindu
    void validate_tokens() {
        regex identifier_pattern("[a-zA-Z][a-zA-Z0-9_]*");

        regex integer_pattern("[0-9]+");

        regex op_pattern(R"([+\-*<>&.@/:=~|$!#%^_\[\]{}"`?]+)");

        regex string_pattern("\'[^\"]*\'");

        regex space_pattern(R"(\s+)");




        int i = 0;
        while (i < this->tokens.size()) {
            string token = this->tokens[i];
            i++;

            if (regex_match(token, identifier_pattern)) {
                this->lex.emplace_back("identifier");
            } else if (regex_match(token, integer_pattern)) {
                this->lex.emplace_back("integer");
            } else if (regex_match(token, op_pattern)) {
                this->lex.emplace_back("operator");
            } else if (regex_match(token, string_pattern)) {
                this->lex.emplace_back("string");
            } else if (token == "(") {
                this->lex.emplace_back("(");
            } else if (token == ")") {
                this->lex.emplace_back(")");
            } else if (token == ";") {
                this->lex.emplace_back(";");
            } else if (token == ",") {
                this->lex.emplace_back(",");
            } else if (regex_match(token, space_pattern)) {
                continue;
            }
            else
            {
                std::cout << "Invalid Token" << endl;
                std::exit(0);
            }
        }
    }

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
        for (int i=0; i<input.length(); i++)
        {
            char ch = input[i];
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
                // tokens.push_back(currentToken);
                continue;
            }

            // Check if the current character is a whitespace
            if (isspace(ch))
            {
                // If the current token is not empty, add it to the list of tokens
                if (!currentToken.empty())
                {
                    this->push_token(currentToken);
                    currentToken = ""; // Reset the temporary storage
                }
            }
            // Search through the operator_symbols and if it matches
            else if (find(this->operator_symbols.begin(), this->operator_symbols.end(), ch) != this->operator_symbols.end())
            {
                // If we have any other token in the buffer
                // Push it to the tokens vector
                if (isString)
                {
                    currentToken += ch;
                    continue;
                }
                if (currentToken != "")
                {
                    this->push_token(currentToken);
                    currentToken = "";
                }
                currentToken += ch;

                if (currentToken == "-" && ((i + 1) < this->input.length()) && (this->input[i + 1] == '>'))
                {
                    currentToken += this->input[i + 1];
                    i++;
                }
                else if (currentToken == "/" && ((i + 1) < this->input.length()) && (this->input[i + 1] == '/'))
                {
                    isComment = true;
                    currentToken = "";
                    continue;
                }
                // Now the currentToken is an operator symbol
                // Push it to the tokens vector
                this->push_token(currentToken);
                currentToken = "";
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
            this->push_token(currentToken);
        }
    }
};

#endif
