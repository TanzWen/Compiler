/**
 * DFA 的数据结构
 * 采用邻接表的方式存储
 */

#ifndef LR_DFA
#define LR_DFA

#include <set>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "LR_1_grammer.h"

using namespace std;

class shred_data
{
public:
    static vector<LR_1_grammer> grammer;
    static set<string> v_t; // 终结符集合
    static set<string> v_n; // 非终结符集合

    static unordered_map<string, set<string>> first;
    static unordered_map<string, set<string>> follow;
};
class node : public shred_data
{
public:
    int id;
    vector<LR_1_grammer> core;
    vector<LR_1_grammer> grammers;
    unordered_map<string,int>path;


    node(int i);
    node();

    bool is_in_grammer(LR_1_grammer &g);
    bool is_in_core(LR_1_grammer &g);
    void push_back_gram(LR_1_grammer &g);
    set<string> find_first(vector<string> &v);

    bool operator==(const node &n) const
    {
        return core == n.core;
    }
};
class adj_list : public shred_data
{
public:
    vector<node> list;
    int size;
    adj_list();

    void add_node(node &n);
    void add_grammer(int i, LR_1_grammer &g);
    void show();
};
class LR_1_DFA : public shred_data
{
private:
    int gram_count(LR_1_grammer &g);
    int gram_count(int from, LR_1_grammer &g);
    
public:
    adj_list head_list;
    LR_1_DFA();
    void init();
    void show();
};

#endif // !LR_DFA