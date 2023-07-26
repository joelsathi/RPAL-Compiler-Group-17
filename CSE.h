#ifndef CSE_H
#define CSE_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <queue>
#include <map>
#include "AST.h"
#include "Grammar.h"
#include "ST.h"
#include <fstream>
#include <math.h>

using namespace std;

struct env_var{
    int env_num;
};

struct lambda_var{
    vector<string> bdd_vars;
    int bdd_var_cnt = 0;

    int ctrl_no;
    int env_no;
};

struct eta_var{
    vector<string> bdd_vars;
    int bdd_var_cnt;

    int ctrl_no;
    int env_no;
};

struct tau_var{
    int tau_no;
};

struct cond_var{
    int if_cond_ctrl_no;
    int else_cond_ctrl_no;
};

struct binop_var{
    string op;
};

struct unop_var{
    string op;
};

struct ctrl_and_stack_var{
    bool is_env = false;
    bool is_lambda_var = false;
    bool is_eta_var = false;
    bool is_tau_var = false;
    bool is_cond_var = false;
    bool is_binop_var = false;
    bool is_unop_var = false;
    bool is_name_var = false;
    bool is_bool_var = false;
    bool is_tuple_var = false;
    bool is_func_var = false;
    bool is_int_var = false;
    bool is_string_var = false;
    bool is_dummy_var = false;

    string name_var;
    bool bool_var;
    env_var env;
    lambda_var lambda;
    eta_var eta;
    tau_var tau;
    cond_var cond;
    binop_var binop;
    unop_var unop;
    vector<ctrl_and_stack_var> tuple_elements;
    string func_name;
    int int_var;
    string string_var;
    string dummy;

    int conc_func_cnt = 2;
};

/**
 * @brief TODO: list
 * 1. change tokens to integers from name var -> DONE
 * 2. change tokens to strings from name var -> DONE
 * 3. Check the functions 
 * 4. Test
 */

class CSE{
    public:
        CSE(Node* st_root){
             this->st_root = st_root;
        }

        void solve_CSE(){
            // std::cout << "Solving CSE" << endl;
            initialize_control_structures();
            // std::cout << "Control Structures Initialized" << endl;
            // print_ctrl_structures();
            initialize_cse_control_and_stack();
            // std::cout << "CSE Control and Stack Initialized" << endl;

            // std::cout << "Solving CSE machine" << endl;
            solve_CSE_Machine();
            // std::cout << "CSE machine solved" << endl;
        }

        void print_ctrl_structures(){
            ofstream myfile;
            myfile.open ("CSE_data\\ctrl_structures.txt");
            for (int i = 0; i < this->control_structures.size(); i++){
               myfile << "========================================\n";
               myfile << "\nControl Structure " << i << endl;
                for (int j = 0; j < this->control_structures[i].size(); j++){
                    if (this->control_structures[i][j].is_lambda_var){
                       myfile << "\nLambda Var:\nBounded Variables: " << endl;
                        for (int k = 0; k < this->control_structures[i][j].lambda.bdd_var_cnt; k++){
                           myfile << this->control_structures[i][j].lambda.bdd_vars[k] << " ";
                        }
                       myfile << endl;
                       myfile << "ctrl_no: " << this->control_structures[i][j].lambda.ctrl_no << endl;
                       myfile << endl;
                    }
                    else if (this->control_structures[i][j].is_eta_var){
                       myfile << "Eta Var:\nBounded Variables: " << endl;
                        for (int k = 0; k < this->control_structures[i][j].eta.bdd_var_cnt; k++){
                           myfile << this->control_structures[i][j].eta.bdd_vars[k] << " ";
                        }
                       myfile << endl;
                       myfile << "ctrl_no: " << this->control_structures[i][j].eta.ctrl_no << endl;
                    }
                    else if (this->control_structures[i][j].is_tau_var){
                       myfile << "Tau Var: " << this->control_structures[i][j].tau.tau_no << endl;
                    }
                    else if (this->control_structures[i][j].is_cond_var){
                       myfile << "Cond Var:\nif_ctrl_no: " << this->control_structures[i][j].cond.if_cond_ctrl_no << " else_ctrl_no: " << this->control_structures[i][j].cond.else_cond_ctrl_no << endl;
                    }
                    else if (this->control_structures[i][j].is_binop_var){
                       myfile << "Binop Var: " << this->control_structures[i][j].binop.op << endl;
                    }
                    else if (this->control_structures[i][j].is_unop_var){
                       myfile << "Unop Var: " << this->control_structures[i][j].unop.op << endl;
                    }
                    else if (this->control_structures[i][j].is_env){
                       myfile << "Env Var: " << this->control_structures[i][j].env.env_num << endl;
                    }
                    else if (this->control_structures[i][j].is_func_var){
                        myfile << "Func Var: " << this->control_structures[i][j].func_name << endl;
                    }
                    else if (this->control_structures[i][j].is_int_var){
                        myfile << "Int Var: " << this->control_structures[i][j].int_var << endl;
                    }
                    else if (this->control_structures[i][j].is_string_var){
                        myfile << "String Var: " << this->control_structures[i][j].string_var << endl;
                    }
                    else if (this->control_structures[i][j].is_name_var){
                       myfile << "Name Var: " << this->control_structures[i][j].name_var << endl;
                    }
                    else if (this->control_structures[i][j].is_tuple_var){
                        myfile << "Tuple Var:\n";
                        for (int k = 0; k < this->control_structures[i][j].tuple_elements.size(); k++){
                            myfile << print_ctrl_and_stack_var(this->control_structures[i][j].tuple_elements[k]) << " ";
                        }
                        myfile << endl;
                    }
                    else if (this->control_structures[i][j].is_bool_var){
                        myfile << "Bool Var: " << this->control_structures[i][j].bool_var << endl;
                    }
                    else if (this->control_structures[i][j].is_dummy_var){
                        myfile << "Dummy Var: " << this->control_structures[i][j].dummy << endl;
                    }
                }
               myfile << "========================================\n";
            }
            myfile.close();
            
        }
    
