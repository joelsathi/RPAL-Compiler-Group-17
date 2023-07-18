#ifndef GRAMMER_H
#define GRAMMER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include "AST.h"

using namespace std;

class Grammer
{
public:
    Grammer(vector<string> tokens)
    {
        this->tokens = tokens;
        this->curr_token = this->tokens[0];
        this->AST = Graph();
    }

    Node *get_ast_root()
    {
        return this->AST.get_root();
    }

    void parse()
    {
        E();
        this->AST.print_node_stack();
    }

private:
    vector<string> tokens;
    string curr_token;
    Graph AST;

    vector<string> keywords = {"let", "in", "fn", "where", "aug",
                               "or", "not", "gr", "ge", "ls", "le", "eq", "ne",
                               "true", "false", "nil", "dummy", "within", "and",
                               "rec", "Isinteger", "Isstring", "Isfunction", "Istruthvalue",
                               "Isdummy", "Istuple"};

    bool checkType(string token, string str_type)
    {
        if (str_type.compare("identifier"))
        {
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

    void consume(string token)
    {
        if (checkType(token, "identifier"))
        {
            this->AST.push_to_stack(token, "identifier");
        }
        else if (checkType(token, "string"))
        {
            this->AST.push_to_stack(token, "string");
        }
        else if (checkType(token, "integer"))
        {
            this->AST.push_to_stack(token, "integer");
        }

        this->tokens.erase(this->tokens.begin());

        if (this->tokens.size() > 0)
        {
            this->curr_token = tokens[0];
        }
        else
        {
            this->curr_token = "";
        }
    }

    // Ama 
    void E()
    {
        
    }

    void Ew()
    {
        T();
        if (this->curr_token.compare("where") == 0)
        {
            consume("where");
            Dr();

            this->AST.build_tree("where", 2);
        }
    }
    void T()
    {
        int cnt = 0;
        // std::cout << "Calling Ta from T with cur Token -> " << this->token << endl;
        Ta();
        cnt++;
        while ((this->curr_token.compare(",")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in T()" << endl;
            consume(",");
            // std::cout << "Calling Ta from T with cur Token -> " << this->token << endl;
            Ta();
            cnt++;
        }

        if (cnt > 1)
        {
            // std::cout << "Building node tau" << endl;
            this->AST.build_tree("tau", cnt);
        }
    }

    // Tuple aug Expression
    void Ta()
    {
        int cnt = 0;
        // std::cout << "Calling Tc from Ta with cur Token -> " << this->token << endl;
        Tc();
        cnt++;
        while ((this->curr_token.compare("aug")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Ta()" << endl;
            consume("aug");
            // std::cout << "Calling Tc from Ta with cur Token -> " << this->token << endl;
            Tc();
            cnt++;
        }

        if (cnt > 1)
        {
            // std::cout << "Building node aug" << endl;
            this->AST.build_tree("aug", cnt);
        }
    }

    // Tuple conditional Expression
    void Tc()
    {
        // std::cout << "Calling B from Tc with cur Token -> " << this->token << endl;
        B();
        if ((this->curr_token.compare("->")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Tc()" << endl;
            consume("->");
            // std::cout << "Calling Tc from Tc with cur Token -> " << this->token << endl;
            Tc();
            // std::cout << "Consuming " << this->token << " in Tc()" << endl;
            if ((this->curr_token.compare("|")) == 0)
                consume("|");
            else
                throw std::runtime_error("Can't parse the given input! \'|\' is missing");
            // std::cout << "Calling Tc from Tc with cur Token -> " << this->token << endl;
            Tc();

            // std::cout << "Building node ->" << endl;
            this->AST.build_tree("->", 3);
        }
    }

    // Boolean OR Expression
    void B()
    {
        int cnt = 0;
        // std::cout << "Calling Bt from B with cur Token -> " << this->token << endl;
        Bt();
        cnt++;
        while ((this->curr_token.compare("or")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in B()" << endl;
            consume("or");
            // std::cout << "Calling Bt from B with cur Token -> " << this->token << endl;
            Bt();
            cnt++;
        }

        if (cnt > 1)
        {
            // std::cout << "Building node or" << endl;
            this->AST.build_tree("or", cnt);
        }
    }

    // Boolean AND Expression
    void Bt()
    {
        int cnt = 0;
        // std::cout << "Calling Bs from Bt with cur Token -> " << this->token << endl;
        Bs();
        cnt++;
        while ((this->curr_token.compare("&")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bt()" << endl;
            consume("&");
            // std::cout << "Calling Bs from Bt with cur Token -> " << this->token << endl;
            Bs();
            cnt++;
        }

        if (cnt > 1)
        {
            // std::cout << "Building node &" << endl;
            this->AST.build_tree("&", cnt);
        }
    }

    // Boolean Simple Expression
    void Bs()
    {
        if ((this->curr_token.compare("not")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Bs()" << endl;
            consume("not");
            // std::cout << "Calling Bp from Bs with cur Token -> " << this->token << endl;
            Bp();

            // std::cout << "Building node not" << endl;
            this->AST.build_tree("not", 1);
        }
        else
        {
            // std::cout << "Calling Bp from Bs with cur Token -> " << this->token << endl;
            Bp();
        }
    }

    // Boolean Primitive Expression
    // Ravindu
    void Bp()
    {
    }

    // Ama
    void A()
    {
    }

    void At()
    {
        // std::cout << "Calling Af from At with cur Token -> " << this->curr_token << endl;
        Af();
        if ((this->curr_token.compare("*")) == 0 | (this->curr_token.compare("/")) == 0)
        {
            string temp = this->curr_token;
            // std::cout << "Consuming " << this->token << " in At()" << endl;
            consume(this->curr_token);
            // std::cout << "Calling Af from At with cur Token -> " << this->curr_token << endl;
            Af();

            // std::cout << "Building node " << temp << endl;
            this->AST.build_tree(temp, 2);
        }
    }

    // Ravindu
    void Af()
    {
    }

    void Ap()
    {
        // std::cout << "Calling R from Ap with cur Token -> " << this->curr_token << endl;
        R();
        while ((this->curr_token.compare("@")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Ap()" << endl;
            consume("@");
            if (checkType(this->curr_token, "identifier"))
            {
                // std::cout << "Consuming " << this->curr_token << " in Ap()" << endl;
                consume(this->curr_token);
            }
            else
            {
                // std::cout << "Error: Expected identifier but found -> " << this->curr_token << endl;
                throw std::runtime_error("Can't parse the given input! Error: Expected identifier");
            }
            // std::cout << "Calling R from Ap with cur Token -> " << this->curr_token << endl;
            R();

            // std::cout << "Building node @" << endl;
            this->AST.build_tree("@", 3);
        }
    }

    void R()
    {
        // std::cout << "Calling Rn from R with cur Token -> " << this->curr_token << endl;
        Rn();

        while (Rn())
        {
            // std::cout << "Calling Rn from R with cur Token -> " << this->curr_token << endl;

            // std::cout << "Building node gamma" << endl;
            this->AST.build_tree("gamma", 2);
        }
    }

    bool Rn()
    {
        if (checkType(this->curr_token, "identifier"))
        {
            // std::cout << "This is an identifier" << endl;
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume(this->curr_token);
            return true;
        }
        else if (checkType(this->curr_token, "integer"))
        {
            // std::cout << "This is an integer" << endl;
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume(this->curr_token);
            return true;
        }
        else if (checkType(this->curr_token, "string"))
        {
            // std::cout << "This is an string" << endl;
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume(this->curr_token);
            return true;
        }
        else if ((this->curr_token.compare("true")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume(this->curr_token);

            // std::cout << "Building node true" << endl;
            this->AST.build_tree("true", 0);
            return true;
        }
        else if ((this->curr_token.compare("false")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume(this->curr_token);

            // std::cout << "Building node false" << endl;
            this->AST.build_tree("false", 0);
            return true;
        }
        else if ((this->curr_token.compare("nil")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume(this->curr_token);

            // std::cout << "Building node nil" << endl;
            this->AST.build_tree("nil", 0);
            return true;
        }
        else if ((this->curr_token.compare("(")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            consume("(");
            // std::cout << "Calling E from Rn with cur Token -> " << this->curr_token << endl;
            E();
            // std::cout << "Consuming " << this->curr_token << " in Rn()" << endl;
            if ((this->curr_token.compare(")")) == 0)
                consume(")");

            else
                throw std::runtime_error("Can't parse the given input! \')\' is missing");
            return true;
        }
        else if ((this->curr_token.compare("dummy")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Rn()" << endl;
            consume(this->curr_token);

            // std::cout << "Building node dummy" << endl;
            this->AST.build_tree("dummy", 0);

            return true;
        }

        return false;
    }

    void D()
    {
        // std::cout << "Calling Da from D with cur Token -> " << this->curr_token << endl;
        Da();
        if ((this->curr_token.compare("within")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in D()" << endl;
            consume("within");
            // std::cout << "Calling D from D with cur Token -> " << this->curr_token << endl;
            D();

            // std::cout << "Building node within" << endl;
            this->AST.build_tree("within", 2);
        }
    }

    void Da()
    {
        int cnt = 0;
        // std::cout << "Calling Dr from Da with cur Token -> " << this->curr_token << endl;
        Dr();
        cnt++;
        while ((this->curr_token.compare("and")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Da()" << endl;
            consume("and");
            // std::cout << "Calling Dr from Da with cur Token -> " << this->token << endl;
            Dr();
            cnt++;
        }

        if (cnt > 1)
        {
            // std::cout << "Building node and" << endl;
            this->AST.build_tree("and", cnt);
        }
    }

    void Dr()
    {
        if ((this->curr_token.compare("rec")) == 0)
        {
            // std::cout << "Consuming " << this->token << " in Dr()" << endl;
            consume("rec");
            // std::cout << "Calling Db from Dr with cur Token -> " << this->token << endl;
            Db();

            // std::cout << "Building node rec" << endl;
            this->AST.build_tree("rec", 1);
        }
        else
        {
            // std::cout << "Calling Db from Dr with cur Token -> " << this->token << endl;
            Db();
        }
    }

    void Db()
    {
        if ((this->curr_token.compare("(")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Db()" << endl;
            consume("(");
            // std::cout << "Calling D from Db with cur Token -> " << this->curr_token << endl;
            D();
            // std::cout << "Consuming " << this->curr_token << " in Db()" << endl;
            if ((this->curr_token.compare(")")) == 0)
                consume(")");
            else
                throw std::runtime_error("Can't parse the given input! \')\' is missing");
        }
        else if (checkType(this->curr_token, "identifier"))
        {
            consume(this->curr_token);
            if ((this->curr_token.compare(",")) == 0 | (this->curr_token.compare("=") == 0))
            {
                // std::cout << "Calling Vl from Db with cur Token -> " << this->curr_token << endl;
                Vl();
                // std::cout << "Consuming " << this->curr_token << " in Db()" << endl;
                if ((this->curr_token.compare("=")) == 0)
                    consume("=");
                else
                    throw std::runtime_error("Can't parse the given input! \'=\' is missing");
                // std::cout << "Calling E from Db with cur Token -> " << this->curr_token << endl;
                E();

                // std::cout << "Building node =" << endl;
                this->AST.build_tree("=", 2);
            }
            else
            {
                // Check again
                int cnt = 1;

                while ((this->curr_token.compare("=")) != 0)
                {
                    // std::cout << "Calling Vb from Db with cur Token -> " << this->curr_token << endl;
                    Vb();
                    cnt++;
                }
                // std::cout << "Consuming " << this->curr_token << " in Db()" << endl;
                if ((this->curr_token.compare("=")) == 0)
                    consume("=");
                else
                    throw std::runtime_error("Can't parse the given input! \'=\' is missing");
                // std::cout << "Calling E from Db with cur Token -> " << this->curr_token << endl;
                E();
                cnt++;

                // std::cout << "Building node fcn_form" << endl;
                this->AST.build_tree("fcn_form", cnt);
            }
        }
    };

    void Vb()
    {
        if (checkType(this->curr_token, "identifier"))
        {
            // std::cout << "This is an identifier" << endl;
            // std::cout << "Consuming " << this->curr_token << " in Vb()" << endl;
            consume(this->curr_token);
        }
        else if ((this->curr_token.compare("(")) == 0)
        {
            // std::cout << "Consuming " << this->curr_token << " in Vb()" << endl;
            consume("(");
            if ((this->curr_token.compare(")")) == 0)
            {
                // std::cout << "Consuming " << this->curr_token << " in Vb()" << endl;
                consume(")");

                // std::cout << "Building node ()" << endl;
                this->AST.build_tree("()", 0);
                return;
            }

            // CHECK AGAIN
            if (checkType(this->curr_token, "identifier"))
            {
                // std::cout << "This is an identifier" << endl;
                // std::cout << "Consuming " << this->curr_token << " in Vl()" << endl;
                consume(this->curr_token);
            }
            else
            {
                // std::cout << "Error: Expected identifier but found " << this->curr_token << endl;
                return;
            }
            // std::cout << "Calling Vl from Vb with cur Token -> " << this->curr_token << endl;
            Vl();
            if ((this->curr_token.compare(")")) == 0)
                consume(")");
            else
                throw std::runtime_error("Can't parse the given input! \')\' is missing");
            // std::cout << "Consuming " << this->curr_token << " in Vb()" << endl;
        }
        else
        {
            // std::cout << "Error: Expected identifier or ( but found -> " << this->curr_token << endl;
            return;
        }
    }

    void Vl()
    {
        int cnt = 1;
        if ((this->curr_token.compare(",")) == 0)
        {
            while ((this->curr_token.compare(",")) == 0)
            {
                // std::cout << "Consuming " << this->curr_token << " in Vl()" << endl;
                consume(",");
                if (checkType(this->curr_token, "identifier"))
                {
                    // std::cout << "This is an identifier" << endl;
                    // std::cout << "Consuming " << this->curr_token << " in Vl()" << endl;
                    consume(this->curr_token);
                    cnt++;
                }
                else
                {
                    // std::cout << "Error: Expected identifier but found -> " << this->curr_token << endl;
                    return;
                }
            }
        }

        if (cnt > 1)
        {
            // std::cout << "Building node ," << endl;
            this->AST.build_tree(",", cnt);
        }
    }
};
#endif
