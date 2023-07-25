#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include "AST.h"
#include "Grammar.h"
#include "ST.h"
#include "tokenizer.h"
#include "CSE.h"

#include <fstream>
#include <cstdlib>

using namespace std;

int nodeCounter = 0;

void generateDotFile(Node *root, ofstream &dotFile, int &nodeCounter)
{
    if (root == nullptr)
        return;

    int currentNodeIndex = nodeCounter;

    // if the node has any children then make the shape of the node circle and color it lightblue
    // otherwise make the shape of the node circle and leave it without any color

    // have to set the radius of the circle to a fixed valu

    if (root->children.size() > 0)
        dotFile << "node" << currentNodeIndex << " [label=\"" << root->data << "\",shape=circle, style=\"filled\", fillcolor=\"lightblue\", fontcolor=\"black\", fontsize=14, width=1.5];" << endl;
    else
        dotFile << "node" << currentNodeIndex << " [label=\"" << root->data << "\",shape=circle, style=\"filled\", fillcolor=\"gray\", fontcolor=\"black\", fontsize=14, width=1.5];" << endl;

    // dotFile << "node" << currentNodeIndex << " [label=\"" << root->data << "\"];" << endl;

    for (Node *child : root->children)
    {
        int childNodeIndex = nodeCounter + 1;
        dotFile << "node" << currentNodeIndex << " -- node" << childNodeIndex << ";" << endl;
        nodeCounter++;
        generateDotFile(child, dotFile, nodeCounter);
    }
}

void generateTreeDotFile(Node *root, const string &fileName)
{
    ofstream dotFile(fileName);

    if (dotFile.is_open())
    {
        dotFile << "graph Tree {" << endl;
        dotFile << "node [shape=box, style=\"filled\", fillcolor=\"lightblue\", fontcolor=\"black\"];" << endl;

        int nodeCounter = 0;
        generateDotFile(root, dotFile, nodeCounter);

        dotFile << "}" << endl;

        dotFile.close();

        // std::cout << "Dot file '" << fileName << "' generated successfully." << endl;
    }
    else
    {
        std::cout << "Failed to open dot file." << endl;
    }
}

int main(int argc, char **argv)
{

    Tokenizer tok = Tokenizer();

    vector<string> tokens_to_parse = tok.tokenize(argc, argv);
    Grammar g = Grammar(tokens_to_parse);
    g.parse();

    Node *root = g.get_ast_root();
    // generateTreeDotFile(root, "Generated_Graphs\\ast_tree.dot");
    // system("dot -Tpng -O Generated_Graphs\\ast_tree.dot");
    // system("dot -Tpng -Gdpi=300 -O Generated_Graphs\\ast_tree.dot");

    ST st = ST(root);
    st.standardize();

    Node *st_root = st.get_root();
    // generateTreeDotFile(st_root, "Generated_Graphs\\st_tree.dot");
    // system("dot -Tpng -O Generated_Graphs\\st_tree.dot");

    CSE cse = CSE(st_root);
    cse.solve_CSE();

    return 0;
}