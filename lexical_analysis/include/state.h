#ifndef STATE_H
#define STATE_H
#include <iostream>
#include <string>
#include <set>
#include <stack>
#include <vector>
#include <queue>

using namespace std;

class state
{
public:
    /* 成员变量 */
    int id;             // 当前state的id值
    bool is_final;      // 标志是否为终态
    set<int> state_set; // 该状态包含的所有状态的id

    /* 构造函数 */
    state();
    state(int num);
    state(int num, bool is_final);
    state(int id, vector<state> &v);

    /* 成员函数 */
    void set_is_final(bool is_final); // 设置状态是否为终态
    bool equals(state s);             // 判断两个状态所包含的是否一样
    bool is_count(int state_id);      // 判断一个状态是否在该状态的set中
    void add_state(vector<state> &v); // 将参数中的状态添加进该状态
    void add_state(int v);
    void add_state(state s);
};

#endif