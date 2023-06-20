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

using namespace std;

class edge
{
public:
    int id;
    string path;
    edge()
    {
        id = -1;
        path = "";
    }
    edge(int id, string path)
    {
        this->id = id;
        this->path = path;
    }
};
class head_node
{
public:
    int id;
    bool is_final;
    vector<edge *> next;
    set<int> include;

    head_node()
    {
        id = -1;
        is_final = false;
    }
};

class NFA
{
protected:
    fstream nfa_file;            // 可视化输出的文件
    string post_exp;             // 需要转化的后缀表达式
    string in_exp;               // 中缀表达式
    set<char> letter;            // 字符集
    int state_num;               // 状态数量

    vector<head_node> nfa_graph;
    int start_id;

    void letter_set(); // 获取字符集
    void re_2_nfa();   // 正则表达式转nfa

public:
    NFA();
    NFA(string pe);
    void show_nfa();
};

#endif