    private:
        // Root of the ST
        Node* st_root;

        // Binary operators
        vector<string> binary_operators = {"or", "gr", "ge", "ls", "le", "eq", "ne", "&", "aug",
                                            "-", "+", "=", "/", "*", ">", ">=", "<", "<=", "**"};

        // Predefined functions
        vector<string> predefined_functions = {"Print", "Isinteger", "Isstring", "Istuple", "Isfunction", 
                                                "Isdummy", "Conc", "Stem", "Stern", "Order", "Null", "ItoS"};
        
        // The current control structure contents
        vector<ctrl_and_stack_var> current_control_structure;
        // Maintain the control structures
        vector<vector<ctrl_and_stack_var>> control_structures;
        // Keep track of the control structure number
        int ctrl_cnt = 1;
        // Queue to do pre-order traversal
        queue<Node*> q;

        // Maintain a control for the CSE machine
        vector<ctrl_and_stack_var> control;
        // Maintain a stack for the CSE machine
        vector<ctrl_and_stack_var> stack;
        // Keep track of the rules used
        int rule_used;

        // Maintain the environment variables in a map
        map<int, vector<string>> env_vars;
        map<int, vector<ctrl_and_stack_var>> env_vals;

        // Maintain the environment tree
        // first -> cur_env
        // second -> parent_env
        map<int, int> env_tree;
        int env_number = 0;

        int cur_env_number = 0;
        map<int, int> prev_env;

        std::ofstream cse_file;

        void print_control_and_stack_in_CSE(){
            
            this->cse_file << "==================================================\n";
            this->cse_file << "===================Control State==================\n";

            for (int i=0; i<control.size(); i++){
                if (this->control[i].is_lambda_var){
                    this->cse_file << "\nLambda Var:\nBounded Variables: " << endl;
                    for (int k = 0; k < this->control[i].lambda.bdd_var_cnt; k++){
                        this->cse_file << this->control[i].lambda.bdd_vars[k] << " ";
                    }
                    this->cse_file << endl;
                    this->cse_file << "ctrl_no: " << this->control[i].lambda.ctrl_no << endl;
                    this->cse_file << endl;
                }
                else if (this->control[i].is_eta_var){
                    this->cse_file << "Eta Var:\nBounded Variables: " << endl;
                    for (int k = 0; k < this->control[i].eta.bdd_var_cnt; k++){
                        this->cse_file << this->control[i].eta.bdd_vars[k] << " ";
                    }
                    this->cse_file << endl;
                    this->cse_file << "ctrl_no: " << this->control[i].eta.ctrl_no << endl;
                }
                else if (this->control[i].is_tau_var){
                    this->cse_file << "Tau Var: " << this->control[i].tau.tau_no << endl;
                }
                else if (this->control[i].is_cond_var){
                    this->cse_file << "Cond Var:\nif_ctrl_no: " << this->control[i].cond.if_cond_ctrl_no << " else_ctrl_no: " << this->control[i].cond.else_cond_ctrl_no << endl;
                }
                else if (this->control[i].is_binop_var){
                    this->cse_file << "Binop Var: " << this->control[i].binop.op << endl;
                }
                else if (this->control[i].is_unop_var){
                    this->cse_file << "Unop Var: " << this->control[i].unop.op << endl;
                }
                else if (this->control[i].is_env){
                    this->cse_file << "Env Var: " << this->control[i].env.env_num << endl;
                }
                else if (this->control[i].is_name_var){
                    this->cse_file << "Name Var: " << this->control[i].name_var << endl;
                }
                else if (this->control[i].is_func_var){
                    this->cse_file << "Func Var: " << this->control[i].func_name << endl;
                }
                else if (this->control[i].is_int_var){
                    this->cse_file << "Int Var: " << this->control[i].int_var << endl;
                }
                else if (this->control[i].is_string_var){
                    this->cse_file << "String Var: " << this->control[i].string_var << endl;
                }
                else if (this->control[i].is_bool_var){
                    this->cse_file << "Bool Var: " << this->stack[i].bool_var << endl;
                }
                else if (this->control[i].is_tuple_var){
                    this->cse_file << "Tuple Var: " << endl;
                    for (int k = 0; k < this->control[i].tuple_elements.size(); k++){
                        this->cse_file << print_ctrl_and_stack_var(this->control[i].tuple_elements[k]) << " ";
                    }
                    this->cse_file << endl;
                }
                else if (this->control[i].is_dummy_var){
                    this->cse_file << "Dummy Var: " << this->control[i].dummy << endl;
                }
            }

            this->cse_file << "====================Stack State===================\n";

            for (int i=0; i<stack.size(); i++){
                if (this->stack[i].is_lambda_var){
                    this->cse_file << "\nLambda Var:\nBounded Variables: " << endl;
                    for (int k = 0; k < this->stack[i].lambda.bdd_var_cnt; k++){
                        this->cse_file << this->stack[i].lambda.bdd_vars[k] << " ";
                    }
                    this->cse_file << endl;
                    this->cse_file << "ctrl_no: " << this->stack[i].lambda.ctrl_no << endl;
                    // this->cse_file << endl;
                    this->cse_file << "env_no: " << this->stack[i].lambda.env_no << endl;
                    this->cse_file << endl;
                }
                else if (this->stack[i].is_eta_var){
                    this->cse_file << "Eta Var:\nBounded Variables: " << endl;
                    for (int k = 0; k < this->stack[i].eta.bdd_var_cnt; k++){
                        this->cse_file << this->stack[i].eta.bdd_vars[k] << " ";
                    }
                    this->cse_file << endl;
                    this->cse_file << "ctrl_no: " << this->stack[i].eta.ctrl_no << endl;
                    // this->cse_file << endl;
                    this->cse_file << "env_no: " << this->stack[i].eta.env_no << endl;
                    this->cse_file << endl;
                }
                else if (this->stack[i].is_tau_var){
                    this->cse_file << "Tau Var: " << this->stack[i].tau.tau_no << endl;
                    runtime_error("Tau variable in stack");
                }
                else if (this->stack[i].is_cond_var){
                    this->cse_file << "Cond Var:\nif_ctrl_no: " << this->stack[i].cond.if_cond_ctrl_no << " else_ctrl_no: " << this->stack[i].cond.else_cond_ctrl_no << endl;
                    runtime_error("Cond variable in stack");
                }
                else if (this->stack[i].is_binop_var){
                    this->cse_file << "Binop Var: " << this->stack[i].binop.op << endl;
                    runtime_error("Binop variable in stack");
                }
                else if (this->stack[i].is_unop_var){
                    this->cse_file << "Unop Var: " << this->stack[i].unop.op << endl;
                    runtime_error("Unop variable in stack");
                }
                else if (this->stack[i].is_env){
                    this->cse_file << "Env Var: " << this->stack[i].env.env_num << endl;
                }
                else if (this->stack[i].is_name_var){
                    this->cse_file << "Name Var: " << this->stack[i].name_var << endl;
                }
                else if (this->stack[i].is_func_var){
                    this->cse_file << "Func Var: " << this->stack[i].func_name << endl;
                }
                else if (this->stack[i].is_int_var){
                    this->cse_file << "Int Var: " << this->stack[i].int_var << endl;
                }
                else if (this->stack[i].is_string_var){
                    this->cse_file << "String Var: " << this->stack[i].string_var << endl;
                }
                else if (this->stack[i].is_bool_var){
                    this->cse_file << "Bool Var: " << this->stack[i].bool_var << endl;
                }
                else if (this->stack[i].is_tuple_var){
                    this->cse_file << "Tuple Var: " << endl;
                    for (int k = 0; k < this->stack[i].tuple_elements.size(); k++){
                        this->cse_file << print_ctrl_and_stack_var(this->stack[i].tuple_elements[k]) << " ";
                    }
                    this->cse_file << endl;
                }
                else if (this->stack[i].is_dummy_var){
                    this->cse_file << "Dummy Var: " << this->stack[i].dummy << endl;
                }
            }

            this->cse_file << "==================================================\n";
        }

        
        void initialize_control_structures(){
            q.push(this->st_root);

            while (!q.empty()){
                Node* cur_node = q.front();
                q.pop();

                this->current_control_structure.clear();
                pre_order(cur_node);

                this->control_structures.push_back(current_control_structure);
            }
        }

