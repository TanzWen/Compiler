/**
 * LALR_1_DFA 的数据结构
 * 采用邻接表的方式存储
 */

#ifndef LR_LALR_1_DFA
#define LR_LALR_1_DFA

#include <set>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "LALR_1_grammer.h"

using namespace std;

class shred_data
{
public:
    static vector<LALR_1_grammer> grammer;
    static set<string> v_t; // 终结符集合
    static set<string> v_n; // 非终结符集合

    static unordered_map<string, set<string>> first;
    static unordered_map<string, set<string>> follow;
};
class node : public shred_data
{
public:
    int id;
    vector<LALR_1_grammer> core;
    vector<LALR_1_grammer> grammers;
    unordered_map<string, int> path;

    node(int i);
    node();

    bool is_in_grammer(LALR_1_grammer &g);
    bool is_in_core(LALR_1_grammer &g);
    bool is_similar(node &n);
    void push_back_gram(LALR_1_grammer &g);
    set<string> find_first(vector<string> &v, set<string> &search);

    bool operator==(const node &n) const
    {
        if (core.size() != n.core.size())
            return false;
        for (int i = 0; i < core.size(); i++)
        {
            if (core[i] != n.core[i] || core[i].search != n.core[i].search)
                return false;
        }
        return true;
    }
};
class adj_list : public shred_data
{
public:
    vector<node> list;
    int size;
    adj_list();

    void add_node(node &n);
    void erase_node(int id,int now_id);
    void add_grammer(int i, LALR_1_grammer &g);
    void show();
};
class LALR_1_DFA : public shred_data
{
private:
    int gram_count(LALR_1_grammer &g);
    int gram_count(int from, LALR_1_grammer &g);

public:
    adj_list head_list;
    LALR_1_DFA();
    void init();
    void show();
};

#endif // !LR_LALR_1_DFA