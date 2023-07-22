#ifndef ST_H
#define ST_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "AST.h"

using namespace std;

class ST
{
public:
    ST(Node *root)
    {
        this->st_root = root;
    }

    /**
     * @brief This will construct the standard tree.
     *
     */
    void standardize()
    {
        construct_ST_rec(this->st_root);
    }

    /**
     * @brief This will print the standard tree.
     *
     */
    void print_ST()
    {
        std::cout << "==================================" << endl;
        std::cout << "Printing the Standard Tree ( BFS )" << endl;
        std::cout << "==================================" << endl;

        std::cout << "\n\n"
                  << endl;

        vector<Node *> queue;
        queue.push_back(this->st_root);

        while (queue.size() > 0)
        {
            Node *cur_node = queue[0];
            queue.erase(queue.begin());
            std::cout << "Parent Node -> " << cur_node->data << endl;
            std::cout << "Children -> " << endl;
            for (int i = 0; i < cur_node->children.size(); i++)
            {
                queue.push_back(cur_node->children[i]);
                std::cout << "Child Node -> " << cur_node->children[i]->data << endl;
            }
            std::cout << "\n\n"
                      << endl;
        }
    }

private:
    Node *st_root;

    /**
     * @brief This will construct the standard tree recursively.
     *
     * @param node
     */
    void construct_ST_rec(Node *node)
    {
        for (int i = 0; i < node->children.size(); i++)
        {
            construct_ST_rec(node->children[i]);
        }

        stardardize_node(node);
    }

    /**
     * @brief This will check the data of the node and standardize by
     * calling the appropriate function.
     *
     * @param node
     */
    void stardardize_node(Node *node)
    {
        if (node->data.compare("let") == 0)
        {
            standardize_let(node);
        }
        else if (node->data.compare("fcn_form") == 0)
        {
            standadize_fcn_form(node);
        }

        else if (node->data.compare("where") == 0)
        {
            standardize_where(node);
        }
        else if (node->data.compare("lambda") == 0 && node->children[0]->data.compare(",") != 0)
        {
            standardize_lambda(node);
        }
        else if (node->data.compare("within") == 0)
        {
            standardize_within(node);
        }
        else if (node->data.compare("@") == 0)
        {
            standardize_at(node);
        }
        else if (node->data.compare("and") == 0)
        {
            standardize_and(node);
        }
        else if (node->data.compare("rec") == 0)
        {
            standardize_rec(node);
        }
        else
        {
            stardardize_node(node);
        }
    }

