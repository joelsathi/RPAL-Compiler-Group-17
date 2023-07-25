#ifndef GRAMMER_H
#define GRAMMER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include "AST.h"

using namespace std;

class Grammar
{
public:
    Grammar(vector<string> grammar)
    {
        this->grammar = grammar;
        this->token = grammar[0];
        this->AST = Graph();
        // std::cout << "Starting to parse" << endl;
        // std::cout << "This is the token -> " << this->token << endl;
    }

    Node* get_ast_root()
    {
        return this->AST.get_root();
    }

    void parse()
    {
        E();
        // std::cout << "Successfully parsed the string" << endl;
        // this->AST.print_node_stack();
        // this->AST.print_graph();
    }

private:
    vector<string> grammar;
    string token;
    Graph AST;

    // Have to check for the keywords that are used for error handling
    // For example: Isinteger, Isstring
    vector<string> keywords = {"let", "in", "fn", "where", "aug",
                               "or", "not", "gr", "ge", "ls", "le", "eq", "ne",
                               "true", "false", "nil", "dummy", "within", "and", 
                               "rec"};

    bool CheckType(string token, string str_type)
    {
        if (str_type.compare("identifier") == 0)
        {
            // We don't want to match a keyword as an identifier
            if (std::find(keywords.begin(), keywords.end(), token) != keywords.end())
            {
                return false;
            }
            regex identifier_pat("[a-zA-Z][a-zA-Z0-9_]*");
            return regex_match(token, identifier_pat);
        }
        else if (str_type.compare("integer") == 0)
        {
            regex integer_pat("[0-9]+");
            return regex_match(token, integer_pat);
        }
        else if (str_type.compare("string") == 0)
        {
            regex string_pat("\'[^\"]*\'");
            return regex_match(token, string_pat);
        }
        return false;
    }

    /**
     * @brief This function will take the token which should be parsed in the grammar & consume it.
     *
     * @param token
     */
    void CONSUME(string token)
    {
        // std::cout << "Consumed -> " << token << endl;

        // if (CheckType(token, "identifier") | CheckType(token, "string") | CheckType(token, "integer"))
        // {
            // std::cout << "Pushing to stack -> " << token << endl;
        //     this->AST.push_to_stack(token);
        // }

        if (CheckType(token, "identifier")){
            // std::cout << "Pushing to stack -> " << token << endl;
            this->AST.push_to_stack(token, "identifier");
        }
        else if (CheckType(token, "string")){
            // std::cout << "Pushing to stack -> " << token << endl;
            this->AST.push_to_stack(token, "string");
        }
        else if (CheckType(token, "integer")){
            // std::cout << "Pushing to stack -> " << token << endl;
            this->AST.push_to_stack(token, "integer");
        }

        this->grammar.erase(this->grammar.begin());
        // Update the token
        if (this->grammar.size() > 0)
        {
            this->token = this->grammar[0];
        }
        else
        {
            this->token = "";
        }

        // std::cout << "This is the new token -> " << this->token << endl;
    }

