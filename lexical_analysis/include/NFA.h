#ifndef NFA_H
#define NFA_H
#include <iostream>
#include <string>
#include <set>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <cstdio>

#include "str.h"
#include "state.h"
#include "edge.h"

using namespace std;

class NFA
{
protected:
    fstream nfa_file;            // 可视化输出的文件
    string post_exp;             // 需要转化的后缀表达式
    string in_exp;               // 中缀表达式
    state start;                 // 起始状态
    state end;                   // 终止状态
    set<char> letter;            // 字符集
    int state_num;               // 状态数量
    vector<state> state_vector;  // 状态集合
    vector<edge> edge_vector;    // 边集合
    stack<pair<state, state>> s; // 生成nfa时所需的栈

    void letter_set(); // 获取字符集
    void re_2_nfa();   // 正则表达式转nfa

public:
    NFA();
    NFA(string pe);
    void show_nfa();
};

#endif