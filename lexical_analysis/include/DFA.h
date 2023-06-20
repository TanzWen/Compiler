#ifndef DFA_H
#define DFA_H

#include "NFA.h"

#include <unordered_map>
#include <algorithm>

class DFA : public NFA
{
protected:
    fstream dfa_file;            // 可视化dfa文件
    int dfa_state_num;           // dfa数量
    string token_name;           // token名称 

    vector<head_node> dfa_graph;
    vector<head_node> min_dfa_graph;
    

    // 从nfa中找到当前状态的闭包
    set<int> find_closures(int id);


    void nfa_2_dfa();
    void min_dfa();


public:
    DFA(string pe);
    DFA(string pe, string name);
    void show_dfa();
    bool check(string str);
    string get_token_name();
};

#endif