    // Expression Grammar
    void E()
    {
        // Let expression
        if ((this->token.compare("let")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in E()" << endl;
            CONSUME("let");
            // std::cout << "Calling D from E with cur Token -> " << this->token << endl;
            D();
            // std::cout << "Consuming " << this->token << " in E()" << endl;
            if ((this->token.compare("in")) == 0)
                CONSUME("in");
            else
                throw std::runtime_error("Can't parse the given input. \'in\' is missing");
            // std::cout << "Calling E from E with cur Token -> " << this->token << endl;
            E();

            // std::cout << "Building node let" << endl; 
            this->AST.build_Tree("let", 2);
        }
        // Lambda Expression
        else if ((this->token.compare("fn")) == 0)
        {
            int cnt = 0;
            // std::cout << "Consuming " << this->token << " in E()" << endl;
            CONSUME("fn");
            // std::cout << "Calling Vb from E with cur Token -> " << this->token << endl;
            Vb();
            cnt++;
            while ((this->token.compare(".")) != 0)
            {
                // std::cout << "Calling Vb from E with cur Token -> " << this->token << endl;
                Vb();
                cnt++;
            }
            // std::cout << "Consuming " << this->token << " in E()" << endl;
            if ((this->token.compare(".")) == 0)
                CONSUME(".");
            else
                throw std::runtime_error("Can't parse the given input! \'.\' is missing");
            // std::cout << "Calling E from E with cur Token -> " << this->token << endl;
            E();
            cnt++;
            
            // std::cout << "Building node lambda" << endl;
            this->AST.build_Tree("lambda", cnt);
        }
        // Where case
        else
        {
            // std::cout << "Calling Ew from E with cur Token -> " << this->token << endl;
            Ew();
        }
    }

    // Expression where
    void Ew()
    {   
        int cnt = 0;
        // std::cout << "Calling T from Ew with cur Token -> " << this->token << endl;
        T();
        cnt++;
        if ((this->token.compare("where")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Ew()" << endl;
            CONSUME("where");
            // std::cout << "Calling D from Ew with cur Token -> " << this->token << endl;
            Dr();
            cnt++;

            // std::cout << "Building node where" << endl;
            this->AST.build_Tree("where", cnt);
        }
    }

    // Tuple Expression
    void T()
    {
        int cnt = 0;
        // std::cout << "Calling Ta from T with cur Token -> " << this->token << endl;
        Ta();
        cnt++;
        while ((this->token.compare(",")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in T()" << endl;
            CONSUME(",");
            // std::cout << "Calling Ta from T with cur Token -> " << this->token << endl;
            Ta();
            cnt++;
        }

        if (cnt > 1){
            // std::cout << "Building node tau" << endl;
            this->AST.build_Tree("tau", cnt);
        }
    }

    // Tuple aug Expression
    void Ta()
    {
        int cnt = 0;
        // std::cout << "Calling Tc from Ta with cur Token -> " << this->token << endl;
        Tc();
        cnt++;
        while ((this->token.compare("aug")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Ta()" << endl;
            CONSUME("aug");
            // std::cout << "Calling Tc from Ta with cur Token -> " << this->token << endl;
            Tc();
            cnt++;
        }

        if (cnt > 1){
            // std::cout << "Building node aug" << endl;
            this->AST.build_Tree("aug", cnt);
        }
    }

    // Tuple conditional Expression
    void Tc()
    {
        // std::cout << "Calling B from Tc with cur Token -> " << this->token << endl;
        B();
        if ((this->token.compare("->")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Tc()" << endl;
            CONSUME("->");
            // std::cout << "Calling Tc from Tc with cur Token -> " << this->token << endl;
            Tc();
            // std::cout << "Consuming " << this->token << " in Tc()" << endl;
            if ((this->token.compare("|")) == 0)
                CONSUME("|");
            else
                throw std::runtime_error("Can't parse the given input! \'|\' is missing");
            // std::cout << "Calling Tc from Tc with cur Token -> " << this->token << endl;
            Tc();

            // std::cout << "Building node ->" << endl;
            this->AST.build_Tree("->", 3);
        }
    }

    // Boolean OR Expression
    void B()
    {   
        int cnt = 0;
        // std::cout << "Calling Bt from B with cur Token -> " << this->token << endl;
        Bt();
        cnt++;
        while ((this->token.compare("or")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in B()" << endl;
            CONSUME("or");
            // std::cout << "Calling Bt from B with cur Token -> " << this->token << endl;
            Bt();
            cnt++;
        }

        if (cnt > 1){
            // std::cout << "Building node or" << endl;
            this->AST.build_Tree("or", cnt);
        }
    }

    // Boolean AND Expression
    void Bt()
    {
        int cnt = 0;
        // std::cout << "Calling Bs from Bt with cur Token -> " << this->token << endl;
        Bs();
        cnt++;
        while ((this->token.compare("&")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bt()" << endl;
            CONSUME("&");
            // std::cout << "Calling Bs from Bt with cur Token -> " << this->token << endl;
            Bs();
            cnt++;
        }

        if (cnt > 1){
            // std::cout << "Building node &" << endl;
            this->AST.build_Tree("&", cnt);
        }
    }

    // Boolean Simple Expression
    void Bs()
    {
        if ((this->token.compare("not")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bs()" << endl;
            CONSUME("not");
            // std::cout << "Calling Bp from Bs with cur Token -> " << this->token << endl;
            Bp();

            // std::cout << "Building node not" << endl;
            this->AST.build_Tree("not", 1);
        }
        else
        {
            // std::cout << "Calling Bp from Bs with cur Token -> " << this->token << endl;
            Bp();
        }
    }

    // Boolean Primitive Expression
    void Bp()
    {
        // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
        A();
        if ((this->token.compare("gr")) == 0 | (this->token.compare(">")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bp()" << endl;
            CONSUME(this->token);
            // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
            A();

            // std::cout << "Building node gr" << endl;
            this->AST.build_Tree("gr", 2);
        }
        else if ((this->token.compare("ge")) == 0 | (this->token.compare(">=")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bp()" << endl;
            CONSUME(this->token);
            // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
            A();

            // std::cout << "Building node ge" << endl;
            this->AST.build_Tree("ge", 2);
        }
        else if ((this->token.compare("ls")) == 0 | (this->token.compare("<")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bp()" << endl;
            CONSUME(this->token);
            // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
            A();

            // std::cout << "Building node ls" << endl;
            this->AST.build_Tree("ls", 2);
        }
        else if ((this->token.compare("le")) == 0 | (this->token.compare("<=")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bp()" << endl;
            CONSUME(this->token);
            // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
            A();

            // std::cout << "Building node le" << endl;
            this->AST.build_Tree("le", 2);
        }
        else if ((this->token.compare("eq")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bp()" << endl;
            CONSUME("eq");
            // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
            A();

            // std::cout << "Building node eq" << endl;
            this->AST.build_Tree("eq", 2);
        }
        else if ((this->token.compare("ne")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bp()" << endl;
            CONSUME("ne");
            // std::cout << "Calling A from Bp with cur Token -> " << this->token << endl;
            A();

            // std::cout << "Building node ne" << endl;
            this->AST.build_Tree("ne", 2);
        }
    }

    void A()
    {

        if ((this->token.compare("+")) == 0){
            // std::cout << "Consuming " << this->token << " in A() as prefix plus" << endl;
            CONSUME(this->token);
            // std::cout << "Calling At from A with cur Token -> " << this->token << endl;
            At();
        }
        else if ((this->token.compare("-")) == 0){
            // std::cout << "Consuming " << this->token << " in A() as prefix minus" << endl;
            CONSUME(this->token);
            // std::cout << "Calling At from A with cur Token -> " << this->token << endl;
            At();

            // std::cout << "Building node neg" << endl;
            this->AST.build_Tree("neg", 1);  
        }
        else{
            // std::cout << "Calling At from A with cur Token -> " << this->token << endl;
            At();

            while ((this->token.compare("+")) == 0 | (this->token.compare("-")) == 0)
            {
                string temp = this->token;
                // std::cout << "Consuming " << this->token << " in A()" << endl;
                CONSUME(this->token);
                // std::cout << "Calling At from A with cur Token -> " << this->token << endl;
                At();

                // std::cout << "Building node " << temp << endl;
                this->AST.build_Tree(temp, 2);
            }
        }
        
        
    }

    void At()
    {
        // std::cout << "Calling Af from At with cur Token -> " << this->token << endl;
        Af();
        if ((this->token.compare("*")) == 0 | (this->token.compare("/")) == 0)
        {
            string temp = this->token;
            // std::cout << "Consuming " << this->token << " in At()" << endl;
            CONSUME(this->token);
            // std::cout << "Calling Af from At with cur Token -> " << this->token << endl;
            Af();

            // std::cout << "Building node " << temp << endl;
            this->AST.build_Tree(temp, 2);
        }
    }

    void Af()
    {
        // std::cout << "Calling Ap from Af with cur Token -> " << this->token << endl;
        Ap();
        if ((this->token.compare("**")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Af()" << endl;
            CONSUME("**");
            // std::cout << "Calling Af from Af with cur Token -> " << this->token << endl;
            Af();

            // std::cout << "Building node **" << endl;
            this->AST.build_Tree("**", 2);
        }
    }

    void Ap()
    {
        // std::cout << "Calling R from Ap with cur Token -> " << this->token << endl;
        R();
        while ((this->token.compare("@")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Ap()" << endl;
            CONSUME("@");
            if (CheckType(this->token, "identifier"))
            {
                // std::cout << "Consuming " << this->token << " in Ap()" << endl;
                CONSUME(this->token);
            }
            else
            {
                // std::cout << "Error: Expected identifier but found -> " << this->token << endl;
                throw std::runtime_error("Can't parse the given input! Error: Expected identifier");
            }
            // std::cout << "Calling R from Ap with cur Token -> " << this->token << endl;
            R();

            // std::cout << "Building node @" << endl;
            this->AST.build_Tree("@", 3);
        }
    }

    void R()
    {
        // std::cout << "Calling Rn from R with cur Token -> " << this->token << endl;
        Rn();

        while (Rn())
        {
            // std::cout << "Calling Rn from R with cur Token -> " << this->token << endl;

            // std::cout << "Building node gamma" << endl;
            this->AST.build_Tree("gamma", 2);
        }
    }

    bool Rn()
    {
        if (CheckType(this->token, "identifier"))
        {
            // std::cout << "This is an identifier" << endl;
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);
            return true;
        }
        else if (CheckType(this->token, "integer"))
        {
            // std::cout << "This is an integer" << endl;
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);
            return true;
        }
        else if (CheckType(this->token, "string"))
        {
            // std::cout << "This is an string" << endl;
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);
            return true;
        }
        else if ((this->token.compare("true")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);

            // std::cout << "Building node true" << endl;
            this->AST.build_Tree("true", 0);
            return true;
        }
        else if ((this->token.compare("false")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);

            // std::cout << "Building node false" << endl;
            this->AST.build_Tree("false", 0);
            return true;
        }
        else if ((this->token.compare("nil")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);

            // std::cout << "Building node nil" << endl;
            this->AST.build_Tree("nil", 0);
            return true;
        }
        else if ((this->token.compare("(")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME("(");
            // std::cout << "Calling E from Rn with cur Token -> " << this->token << endl;
            E();
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            if ((this->token.compare(")")) == 0)
                CONSUME(")");
                
            else
                throw std::runtime_error("Can't parse the given input! \')\' is missing");
            return true;

        }
        else if ((this->token.compare("dummy")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            CONSUME(this->token);

            // std::cout << "Building node dummy" << endl;
            this->AST.build_Tree("dummy", 0);

            return true;
        }

        // std::cout << "\n\n\n\n Calling false with " << this->token  << "\n\n\n\n" << endl;
        return false;
    }

    void D()
    {
        // std::cout << "Calling Da from D with cur Token -> " << this->token << endl;
        Da();
        if ((this->token.compare("within")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in D()" << endl;
            CONSUME("within");
            // std::cout << "Calling D from D with cur Token -> " << this->token << endl;
            D();

            // std::cout << "Building node within" << endl;
            this->AST.build_Tree("within", 2);
        }
    }

    void Da()
    {
        int cnt = 0;
        // std::cout << "Calling Dr from Da with cur Token -> " << this->token << endl;
        Dr();
        cnt++;
        while ((this->token.compare("and")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Da()" << endl;
            CONSUME("and");
            // std::cout << "Calling Dr from Da with cur Token -> " << this->token << endl;
            Dr();
            cnt++;
        }

        if (cnt > 1){
            // std::cout << "Building node and" << endl;
            this->AST.build_Tree("and", cnt);
        }
    }

    void Dr()
    {
        if ((this->token.compare("rec")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Dr()" << endl;
            CONSUME("rec");
            // std::cout << "Calling Db from Dr with cur Token -> " << this->token << endl;
            Db();

            // std::cout << "Building node rec" << endl;
            this->AST.build_Tree("rec", 1);
        }
        else
        {
            // std::cout << "Calling Db from Dr with cur Token -> " << this->token << endl;
            Db();
        }
    }

    void Db()
    {
        if ((this->token.compare("(")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Db()" << endl;
            CONSUME("(");
            // std::cout << "Calling D from Db with cur Token -> " << this->token << endl;
            D();
            // std::cout << "Consuming " << this->token << " in Db()" << endl;
            if ((this->token.compare(")")) == 0)
                CONSUME(")");
            else
                throw std::runtime_error("Can't parse the given input! \')\' is missing");
        }

        else if (CheckType(this->token, "identifier"))
        {
            // std::cout << "Consuming " << this->token << " in Db()" << endl;
            CONSUME(this->token);
            if ((this->token.compare(",")) == 0 | (this->token.compare("=") == 0))
            {
                // std::cout << "Calling Vl from Db with cur Token -> " << this->token << endl;
                Vl();
                // std::cout << "Consuming " << this->token << " in Db()" << endl;
                if ((this->token.compare("=")) == 0)
                    CONSUME("=");
                else
                    throw std::runtime_error("Can't parse the given input! \'=\' is missing");
                // std::cout << "Calling E from Db with cur Token -> " << this->token << endl;
                E();

                // std::cout << "Building node =" << endl;
                this->AST.build_Tree("=", 2);
            }
            else
            {
                // Check again
                int cnt = 1;

                while ((this->token.compare("=")) != 0)
                {
                    // std::cout << "Calling Vb from Db with cur Token -> " << this->token << endl;
                    Vb();
                    cnt++;
                }
                // std::cout << "Consuming " << this->token << " in Db()" << endl;
                if ((this->token.compare("=")) == 0)
                    CONSUME("=");
                else
                    throw std::runtime_error("Can't parse the given input! \'=\' is missing");
                // std::cout << "Calling E from Db with cur Token -> " << this->token << endl;
                E();
                cnt++;

                // std::cout << "Building node fcn_form" << endl;
                this->AST.build_Tree("fcn_form", cnt);
            }
        }
    }

    void Vb()
    {
        if (CheckType(this->token, "identifier"))
        {
            // std::cout << "This is an identifier" << endl;
            // std::cout << "Consuming " << this->token << " in Vb()" << endl;
            CONSUME(this->token);
        }
        else if ((this->token.compare("(")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Vb()" << endl;
            CONSUME("(");
            if ((this->token.compare(")")) == 0)
            {
                // std::cout << "Consuming " << this->token << " in Vb()" << endl;
                CONSUME(")");

                // std::cout << "Building node ()" << endl;
                this->AST.build_Tree("()", 0);
                return;
            }

            // CHECK AGAIN
            if (CheckType(this->token, "identifier"))
            {
                // std::cout << "This is an identifier" << endl;
                // std::cout << "Consuming " << this->token << " in Vl()" << endl;
                CONSUME(this->token);
            }
            else
            {
                // std::cout << "Error: Expected identifier but found " << this->token << endl;
                return;
            }
            // std::cout << "Calling Vl from Vb with cur Token -> " << this->token << endl;
            Vl();
            if ((this->token.compare(")")) == 0)
                CONSUME(")");
            else
                throw std::runtime_error("Can't parse the given input! \')\' is missing");
        // std::cout << "Consuming " << this->token << " in Vb()" << endl;
        }
        else
        {
            // std::cout << "Error: Expected identifier or ( but found -> " << this->token << endl;
            return;
        }
    }

    void Vl()
    {
        int cnt = 1;
        if ((this->token.compare(",")) == 0)
        {
            while ((this->token.compare(",")) == 0)
            {
                // std::cout << "Consuming " << this->token << " in Vl()" << endl;
                CONSUME(",");
                if (CheckType(this->token, "identifier"))
                {
                    // std::cout << "This is an identifier" << endl;
                    // std::cout << "Consuming " << this->token << " in Vl()" << endl;
                    CONSUME(this->token);
                    cnt++;
                }
                else
                {
                    // std::cout << "Error: Expected identifier but found -> " << this->token << endl;
                    return;
                }
            }
        }

        
        if (cnt > 1){
            // std::cout << "Building node ," << endl;
            this->AST.build_Tree(",", cnt);
        }
    }
};

#endif