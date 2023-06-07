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

        /**
         * @brief This will construct the standard tree.
         * 
         */
        void standardize(){

        }

        /**
         * @brief This will print the standard tree.
         * 
         */
        void print_ST(){

        }
    
    private:
        Node* st_root;

        /**
         * @brief This will construct the standard tree recursively.
         * 
         * @param node 
         */
        void construct_ST_rec(Node* node){

        }

        /**
         * @brief This will check the data of the node and standardize by
         * calling the appropriate function.
         * 
         * @param node 
         */
        void stardardize_node(Node* node){

        }

        void standardize_let(Node* node){

        }

        void standardize_where(Node* node){

        }

        void standadize_fcn_form(Node* node){

        }

        void standardize_lambda(Node* node){

        }

        void standardize_within(Node* node){

        }

        void standardize_at(Node* node){

        }

        void standardize_and(Node* node){

        }
};

#endif