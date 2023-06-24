#include "LR_1_DFA.h"
#define eps "\u03b5"

vector<LR_1_grammer> shred_data::grammer = {};
set<string> shred_data::v_t = {};
set<string> shred_data::v_n = {};

unordered_map<string, set<string>> shred_data::first = {};
unordered_map<string, set<string>> shred_data::follow = {};

LR_1_DFA::LR_1_DFA() {}
int LR_1_DFA::gram_count(LR_1_grammer &g)
{
    for (auto &n : head_list.list)
    {
        if (n.is_in_grammer(g))
            return n.id;
    }
    return -1;
}
int LR_1_DFA::gram_count(int from, LR_1_grammer &g)
{
    // for (auto &n : head_list.list)
    // {
    //     if (n.is_in_core(g))
    //         return n.id;
    //     if (n.id == from && n.path.count(g.right[g.dot - 1]))
    //         return n.path[g.right[g.dot - 1]];
    // }
    node n = head_list.list[from];
    if (n.is_in_core(g))
        return n.id;
    if (n.id == from && n.path.count(g.right[g.dot - 1]))
        return n.path[g.right[g.dot - 1]];

    return -1;
}

void LR_1_DFA::init()
{
    /**
     * 根据文法初始化LR_1_DFA
     * 1. 将文法的开始符号加入到开始状态中
     * 2. 添加产生式的闭包到状态
     * 3. 添加状态的转换
     * 4. 添加状态的接受状态
     *
     */
    node start(0);
    LR_1_grammer gram = grammer[0];
    gram.dot = 0;
    gram.search = "$";
    start.push_back_gram(gram);
    head_list.add_node(start);
    queue<int> q;
    q.push(start.id);
    while (!q.empty())
    {
        node cur = head_list.list[q.front()];
        q.pop();
        int from = cur.id;

        unordered_map<string,node>temp_map;

        for (auto &g : cur.grammers)
        {
            if (g.dot == g.right.size() || g.right[g.dot] == eps)
                continue;
            // string f = find_follow(g);
            LR_1_grammer temp_gram(g.left, g.right, g.dot + 1, g.search);
            temp_map[g.right[g.dot]].push_back_gram(temp_gram);
        }
        for(auto &n:temp_map)
        {
            bool flag = false;
            for(auto &p:head_list.list)
            {
                if(p==n.second)
                {// 存在核心项全相同的 node
                    head_list.list[from].path[n.first] = p.id;
                    flag = true;
                    break;
                }
            }
            if(!flag)
            {
                n.second.id = head_list.size;
                head_list.add_node(n.second);
                q.push(n.second.id);
                head_list.list[from].path[n.first] = n.second.id;
            }
        }
        
    }
}
void LR_1_DFA::show()
{
    head_list.show();
}

node::node() {}
node::node(int i) : id(i) {}
bool node::is_in_core(LR_1_grammer &g)
{
    for (auto &gram : core)
    {
        if (gram == g)
            return true;
    }
    return false;
}
bool node::is_in_grammer(LR_1_grammer &g)
{
    for (auto &gram : grammers)
    {
        if (gram == g)
            return true;
    }
    return false;
}
void node::push_back_gram(LR_1_grammer &g)
{
    core.push_back(g);
    queue<LR_1_grammer> q;
    q.push(g);
    while (!q.empty())
    {
        LR_1_grammer cur = q.front();
        q.pop();
        if (is_in_grammer(cur))
            continue;
        grammers.push_back(cur);
        if (cur.right[0] == eps) // 当前产生式为 A -> eps
            continue;
        if (cur.dot == cur.right.size()) // 当前产生式为 A -> a·
            continue;
        // if (v_t.count(cur.right[cur.dot])) // 当前产生式为 A -> a·b dot 后为终结符
        //     continue;
        // LR_1_grammer temp(cur.left, cur.right, cur.dot + 1);
        // q.push(temp);
        vector<string> v;
        for (int i = cur.dot + 1; i < cur.right.size(); i++)
        {
            v.push_back(cur.right[i]);
        }
        v.push_back(cur.search);
        set<string> s = find_first(v);
        for (auto &g : grammer)
        {
            if (g.left == cur.right[cur.dot])
            { // 将闭包添加进队列
                for (auto &i : s)
                {
                    LR_1_grammer temp(g.left, g.right, 0, i);
                    q.push(temp);
                }
            }
        }
    }
}
set<string> node::find_first(vector<string> &v)
{
    bool is_eps = true;
    set<string> s;
    for (int i = 0; i < v.size() - 1; i++)
    {
        s.insert(first[v[i]].begin(), first[v[i]].end());
        s.erase(eps);
        if (first[v[i]].count(eps) == 0)
        {
            is_eps = false;
            break;
        }
    }
    if (is_eps)
    {
        s.insert(v.back());
    }
    return s;
}

adj_list::adj_list() : size(0) {}
void adj_list::add_node(node &n)
{
    n.id = size;
    list.push_back(n);
    size++;
}
void adj_list::add_grammer(int i, LR_1_grammer &g)
{
    list[i].push_back_gram(g);
}
void adj_list::show()
{
    for (auto &n : list)
    {
        cout << n.id << " : ";
        for (auto &g : n.grammers)
        {
            cout << g.to_string() << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (auto &n : list)
    {
        for (auto &j : n.path)
        {
            cout << "from : " << n.id << " to : " << j.second << " path : " << j.first << endl;
        }
    }
}