#ifndef DFA_H
#define DFA_H

#include "NFA.h"

#include <unordered_map>

class DFA : public NFA
{
protected:
    fstream dfa_file;            // 可视化dfa文件
    int dfa_state_num;           // dfa数量
    state dfa_start;             // 起始
    state dfa_end;               // 终态
    vector<state> dfa_state;     // 存放dfa状态
    vector<edge> dfa_edge;       // 存放dfa的边
    vector<state> min_dfa_state; // 存放最小dfa状态
    vector<edge> min_dfa_edge;   // 存放最小dfa的边
    queue<state> temp;

    vector<state> find_closures(state &s); // 从nfa中找到当前状态的闭包
    int is_count(state s);                // 判断当前状态是否存在于dfa
    state count_id(state s);
    void nfa_2_dfa();
    void min_dfa();
    int is_count_min(state s); // 判断当前状态是否存在于最小dfa

public:
    DFA(string pe);
    void show_closures();
    void show_dfa();
    bool check(string str);
};

#endif