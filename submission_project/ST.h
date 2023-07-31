#ifndef ST_H
#define ST_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "AST.h"

using namespace std;

class ST{
    public:
        ST(Node* root){
            this->st_root = root;
        }

        Node* get_root(){
            return this->st_root;
        }

        void standardize(){
            construct_ST(this->st_root);
        }

        void construct_ST(Node* node){
            for (int i = 0; i < node->children.size(); i++){
                construct_ST(node->children[i]);
            }
            rec_standardize_AST(node);
        }

        void print_ST(){
            std::cout << "==================================" << endl;
            std::cout << "Printing the Standard Tree ( BFS )" << endl;
            std::cout << "==================================" << endl;

            std::cout << "\n\n" << endl;

            vector<Node*> queue;
            queue.push_back(this->st_root);

            while (queue.size() > 0)
            {
                Node* cur_node = queue[0];
                queue.erase(queue.begin());
                std::cout << "Parent Node -> " << cur_node->data << endl;
                std::cout << "Children -> " << endl;
                for (int i = 0; i < cur_node->children.size(); i++)
                {
                    queue.push_back(cur_node->children[i]);
                    std::cout << "Child Node -> " << cur_node->children[i]->data << endl;
                }
                std::cout << "\n\n" << endl;
        }
    }

    private:
        Node* st_root;

        void rec_standardize_AST(Node* node){
            if (node->data.compare("let") == 0){
                standardize_let(node);
            }  
            else if (node->data.compare("where") == 0){
                standardize_where(node);
            }
            else if (node->data.compare("fcn_form") == 0){
                standardize_fcn_form(node);
            }
            else if (node->data.compare("within") == 0){
                standardize_within(node);
            }
            else if (node->data.compare("and") == 0){
                standardize_and(node);
            }
            else if (node->data.compare("@") == 0){
                standardize_at(node);
            }
            else if (node->data.compare("rec") == 0){
                standardize_rec(node);
            }
            else{
            }
        }

        bool CheckType(string token, string str_type){

            vector<string> keywords = {"let", "in", "fn", "where", "aug",
                               "or", "not", "gr", "ge", "ls", "le", "eq", "ne",
                               "true", "false", "nil", "dummy", "within", "and", 
                               "rec"};

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

        void standardize_let(Node* node){
            Node* eq_child = node->children[0];
            Node* p_child = node->children[1];

            Node* x_node = eq_child->children[0];
            Node* e_node = eq_child->children[1];

            node->children.clear();
            eq_child->children.clear();

            node->data = "gamma";
            eq_child->data = "lambda";

            eq_child->children.push_back(x_node);
            eq_child->children.push_back(p_child);

            node->children.push_back(eq_child);
            node->children.push_back(e_node);
        }

        void standardize_where(Node* node){
            Node* p_node = node->children[0];
            Node* eq_node = node->children[1];

            Node* x_node = eq_node->children[0];
            Node* e_node = eq_node->children[1];

            node->children.clear();
            eq_node->children.clear();

            node->data = "gamma";
            eq_node->data = "lambda";

            eq_node->children.push_back(x_node);
            eq_node->children.push_back(p_node);

            node->children.push_back(eq_node);
            node->children.push_back(e_node);
        }

        void standardize_fcn_form(Node* node){
            Node* p_node = node->children[0];

            vector<Node*> v_plus_nodes;

            for (int i=1; i<node->children.size()-1; i++){
                v_plus_nodes.push_back(node->children[i]);
            }

            Node* e_node = node->children[node->children.size()-1];
            node->children.clear();
            node->data = "=";
            node->children.push_back(p_node);
            Node* cur_node = node;

            for (int i=0; i<v_plus_nodes.size(); i++){
                Node* lambda_node = new Node();
                lambda_node->data = "lambda";
                lambda_node->children.push_back(v_plus_nodes[i]);
                cur_node->children.push_back(lambda_node);
                cur_node = lambda_node;
            }

            cur_node->children.push_back(e_node);
        }

        void standardize_within(Node* node){
            Node* eq_node1 = node->children[0];
            Node* eq_node2 = node->children[1];

            Node* x_node1 = eq_node1->children[0];
            Node* e_node1 = eq_node1->children[1];

            Node* x_node2 = eq_node2->children[0];
            Node* e_node2 = eq_node2->children[1];

            node->children.clear();
            eq_node1->children.clear();
            eq_node2->children.clear();

            node->data = "=";

            node->children.push_back(x_node2);

            Node* gamma_node = new Node();
            gamma_node->data = "gamma";

            Node* lambda_node = new Node();
            lambda_node->data = "lambda";

            lambda_node->children.push_back(x_node1);
            lambda_node->children.push_back(e_node2);

            gamma_node->children.push_back(lambda_node);
            gamma_node->children.push_back(e_node1);

            node->children.push_back(gamma_node);
        }

        void standardize_at(Node* node){
            Node* e1 = node->children[0];
            Node* n = node->children[1];
            Node* e2 = node->children[2];

            node->data = "gamma";
            node->children.clear();

            Node* gamma_node = new Node();
            gamma_node->data = "gamma";

            gamma_node->children.push_back(n);
            gamma_node->children.push_back(e1);

            node->children.push_back(gamma_node);
            node->children.push_back(e2);
        }

        void standardize_and(Node* node){
            vector<Node*> x_nodes, e_nodes;

            for (int i=0; i<node->children.size(); i++){
                Node* eq_node = node->children[i];

                Node* x_node = eq_node->children[0];
                Node* e_node = eq_node->children[1];

                x_nodes.push_back(x_node);
                e_nodes.push_back(e_node);
            }

            node->children.clear();
            node->data = "=";

            Node* comma_node = new Node();
            comma_node->data = ",";

            Node* tau_node = new Node();
            tau_node->data = "tau";

            for (int i=0; i<x_nodes.size(); i++){
                comma_node->children.push_back(x_nodes[i]);
                tau_node->children.push_back(e_nodes[i]);
            }

            node->children.push_back(comma_node);
            node->children.push_back(tau_node);
        }

        void standardize_rec(Node* node){
            Node* eq_node = node->children[0];

            Node* x_node = eq_node->children[0];
            Node* e_node = eq_node->children[1];

            node->children.clear();
            node->data = "=";

            Node* gamma_node = new Node();
            gamma_node->data = "gamma";

            Node* lambda_node = new Node();
            lambda_node->data = "lambda";

            Node* y_star_node = new Node();
            y_star_node->data = "Y*";

            lambda_node->children.push_back(x_node);
            lambda_node->children.push_back(e_node);

            gamma_node->children.push_back(y_star_node);
            gamma_node->children.push_back(lambda_node);

            node->children.push_back(x_node);
            node->children.push_back(gamma_node);
        }

};

#endif