        void pre_order(Node* cur_node){
            if (cur_node == NULL){
                return;
            }

            ctrl_and_stack_var cur_ctrl_var;

            if (cur_node->data == "dummy"){
                cur_ctrl_var.is_dummy_var = true;
                cur_ctrl_var.dummy = cur_node->data;
                this->current_control_structure.push_back(cur_ctrl_var);
            }
            // else if (cur_node->data == "nil"){
            //     cur_ctrl_var.is_tuple_var = true;
            //     this->current_control_structure.push_back(cur_ctrl_var);
            // }
            else if (cur_node->data == "true" || cur_node->data == "false"){
                cur_ctrl_var.is_bool_var = true;
                cur_ctrl_var.bool_var = (cur_node->data == "true");
                this->current_control_structure.push_back(cur_ctrl_var);
            }
            else if (cur_node->data == "lambda"){
                vector<string> bdd_vars;

                if (cur_node->children[0]->data == ","){
                    for (int i = 0; i < cur_node->children[0]->children.size(); i++){
                        bdd_vars.push_back(cur_node->children[0]->children[i]->data);
                    }
                }
                else{
                    for (int i = 0; i < cur_node->children.size()-1; i++){
                        bdd_vars.push_back(cur_node->children[i]->data);
                    }
                }

                cur_ctrl_var.is_lambda_var = true;
                cur_ctrl_var.lambda.bdd_vars = bdd_vars;
                cur_ctrl_var.lambda.ctrl_no = this->ctrl_cnt;
                cur_ctrl_var.lambda.bdd_var_cnt = bdd_vars.size();

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

                // Need to stop here and push the current control structure
                this->ctrl_cnt++;

                // Push the last child of the lambda node
                this->q.push(cur_node->children[cur_node->children.size()-1]);
            }

            else if (cur_node->data == "tau"){
                cur_ctrl_var.is_tau_var = true;
                cur_ctrl_var.tau.tau_no = cur_node->children.size();

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

                for (int i = 0; i < cur_node->children.size(); i++){
                    pre_order(cur_node->children[i]);
                }

            }

            else if (cur_node->data == "->"){
                cur_ctrl_var.is_cond_var = true;

                cur_ctrl_var.cond.if_cond_ctrl_no = this->ctrl_cnt;
                cur_ctrl_var.cond.else_cond_ctrl_no = this->ctrl_cnt + 1;

                this->ctrl_cnt += 2;

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

                // Push the if and else conditions to the queue
                q.push(cur_node->children[1]);
                q.push(cur_node->children[2]);
                pre_order(cur_node->children[0]);

            }

            else if ((cur_node->data == "not") || (cur_node->data == "neg") || ((cur_node->data == "+") && (cur_node->children.size() == 1))){
                cur_ctrl_var.is_unop_var = true;
                cur_ctrl_var.unop.op = cur_node->data;

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

                pre_order(cur_node->children[0]);

            }

            else if (std::find(binary_operators.begin(), binary_operators.end(), cur_node->data) != binary_operators.end()){
                cur_ctrl_var.is_binop_var = true;
                cur_ctrl_var.binop.op = cur_node->data;

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

                pre_order(cur_node->children[0]);
                pre_order(cur_node->children[1]);

            }

            else if (std::find(predefined_functions.begin(), predefined_functions.end(), cur_node->data) != predefined_functions.end()){
                cur_ctrl_var.is_func_var = true;
                cur_ctrl_var.func_name = cur_node->data;

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

            }
            else if (regex_match(cur_node->data, regex("[0-9]+"))){
                cur_ctrl_var.is_int_var = true;
                cur_ctrl_var.int_var = stoi(cur_node->data);

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

            }
            else if (regex_match(cur_node->data, regex("\'.*\'"))){
                cur_ctrl_var.is_string_var = true;
                cur_ctrl_var.string_var = cur_node->data.substr(1, cur_node->data.length()-2);

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

            }
            else{
                cur_ctrl_var.is_name_var = true;
                cur_ctrl_var.name_var = cur_node->data;

                // Push the element to the current control structure
                this->current_control_structure.push_back(cur_ctrl_var);

                // std::cout << "Name Var: " << cur_ctrl_var.name_var << endl;

                for (int i = 0; i < cur_node->children.size(); i++){
                    pre_order(cur_node->children[i]);
                }
            }

        }

