#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

struct Node{
    string data;
    vector<Node*> children;
};

class Graph{
    public:
        Graph(){
            this->root = new Node();
        }

        Node* get_root(){
            return this->root;
        }

        void build_tree(string data, int pop_tree_cnt){

        }

        void push_to_stack(string node_data){

        }

        void print_AST(){

        }

    private:
        vector<string> graph;
        Node* root;
        vector<Node*> node_stack;

        void add_node(Node* node, Node* parent){
            
        }
};

#endif