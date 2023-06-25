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
#include <fstream>

#include "LR_1_grammer.h"
#include "LR_1_DFA.h"

using namespace std;

class table_item
{
public:
    string action;
    int next;
    LR_1_grammer gram;

    table_item();
    table_item(int n);
    table_item(string a, int n);
    table_item(string a, LR_1_grammer &g);

    ~table_item();

    string to_string();
};
class tree_node
{
public:
    static int count;
    int id;
    string name;
    vector<tree_node *> children;
    string value;

    tree_node() {}
    tree_node(string n) : name(n), id(count), value("") { count++; }
    ~tree_node() {}

    bool is_leaf() { return children.empty(); }
    void DFS();
    void DFS(queue<string> &token_value);
    string to_string()
    {
        if (name == "string")
        {
            value.replace(0, 1, "\\\"");
            value.replace(value.size() - 1, 1, "\\\"");
        }
        return name + "\\n" + value;
    }
};

class LR_1
{
private:
    vector<LR_1_grammer> grammer;
    set<string> v_t; // 终结符集合
    set<string> v_n; // 非终结符集合
    // 两层哈希表代表行和列，第三层 pair 代表编内的数据项为产生式
    unordered_map<int, unordered_map<string, table_item>> LR_1_prasing_table;
    unordered_map<string, set<string>> first;
    unordered_map<string, set<string>> follow;

    tree_node *root;

    LR_1_DFA dfa;
    string start;
    fstream out;
    fstream out2;

    void show_stack(stack<string> &s);
    void show_queue(queue<string> &q);

public:
    LR_1();
    ~LR_1();
    void add_grammer(vector<string> &v0);
    void show_grammer();
    void show_dfa();
    bool LR_1_check(string &str);
    bool LR_1_check(queue<string> &token_name, queue<string> &token_value);
    void DFS(tree_node *root);
};

#endif