        void initialize_cse_control_and_stack(){
            ctrl_and_stack_var env_ctrl_var;
            env_ctrl_var.is_env = true;
            env_ctrl_var.env.env_num = 0;

            this->env_tree[0] = -1;
            this->prev_env[0] = -1;

            this->control.push_back(env_ctrl_var);

            for (int i=0; i<this->control_structures[0].size(); i++){
                ctrl_and_stack_var cur_ctrl_var = this->control_structures[0][i];
                this->control.push_back(cur_ctrl_var);
            }

            this->stack.push_back(env_ctrl_var);
        }

        ctrl_and_stack_var apply_functions(ctrl_and_stack_var func, ctrl_and_stack_var var){
            if (! func.is_func_var){
                std::cout << "Error: Not a function" << endl;
                std::exit(0);
            }

            if (func.func_name == "Print"){
                // std::cout << "Print function: ";
                string res = print_ctrl_and_stack_var(var);
                cout << res;
                return var;
            }

            if (func.func_name == "ItoS"){
                ctrl_and_stack_var result;
                result.is_string_var = true;
                result.string_var = to_string(var.int_var);
                return result;
            }
            // FIXME: "Isfunction"

            if (func.func_name == "Isfunction"){
                ctrl_and_stack_var result;
                result.is_bool_var = true;
                result.bool_var = var.is_func_var;
                return result;
            }

            if (func.func_name == "Isdummy"){
                ctrl_and_stack_var result;
                result.is_bool_var = true;
                result.bool_var = var.is_dummy_var;
                return result;
            }

            if (func.func_name == "Isinteger"){
                ctrl_and_stack_var result;
                result.is_bool_var = true;
                result.bool_var = var.is_int_var;
                return result;
            }

            if (func.func_name == "Isstring"){
                ctrl_and_stack_var result;
                result.is_bool_var = true;
                result.bool_var = var.is_string_var;
                return result;
            }

            if (func.func_name == "Istuple"){
                ctrl_and_stack_var result;
                result.is_bool_var = true;
                result.bool_var = var.is_tuple_var;
                if (var.is_name_var && var.name_var == "nil"){
                    result.bool_var = true;
                }
                return result;
            }
            
            // FIXME: Have to add a counter to keep track of the number of elements concatenated
            if (func.func_name == "Conc"){
                func.is_string_var = true;
                func.conc_func_cnt -= 1;
                if (var.is_string_var){
                    if (func.conc_func_cnt == 1){
                        func.string_var = var.string_var;
                    }
                    else if (func.conc_func_cnt == 0){
                        func.string_var = func.string_var + var.string_var;
                        func.is_func_var = false;
                        func.func_name = "";
                    }
                    else{
                        std::cout << "Invalid number of arguments for Conc" << endl;
                        std::exit(0);
                    }
                    return func;
                }
                else{
                    std::cout << "Conc operation only defined on Strings" << endl;
                    std::exit(0);
                }
            }

            if (func.func_name == "Stem"){
                ctrl_and_stack_var result;
                result.is_string_var = true;
                if (var.is_string_var){
                    result.string_var = var.string_var.substr(0, 1);
                    return result;
                }
                else{
                    std::cout << "Stem operation only defined on Strings" << endl;
                    std::exit(0);
                }
            }

            if (func.func_name == "Stern"){
                ctrl_and_stack_var result;
                result.is_string_var = true;
                if (var.is_string_var){
                    result.string_var = var.string_var.substr(1, var.name_var.length()-1);
                    return result;
                }
                else{
                    std::cout << "Stern operation only defined on Strings" << endl;
                    std::exit(0);
                }
            }

            if (func.func_name == "Order"){
                if (var.is_tuple_var || (var.is_name_var && var.name_var == "nil")){
                    ctrl_and_stack_var result;
                    result.is_int_var = true;
                    result.int_var = var.tuple_elements.size();
                    if (var.is_name_var && var.name_var == "nil"){
                        result.int_var = 0;
                    }
                    return result;
                }
                else{
                    std::cout << "Order operation only defined on Tuples" << endl;
                    std::exit(0);
                }
            }

            if (func.func_name == "Null"){
                if (var.is_tuple_var){
                    ctrl_and_stack_var result;
                    result.is_bool_var = true;
                    result.bool_var = (var.tuple_elements.size() == 0);
                    return result;
                }
                else{
                    std::cout << "Null operation only defined on Tuples" << endl;
                    std::exit(0);
                }
            }

            std::cout << "Function not defined!" << endl;
            std::exit(0);

            ctrl_and_stack_var result;
            return result;
            // Implement the rest of the functions
        }

