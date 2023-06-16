#include "LALR_1_DFA.h"
#define eps "\u03b5"

vector<LALR_1_grammer> shred_data::grammer = {};
set<string> shred_data::v_t = {};
set<string> shred_data::v_n = {};

unordered_map<string, set<string>> shred_data::first = {};
unordered_map<string, set<string>> shred_data::follow = {};

LALR_1_DFA::LALR_1_DFA() {}
int LALR_1_DFA::gram_count(LALR_1_grammer &g)
{
    for (auto &n : head_list.list)
    {
        if (n.is_in_grammer(g))
            return n.id;
    }
    return -1;
}
int LALR_1_DFA::gram_count(int from, LALR_1_grammer &g)
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

void LALR_1_DFA::init()
{
    /**
     * 根据文法初始化LALR_1_DFA
     * 1. 将文法的开始符号加入到开始状态中
     * 2. 添加产生式的闭包到状态
     * 3. 添加状态的转换
     * 4. 添加状态的接受状态
     *
     */
    node start(0);
    LALR_1_grammer gram = grammer[0];
    gram.dot = 0;
    gram.search.insert("$");
    start.push_back_gram(gram);
    head_list.add_node(start);
    queue<int> q;
    q.push(start.id);
    while (!q.empty())
    {
        node cur = head_list.list[q.front()];
        q.pop();
        int from = cur.id;

        unordered_map<string, node> temp_map;

        for (auto &g : cur.grammers)
        {
            if (g.dot == g.right.size() || g.right[g.dot] == eps)
                continue;
            // string f = find_follow(g);
            LALR_1_grammer temp_gram(g.left, g.right, g.dot + 1, g.search);
            temp_map[g.right[g.dot]].push_back_gram(temp_gram);
        }
        for (auto &n : temp_map)
        {
            bool flag = false;
            for (auto &p : head_list.list)
            {
                if (p == n.second)
                { // 存在核心项全相同的 node
                    head_list.list[from].path[n.first] = p.id;
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                n.second.id = head_list.size;
                head_list.add_node(n.second);
                q.push(n.second.id);
                head_list.list[from].path[n.first] = n.second.id;
            }
        }
    }
    for (auto &n : head_list.list)
    {
        for (int i = 0; i < n.grammers.size(); i++)
        {
            for (int j = i + 1; j < n.grammers.size(); j++)
            {
                if (n.grammers[i] == n.grammers[j])
                {
                    n.grammers[i].search.insert(n.grammers[j].search.begin(), n.grammers[j].search.end());
                    n.grammers.erase(n.grammers.begin() + j);
                }
            }
        }
        for (int i = 0; i < n.core.size(); i++)
        {
            for (int j = i + 1; j < n.core.size(); j++)
            {
                if (n.core[i] == n.core[j])
                {
                    n.core[i].search.insert(n.core[j].search.begin(), n.core[j].search.end());
                    n.core.erase(n.core.begin() + j);
                }
            }
        }
    }
    for (int i = 0; i < head_list.size; i++)
    {
        for (int j = i + 1; j < head_list.size; j++)
        {
            if (head_list.list[i].is_similar(head_list.list[j]))
            {
                for (auto &g : head_list.list[j].core)
                {
                    head_list.list[i].push_back_gram(g);
                }
                head_list.erase_node(j, i);
            }
        }
    }
}
void LALR_1_DFA::show()
{
    head_list.show();
}

node::node() {}
node::node(int i) : id(i) {}
bool node::is_similar(node &n)
{
    if (core.size() != n.core.size())
        return false;
    for (auto &g : core)
    {
        if (!n.is_in_core(g))
            return false;
    }
    return true;
}
bool node::is_in_core(LALR_1_grammer &g)
{
    for (auto &gram : core)
    {
        if (gram == g)
            return true;
    }
    return false;
}
bool node::is_in_grammer(LALR_1_grammer &g)
{
    for (auto &gram : grammers)
    {
        if (gram == g && gram.search == g.search)
            return true;
    }
    return false;
}
void node::push_back_gram(LALR_1_grammer &g)
{
    // if (is_in_core(g))
    // {
    //     for (auto &gram : core)
    //     {
    //         if (gram == g)
    //         {
    //             gram.search.insert(g.search.begin(), g.search.end());
    //             g.search.insert(gram.search.begin(), gram.search.end());
    //         }
    //     }
    // }
    // else
    core.push_back(g);
    queue<LALR_1_grammer> q;
    q.push(g);
    while (!q.empty())
    {
        LALR_1_grammer cur = q.front();
        q.pop();
        if (is_in_grammer(cur))
        {
            // for (auto &gram : grammers)
            // {
            //     if (gram == cur)
            //     {
            //         gram.search.insert(cur.search.begin(), cur.search.end());
            //         cur.search.insert(gram.search.begin(), gram.search.end());
            //     }
            // }
            continue;
        }
        grammers.push_back(cur);
        if (cur.right[0] == eps) // 当前产生式为 A -> eps
            continue;
        if (cur.dot == cur.right.size()) // 当前产生式为 A -> a·
            continue;
        vector<string> v;
        for (int i = cur.dot + 1; i < cur.right.size(); i++)
        {
            v.push_back(cur.right[i]);
        }
        for (auto &i : cur.search)
        {
            v.push_back(i);
        }
        set<string> s = find_first(v, cur.search);
        for (auto &g : grammer)
        {
            if (g.left == cur.right[cur.dot])
            { // 将闭包添加进队列
                LALR_1_grammer temp(g.left, g.right, 0, s);
                q.push(temp);
            }
        }
    }
}
set<string> node::find_first(vector<string> &v, set<string> &search)
{
    bool is_eps = true;
    set<string> s;
    for (int i = 0; i < v.size() - search.size(); i++)
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
        s.insert(search.begin(), search.end());
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
void adj_list::add_grammer(int i, LALR_1_grammer &g)
{
    list[i].push_back_gram(g);
}
void adj_list::erase_node(int id, int now_id)
{
    list.erase(list.begin() + id);
    size--;
    for (auto &n : list)
    {
        for (auto &p : n.path)
        {
            if (p.second == id)
            {
                p.second = now_id;
            }
            else if (p.second > id)
            {
                p.second--;
            }
        }
        if (n.id > id)
        {
            n.id--;
        }
    }
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