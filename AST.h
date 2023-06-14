#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

struct Node
{
    string data;
    string type = "internal node";
    vector<Node *> children;
};

class Graph
{
public:
    Graph()
    {
        this->root = new Node();
    }

    Node *get_root()
    {
        return this->root;
    }

    void build_tree(string data, int pop_tree_cnt)
    {
        if (this->node_stack.size() == 0)
        {
            this->root->data = data;
            this->node_stack.push_back(this->root);
            return;
        }
        if (pop_tree_cnt == 0)
        {
            this->push_to_stack(data);
            return;
        }
        Node *node = new Node();
        node->data = data;

        for (int i = 0; i < pop_tree_cnt; i++)
        {
            Node *curr = this->node_stack.back();
            this->node_stack.pop_back();
            node->children.push_back(curr);
        }

        reverse(node->children.begin(), node->children.end());

        this->node_stack.push_back(this->root);
    }

    void push_to_stack(string node_name, string node_type = "internal node")
    {
        Node *node = new Node();
        node->data = node_name;
        node->type = node_type;
        this->node_stack.push_back(node);
    }

    void print_node_stack()
    {
        for (int i = 0; i < this->node_stack.size(); i++)
        {
            std::cout << this->node_stack[i]->data << endl;
        }
    }

    void print_graph()
    {
        std::cout << "==================================" << endl;
        std::cout << "Printing the graph" << endl;
        std::cout << "==================================" << endl;

        // std::cout << "Root -> " << this->root.data << endl;

        std::cout << "\n\n"
                  << endl;

        // do BFS
        vector<Node *> queue;
        queue.push_back(this->root);

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
    vector<string> graph;
    Node *root;
    vector<Node *> node_stack;

    void add_node(Node *node, Node *parent)
    {
        parent->children.push_back(node);
    }

    void inorder(Node *node)
    {
        if (node->children.size() == 0)
        {
            std::cout << node->data << endl;
            return;
        }
        for (int i = 0; i < node->children.size(); i++)
        {
            inorder(node->children[i]);
        }
    }
};

#endif