    bool checkType(string token, string str_type)
    {

        vector<string> keywords = {"let", "in", "fn", "where", "aug",
                                   "or", "not", "gr", "ge", "ls", "le", "eq", "ne",
                                   "true", "false", "nil", "dummy", "within", "and",
                                   "rec", "Isinteger"};

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

    // Ama
    void standardize_let(Node *node)
    {
    }

    void standardize_where(Node *node)
    {
        Node *child_0 = node->children[0];
        Node *child_1 = node->children[1];

        Node *eq;
        Node *p;
        Node *e;

        if (child_0->data.compare("=") == 0)
        {
            eq = child_0;
            p = child_1;
        }
        else if (child_1->data.compare("=") == 0)
        {
            eq = child_1;
            p = child_0;
        }
        else
        {
            throw std::invalid_argument("Invalid where expression");
        }

        Node *x;
        Node *e;

        if (checkType(eq->children[0]->data, "identifier"))
        {
            x = eq->children[0];
            e = eq->children[1];
        }
        else if (checkType(eq->children[1]->data, "identifier"))
        {
            x = eq->children[1];
            e = eq->children[0];
        }
        else
        {
            throw std::invalid_argument("Invalid syntax: Expected an identifier");
        }

        node->data = "gamma";
        node->children.clear();

        eq->data = "lambda";
        eq->children.clear();

        eq->children.push_back(x);
        eq->children.push_back(p);

        node->children.push_back(eq);
        node->children.push_back(e);
    }

    // Ravindu
    void standadize_fcn_form(Node *node)
    {
        /* last two child nodes are detached, and they are attached to a new lambda node.
         * The new lambda node is attached again to the node(fcn_form).
         * Do above 2 steps until we end up with two child nodes (left child = p, right child = lambda)
         * change fcn_form to =.
         * */
        if (node->children.size()>=3)
        {
            while (node->children.size()>2)
            {
                if(checkType(node->children[node->children.size()-2]->data, "identifier"))
                {
                    Node *lambda = new Node();
                    lambda->data="lambda";

                    Node *child_right = node->children.back();
                    node->children.pop_back();

                    Node *child_left = node->children.back();
                    node->children.pop_back();

                    lambda->children.push_back(child_left);
                    lambda->children.push_back(child_right);

                    node->children.push_back(lambda);
                }
                else
                {
                    throw std::invalid_argument("Invalid syntax: Expected an identifier");
                }
            }

            if(checkType(node->children[0]->data, "identifier"))
            {
                node->data = "=";
            }
            else
            {
                throw std::invalid_argument("Invalid syntax: Expected an identifier");
            }

        }
        else
        {
            throw std::invalid_argument("Invalid function form");
        }
    }

    // Ama
    void standardize_lambda(Node *node)
    {
    }

    void standardize_within(Node *node)
    {
        Node *eq1 = node->children[0];
        Node *eq2 = node->children[1];

        if (eq1->data.compare("=") != 0 || eq2->data.compare("=") != 0)
        {
            throw std::invalid_argument("Invalid within expression");
        }

        Node *x1;
        Node *e1;

        if (checkType(eq1->children[0]->data, "identifier"))
        {
            x1 = eq1->children[0];
            e1 = eq1->children[1];
        }
        else if (checkType(eq1->children[1]->data, "identifier"))
        {
            x1 = eq1->children[1];
            e1 = eq1->children[0];
        }
        else
        {
            throw std::invalid_argument("Invalid syntax: Expected an identifier");
        }

        Node *x2;
        Node *e2;

        if (checkType(eq2->children[0]->data, "identifier"))
        {
            x2 = eq2->children[0];
            e2 = eq2->children[1];
        }
        else if (checkType(eq2->children[1]->data, "identifier"))
        {
            x2 = eq2->children[1];
            e2 = eq2->children[0];
        }
        else
        {
            throw std::invalid_argument("Invalid syntax: Expected an identifier");
        }

        node->data = "=";
        node->children.clear();

        eq1->data = "lambda";
        eq1->children.clear();
        eq2->data = "gamma";
        eq2->children.clear();

        eq1->children.push_back(x1);
        eq1->children.push_back(e2);

        eq2->children.push_back(eq1);
        eq2->children.push_back(e1);

        node->children.push_back(x2);
        node->children.push_back(eq2);
    }

    void standardize_at(Node *node)
    {
        Node *e1 = node->children[0];
        Node *n = node->children[1];
        Node *e2 = node->children[2];

        node->data = "gamma";
        node->children.clear();

        Node *gamma2 = new Node();
        gamma2->data = "gamma";

        node->children.push_back(gamma2);
        node->children.push_back(e2);

        gamma2->children.push_back(n);
        gamma2->children.push_back(e1);
    }

    void standardize_rec(Node *node)
    {
        Node *rec_node = node->children[0];
        Node *eq_node = node->children[1];

        if (eq_node->data.compare("=") != 0)
        {
            throw std::invalid_argument("Invalid rec expression");
        }

        Node *x;
        Node *e;

        if (checkType(eq_node->children[0]->data, "identifier"))
        {
            x = eq_node->children[0];
            e = eq_node->children[1];
        }
        else if (checkType(eq_node->children[1]->data, "identifier"))
        {
            x = eq_node->children[1];
            e = eq_node->children[0];
        }
        else
        {
            throw std::invalid_argument("Invalid syntax: Expected an identifier");
        }

        rec_node->data = "=";
        rec_node->children.clear();

        eq_node->data = "gamma";
        eq_node->children.clear();

        Node *y_node = new Node();
        y_node->data = "Y";

        Node *lambda_node = new Node();
        lambda_node->data = "lambda";

        eq_node->children.push_back(y_node);
        eq_node->children.push_back(lambda_node);

        lambda_node->children.push_back(x);
        lambda_node->children.push_back(e);

        rec_node->children.push_back(x);
        rec_node->children.push_back(eq_node);
    }

    // Ravindu
    void standardize_and(Node *node)
    {
        Node *comma = new Node();
        Node *tau = new Node();

        comma->data = ",";
        tau->data = "tau";

        while (!node->children.empty())
        {
            Node *child = node->children[0];
            node->children.erase(node->children.begin());

            if (checkType(child->children[0]->data,"identifier"))
            {
                comma->children.push_back(child->children[0]);
                tau->children.push_back(child->children[1]);
            }
            else if (checkType(child->children[1]->data,"identifier"))
            {
                comma->children.push_back(child->children[1]);
                tau->children.push_back(child->children[0]);
            }
            else
            {
                throw std::invalid_argument("Invalid syntax: Expected an identifier");
            }

        }
        node->data = "=";
        node->children.push_back(comma);
        node->children.push_back(tau);
    }
};

#endif