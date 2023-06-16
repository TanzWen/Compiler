#ifndef LR1
#define LR1

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <stack>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "LALR_1_grammer.h"
#include "LALR_1_DFA.h"

using namespace std;

class table_item
{
public:
    string action;
    int next;
    LALR_1_grammer gram;

    table_item();
    table_item(int n);
    table_item(string a, int n);
    table_item(string a, LALR_1_grammer &g);

    ~table_item();

    string to_string();
};

class LALR_1
{
private:
    vector<LALR_1_grammer> grammer;
    set<string> v_t; // 终结符集合
    set<string> v_n; // 非终结符集合
    // 两层哈希表代表行和列，第三层 pair 代表编内的数据项为产生式
    unordered_map<int, unordered_map<string, table_item>> LALR_1_prasing_table;
    unordered_map<string, set<string>> first;
    unordered_map<string, set<string>> follow;

    LALR_1_DFA dfa;
    string start;

    void show_stack(stack<string> &s);
    void show_queue(queue<string> &q);

public:
    LALR_1();
    void add_grammer(vector<string> &v0);
    void show_grammer();
    void show_dfa();
    bool LALR_1_check(string &str);
};

#endif