        ctrl_and_stack_var solve_binary_ops(string operator_to_apply, ctrl_and_stack_var rand1, ctrl_and_stack_var rand2){

            ctrl_and_stack_var result;

            if (operator_to_apply == "aug"){
                result.is_tuple_var = true;
                if (rand1.is_tuple_var){
                    result.tuple_elements = rand1.tuple_elements;
                }
                else if (rand1.is_int_var || rand1.is_bool_var || rand1.is_string_var){    
                    result.tuple_elements.push_back(rand1);
                }
                else{
                    if ((rand1.is_name_var && rand1.name_var == "nil")){
                        // Do nothing
                    }
                    else{
                        std::cout << "rand1 set naa!" << endl;
                        std::exit(0);
                    }
                }
                if (rand2.is_int_var || rand2.is_bool_var || rand2.is_string_var || rand2.is_tuple_var){
                    result.tuple_elements.push_back(rand2);
                }
                else{
                    if ((rand2.is_name_var && rand2.name_var == "nil")){
                        // Do nothing
                    }
                    else{
                        std::cout << "rand2 set naa!" << endl;
                        std::exit(0);
                    }
                }
                return result;
            }

            if (rand1.is_string_var && rand2.is_string_var){
                if (operator_to_apply == "eq"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.string_var == rand2.string_var);
                    return result;
                }
                if (operator_to_apply == "ne"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.string_var != rand2.string_var);
                    return result;
                }
                if ((operator_to_apply == "ls" )|| (operator_to_apply == "<")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.string_var < rand2.string_var);
                    return result;
                }
                if ((operator_to_apply == "gr" )|| (operator_to_apply == ">")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.string_var > rand2.string_var);
                    return result;
                }
                if ((operator_to_apply == "le" )|| (operator_to_apply == "<=")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.string_var <= rand2.string_var);
                    return result;
                }
                if ((operator_to_apply == "ge" )|| (operator_to_apply == ">=")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.string_var >= rand2.string_var);
                    return result;
                }

                std::cout << "Operation not defined on strings" << endl;
                std::exit(0);
            }

            if (rand1.is_bool_var && rand2.is_bool_var){
                if (operator_to_apply == "eq"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.bool_var == rand2.bool_var);
                    return result;
                }
                if (operator_to_apply == "ne"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.bool_var != rand2.bool_var);
                    return result;
                }
                if (operator_to_apply == "or"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.bool_var | rand2.bool_var);
                    return result;
                }
                if (operator_to_apply == "&"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.bool_var & rand2.bool_var);
                    return result;
                }

                std::cout << "Operation not defined on bools" << endl;
                std::exit(0);
            }

            if (rand1.is_int_var && rand2.is_int_var){
                if (operator_to_apply == "eq"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.int_var == rand2.int_var);
                    return result;
                }
                if (operator_to_apply == "ne"){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.int_var != rand2.int_var);
                    return result;
                }
                if ((operator_to_apply == "ls" )|| (operator_to_apply == "<")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.int_var < rand2.int_var);
                    return result;
                }
                if ((operator_to_apply == "gr" )|| (operator_to_apply == ">")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.int_var > rand2.int_var);
                    return result;
                }
                if ((operator_to_apply == "le" )|| (operator_to_apply == "<=")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.int_var <= rand2.int_var);
                    return result;
                }
                if ((operator_to_apply == "ge" )|| (operator_to_apply == ">=")){
                    result.is_bool_var = true;
                    result.bool_var = (rand1.int_var >= rand2.int_var);
                    return result;
                }
                if (operator_to_apply == "+"){
                    result.is_int_var = true;
                    result.int_var = (rand1.int_var + rand2.int_var);
                    return result;
                }
                if (operator_to_apply == "-"){
                    result.is_int_var = true;
                    result.int_var = (rand1.int_var - rand2.int_var);
                    return result;
                }
                if (operator_to_apply == "*"){
                    result.is_int_var = true;
                    result.int_var = (rand1.int_var * rand2.int_var);
                    return result;
                }
                if (operator_to_apply == "/"){
                    result.is_int_var = true;
                    result.int_var = (rand1.int_var / rand2.int_var);
                    return result;
                }
                if (operator_to_apply == "**"){
                    result.is_int_var = true;
                    result.int_var = pow(rand1.int_var, rand2.int_var);
                    return result;
                }

                std::cout << "Operation not defined on integers" << endl;
                std::exit(0);
            }

            return result;
        }

        ctrl_and_stack_var solve_unary_ops(string operator_to_apply, ctrl_and_stack_var rand){

            ctrl_and_stack_var result;

            if (operator_to_apply == "not"){
                if (rand.is_bool_var){
                    result.is_bool_var = true;
                    result.bool_var = !rand.bool_var;
                    return result;
                }
                else{
                    std::cout << "Operation only defined on booleans" << endl;
                    std::exit(0);
                }
            }

            if (operator_to_apply == "neg"){
                if (rand.is_int_var){
                    result.is_int_var = true;
                    result.int_var = -rand.int_var;
                    return result;
                }
                else{
                    std::cout << "Operation only defined on integers" << endl;
                    std::exit(0);
                }
            }

            return result;

        }

        void use_rule_1(){
            ctrl_and_stack_var cur_ctrl_var = this->control.back();
            this->control.pop_back();

            if (cur_ctrl_var.is_int_var || cur_ctrl_var.is_bool_var || 
                cur_ctrl_var.is_string_var || cur_ctrl_var.is_func_var || cur_ctrl_var.is_dummy_var ||
                (cur_ctrl_var.is_name_var && (cur_ctrl_var.name_var == "Y*" || cur_ctrl_var.name_var == "nil"))){
                this->stack.push_back(cur_ctrl_var);
                return;
            }
            else{
                // std::cout << "Searching for variable: " << cur_ctrl_var.name_var << endl;
                // Check if it is a bounded variable
                int cur_env_no = this->cur_env_number;

                // for (int i=this->stack.size()-1; i>=0; i--){
                //     if (this->stack[i].is_env){
                //         cur_env_no = this->stack[i].env.env_num;
                //         break;
                //     }
                // }
                // std::cout << "Current env no: " << cur_env_no << endl;
                // iterate through the env_var map and find the value of the variable
                while (cur_env_no != -1){
                    for (int i=0; i< this->env_vars[cur_env_no].size(); i++){
                        // std::cout << "Checking variable: " << this->env_vars[cur_env_no][i] << endl;
                        if (this->env_vars[cur_env_no][i] == cur_ctrl_var.name_var){
                            // std::cout << "Found variable: " << cur_ctrl_var.name_var << " in env: " << cur_env_no << " with value: " << this->env_vals[cur_env_no][i].name_var << endl;
                            this->stack.push_back(this->env_vals[cur_env_no][i]);
                            return;
                        }
                    }

                    cur_env_no = this->env_tree[cur_env_no];
                    // std::cout << "updated env no: " << cur_env_no << endl;
                }
                // Raise an error if the variable is not found
                int val = cur_ctrl_var.name_var[0];
                // if (val == 0){
                //     return;
                // }
                std::cout << "Variable not found: " << val << endl;
                std::cout << "Variable not found: " << cur_ctrl_var.name_var << endl;
                string value = this->print_ctrl_and_stack_var(cur_ctrl_var);
                std::cout << value << endl;
                std::exit(0);
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
        }

        void use_rule_2(){
            ctrl_and_stack_var cur_ctrl_var = this->control.back();
            this->control.pop_back();
            cur_ctrl_var.lambda.env_no = this->env_number;
            this->stack.push_back(cur_ctrl_var);
        }

        void use_rule_3(){
            ctrl_and_stack_var gamma = this->control.back();
            this->control.pop_back();
            
            ctrl_and_stack_var rator = this->stack.back();
            this->stack.pop_back();

            ctrl_and_stack_var rand = this->stack.back();
            this->stack.pop_back();

            ctrl_and_stack_var new_ctrl_var;
            // if (rator.func_name == "aug"){
            //     ctrl_and_stack_var rand2 = this->stack.back();
            //     this->stack.pop_back();

            //     new_ctrl_var = apply_functions(rator, rand, rand2);
            // }
            // else{
            //     new_ctrl_var = apply_functions(rator, rand);
            // }

            new_ctrl_var = apply_functions(rator, rand);

            if (rator.func_name != "Print" )
                this->stack.push_back(new_ctrl_var);
            else{
                ctrl_and_stack_var dummy_ctrl;
                dummy_ctrl.is_dummy_var = true;
                dummy_ctrl.dummy = "dummy";
                this->stack.push_back(dummy_ctrl);
            }
        }

        void use_rule_4(bool if_use_11 = false){
            // Remove Gamma node from control
            this->control.pop_back();

            // Get the lambda node from stack
            ctrl_and_stack_var lambda = this->stack.back();
            this->stack.pop_back();

            

            // Link the child with the parent env
            this->env_tree[this->env_number+1] = lambda.lambda.env_no;
            // Link the previous environmnet
            this->prev_env[this->env_number+1] = this->cur_env_number;

            if (! if_use_11){
                ctrl_and_stack_var rand = this->stack.back();
                this->stack.pop_back();
                // Add the bounded variable and the corresponding value to the env
                this->env_vars[this->env_number+1].push_back(lambda.lambda.bdd_vars[0]);
                this->env_vals[this->env_number+1].push_back(rand);
            }
            else{
                ctrl_and_stack_var rand = this->stack.back();
                this->stack.pop_back();

                if (! rand.is_tuple_var){
                    std::cout << "Expected tuple variable" << endl;
                    std::exit(0);
                }

                for (int i=0; i<lambda.lambda.bdd_var_cnt; i++){
                    // Remove the corresponding rand from stack
                    // ctrl_and_stack_var rand = this->stack.back();
                    // this->stack.pop_back();

                    // Add the bounded variable and the corresponding value to the env
                    this->env_vars[this->env_number+1].push_back(lambda.lambda.bdd_vars[i]);
                    this->env_vals[this->env_number+1].push_back(rand.tuple_elements[i]);
                }
            }
            
            // Create new environment and push it to control and stack
            ctrl_and_stack_var new_env;
            new_env.is_env = true;
            new_env.env.env_num = this->env_number+1;
            this->env_number++;

            this->cur_env_number = this->env_number;

            this->control.push_back(new_env);
            this->stack.push_back(new_env);

            // Get the ctrl number of lambda node
            int ctrl_num = lambda.lambda.ctrl_no;
            // Push the elements of the bounded env to control
            for (int i=0; i < this->control_structures[ctrl_num].size(); i++){
                this->control.push_back(this->control_structures[ctrl_num][i]);
            }

        }

        void use_rule_5(){
            ctrl_and_stack_var cur_ctrl_var = this->control.back();
            this->control.pop_back();

            int env_to_disable = cur_ctrl_var.env.env_num;
            this->cur_env_number = this->prev_env[env_to_disable];

            for (int i = this->stack.size()-1; i >= 0 ; i--){
                if (this->stack[i].is_env && (this->stack[i].env.env_num == env_to_disable)){
                    this->stack.erase(this->stack.begin() + i);
                    break;
                }
            }
        }

        void use_rule_6(){
            string bin_op_to_apply = this->control.back().binop.op;
            this->control.pop_back();

            ctrl_and_stack_var rand1 = this->stack.back();
            this->stack.pop_back();
            ctrl_and_stack_var rand2 = this->stack.back();
            this->stack.pop_back();

            ctrl_and_stack_var new_ctrl_var = solve_binary_ops(bin_op_to_apply, rand1, rand2);
            this->stack.push_back(new_ctrl_var);
        }

        void use_rule_7(){
            string un_op_to_apply = this->control.back().unop.op;
            this->control.pop_back();

            ctrl_and_stack_var rand = this->stack.back();
            this->stack.pop_back();

            ctrl_and_stack_var new_ctrl_var = solve_unary_ops(un_op_to_apply, rand);
            this->stack.push_back(new_ctrl_var);
        }

        void use_rule_8(){
            ctrl_and_stack_var cur_ctrl_var = this->control.back();
            this->control.pop_back();

            ctrl_and_stack_var bool_stack_content = this->stack.back();
            this->stack.pop_back();

            int if_ctrl_num = cur_ctrl_var.cond.if_cond_ctrl_no;
            int else_ctrl_num = cur_ctrl_var.cond.else_cond_ctrl_no;

            int ctrl_to_push;

            if (bool_stack_content.bool_var){
                ctrl_to_push = if_ctrl_num;
            }
            else{
                ctrl_to_push = else_ctrl_num;
            }

            for (int i=0; i<this->control_structures[ctrl_to_push].size(); i++){
                this->control.push_back(this->control_structures[ctrl_to_push][i]);
            }
        }

        void use_rule_9(){
            int tau_cnt = this->control.back().tau.tau_no;
            this->control.pop_back();

            ctrl_and_stack_var new_ctrl_var;
            new_ctrl_var.is_tuple_var = true;

            for (int i=0; i<tau_cnt; i++){
                ctrl_and_stack_var rand = this->stack.back();
                this->stack.pop_back();
                if (! (rand.is_tuple_var || rand.is_int_var || rand.is_string_var || rand.is_bool_var)){
                    if (rand.is_name_var && rand.name_var == "nil"){
                        // Do nothing
                    }
                    else{
                        std::cout << "Error: Tuple element is not a tuple, int, string or bool" << std::endl;
                        exit(1);
                    }
                    // std::cout << "Error: Tuple element is not a tuple, int, string or bool" << std::endl;
                    // exit(1);
                }
                new_ctrl_var.tuple_elements.push_back(rand);
            }

            this->stack.push_back(new_ctrl_var);
        }

        void use_rule_10(){
            // remove the gamma node from the control
            this->control.pop_back();

            // remove the tuple from the stack
            ctrl_and_stack_var tuple_elm = this->stack.back();
            this->stack.pop_back();

            // remove the index from the stack
            ctrl_and_stack_var index = this->stack.back();
            this->stack.pop_back();

            // get the desired tuple element
            ctrl_and_stack_var desired_tuple_elm = tuple_elm.tuple_elements[index.int_var-1];

            // push the desired tuple element to the stack
            this->stack.push_back(desired_tuple_elm);
        }

        void use_rule_11(){
            use_rule_4(true);
        }

        void use_rule_12(){
            // Remove the Gamma node from the control
            this->control.pop_back();

            // Remove Y from the stack
            this->stack.pop_back();

            // Get the lambda variable from the stack
            ctrl_and_stack_var cur_stack_var = this->stack.back();
            this->stack.pop_back();

            cur_stack_var.is_lambda_var = false;
            cur_stack_var.is_eta_var = true;
            cur_stack_var.eta.bdd_var_cnt = cur_stack_var.lambda.bdd_var_cnt;
            cur_stack_var.eta.bdd_vars = cur_stack_var.lambda.bdd_vars;
            cur_stack_var.eta.ctrl_no = cur_stack_var.lambda.ctrl_no;
            cur_stack_var.eta.env_no = cur_stack_var.lambda.env_no;

            this->stack.push_back(cur_stack_var);
        }

        void use_rule_13(){
            // Push the gamma node
            ctrl_and_stack_var new_ctrl_var;
            new_ctrl_var.is_name_var = true;
            new_ctrl_var.name_var = this->control.back().name_var;

            this->control.push_back(new_ctrl_var);

            // Push the lambda node
            ctrl_and_stack_var cur_stack_var = this->stack.back();

            cur_stack_var.is_eta_var = false;
            cur_stack_var.is_lambda_var = true;
            cur_stack_var.lambda.bdd_var_cnt = cur_stack_var.eta.bdd_var_cnt;
            cur_stack_var.lambda.bdd_vars = cur_stack_var.eta.bdd_vars;
            cur_stack_var.lambda.ctrl_no = cur_stack_var.eta.ctrl_no;
            cur_stack_var.lambda.env_no = cur_stack_var.eta.env_no;
            
            this->stack.push_back(cur_stack_var);
        }

        void solve_CSE_Machine(){
            // this->cse_file.open("CSE_data\\CSE_machine_states.txt");

            int iteration_cnt = 1;
            while (!this->control.empty()){
                // this->cse_file << "----------------------------" << endl;
                // this->cse_file << "Iteration " << iteration_cnt << endl;
                // this->cse_file << "----------------------------" << endl;
                iteration_cnt++;
                // print_control_and_stack_in_CSE();
                // print_env_tree();

                ctrl_and_stack_var cur_ctrl_var = this->control.back();

                if ((cur_ctrl_var.is_name_var && cur_ctrl_var.name_var != "gamma") || 
                    cur_ctrl_var.is_bool_var || cur_ctrl_var.is_int_var || cur_ctrl_var.is_dummy_var ||
                    cur_ctrl_var.is_string_var || cur_ctrl_var.is_func_var){
                    this->rule_used = 1;
                    use_rule_1();
                }
                else if (cur_ctrl_var.is_lambda_var){
                    this->rule_used = 2;
                    use_rule_2();
                }
                else if (cur_ctrl_var.is_name_var && cur_ctrl_var.name_var == "gamma"){
                    if (this->stack.back().is_lambda_var){
                        if (this->stack.back().lambda.bdd_var_cnt == 1){
                            this->rule_used = 4;
                            use_rule_4();
                        }
                        else{
                            this->rule_used = 11;
                            use_rule_11();
                        }
                    }
                    else if (this->stack.back().is_name_var && this->stack.back().name_var == "Y*"){
                        this->rule_used = 12;
                        use_rule_12();
                    }
                    else if (this->stack.back().is_eta_var){
                        this->rule_used = 13;
                        use_rule_13();
                    }
                    else if (this->stack.back().is_tuple_var){
                        // std::cout << "\nUsing rule 10...\n\n" << endl;
                        this->rule_used = 10;
                        use_rule_10();
                    }
                    else{
                        this->rule_used = 3;
                        use_rule_3();
                    }
                }
                else if (cur_ctrl_var.is_env){
                    this->rule_used = 5;
                    use_rule_5();
                }
                else if (cur_ctrl_var.is_binop_var){
                    this->rule_used = 6;
                    use_rule_6();
                }
                else if (cur_ctrl_var.is_unop_var){
                    this->rule_used = 7;
                    use_rule_7();
                }
                else if (cur_ctrl_var.is_cond_var){
                    this->rule_used = 8;
                    use_rule_8();
                }
                else if (cur_ctrl_var.is_tau_var){
                    this->rule_used = 9;
                    use_rule_9();
                }
                else{
                    std::cout << "Error: No rule found for the current control variable" << endl;
                    if (cur_ctrl_var.is_name_var){
                        std::cout << cur_ctrl_var.name_var << endl;
                    }
                    std::exit(0);
                }

                // this->cse_file << "=====================Rule Used====================\n";
                // this->cse_file << "Rule no: " << this->rule_used << endl;
            }
            // this->cse_file << "----------------------------------------------" << endl;
            // this->cse_file << "------Final Stack and Control Structures------" << endl;
            // this->cse_file << "----------------------------------------------" << endl;
            print_control_and_stack_in_CSE();
            // this->cse_file << "Total number of steps: " << iteration_cnt << endl;
            // this->cse_file.close();
        }

        void print_env_tree(){
            //iterate through the map env_tree
            // print child and parent pair
            for (auto it = this->env_tree.begin(); it != this->env_tree.end(); it++){
                std::cout << "Child: " <<  it->first << " Parent: " << it->second << endl;

                // iterate through the map env_vars map<int, vector<string>> and print the variables
                // and corresponding environment value in the map env_vals map<int, vector<string>>
                for (int i=0; i< this->env_vars[it->first].size(); i++){
                    std::cout << "Variable: " << this->env_vars[it->first][i] << " Value: ";
                    if (this->env_vals[it->first][i].is_lambda_var){
                        std::cout << "lambda" << endl;
                        // std::cout << this->env_vals[it->first][i].lambda.env_no << endl;
                        // std::cout << this->env_vals[it->first][i].lambda.ctrl_no << endl;
                        // std::cout << "Bdd var: " <<this->env_vals[it->first][i].lambda.bdd_vars[0] << endl;
                        
                    }
                    else if (this->env_vals[it->first][i].is_eta_var){
                        std::cout << "eta" << endl;
                        // std::cout << this->env_vals[it->first][i].eta.env_no << endl;
                        // std::cout << this->env_vals[it->first][i].eta.ctrl_no << endl;
                        // std::cout << "Bdd var: " <<this->env_vals[it->first][i].eta.bdd_vars[0] << endl;
                    }
                    else{
                        std::cout << this->print_ctrl_and_stack_var(this->env_vals[it->first][i]) << endl;
                    }
                }

            }
        }

        string print_ctrl_and_stack_var(ctrl_and_stack_var elem){
            string res = "";
            if (elem.is_name_var){
                res += elem.name_var;
            }
            else if (elem.is_dummy_var){
                res += elem.dummy;
            }
            else if (elem.is_int_var){
                res += std::to_string(elem.int_var);
            }
            else if (elem.is_string_var){
                for (int i=0; i<elem.string_var.length(); i++){
                    if (elem.string_var[i] == '\\' && (i+1 < elem.string_var.length()) &&elem.string_var[i+1] == 'n'){
                        // std::cout << "found a new line char" << endl;
                        res += '\n';
                        i++;
                    }
                    else if (elem.string_var[i] == '\\' && (i+1 < elem.string_var.length()) &&elem.string_var[i+1] == 't'){
                        // std::cout << "found a tab char" << endl;
                        res += '\t';
                        i++;
                    }
                    else{
                        res += elem.string_var[i];
                    }
                }
                // res += elem.string_var;
            }
            else if (elem.is_bool_var){
                res += elem.bool_var ? "true" : "false";
            }
            else if (elem.is_tuple_var){
                if (elem.tuple_elements.size() == 0){
                    res += "nil";
                }
                else{
                    res += "(";
                    for (int i=0; i<elem.tuple_elements.size(); i++){
                        res += print_ctrl_and_stack_var(elem.tuple_elements[i]);
                        if (i != elem.tuple_elements.size()-1){
                            res += ", ";
                        }
                    }
                    res += ")";
                }
            }
            else if (elem.is_lambda_var){
                res += "[lambda closure: " + elem.lambda.bdd_vars[0] + ": " + to_string(elem.lambda.ctrl_no) + "]";
            }
            else{
                std::cout << "Error: Can't print the control structure" << endl;
                std::exit(0);
            }
            return res;
        }

};

#endif