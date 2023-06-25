#include "LR_1.h"
#define eps "\u03b5"

table_item::table_item() : action(""), next(-1) {}
table_item::table_item(int n) : action(""), next(n) {}
table_item::table_item(string a, int n) : action(a), next(n) {}
table_item::table_item(string a, LR_1_grammer &g) : action(a), next(-1), gram(g) {}
table_item::~table_item() {}
string table_item::to_string()
{
    string s = action + " : ";
    if (action == "shift" || action == "goto")
    {
        s += std::to_string(next);
    }
    else if (action == "reduce")
    {
        s += gram.to_string();
    }
    else if (action == "acc")
    {
        s += "acc";
    }
    return s;
}

LR_1::LR_1()
{
    out.open("./output/grammer_tree.dot", ios::out);
    out2.open("./output/analysis_process.txt", ios::out);
}
LR_1::~LR_1()
{
    out.close();
    out2.close();
}
void LR_1::add_grammer(vector<string> &v0)
{
    /* 分离文法 */
    for (string str : v0)
    {
        stringstream ss(str);
        string left;
        string right;

        ss >> left;  // 将文法左边放入left中
        ss >> right; // 不需要 "->"
        if (grammer.empty())
        { // 拓广文法
            start = left;
            LR_1_grammer temp(start + '\'', {start});
            grammer.push_back(temp);
        }

        vector<string> v; // 存放右边文法
        while (ss >> right)
        {
            v.push_back(right);
            if (right == "|" || !ss.good())
            { // 遇到 "|" or 该行结束
                if (right == "|")
                    v.pop_back();
                LR_1_grammer temp(left, v);
                temp.gram_size = temp.right.size();
                grammer.push_back(temp);
                v.clear();
            }
        }
    }

    /* 分离出终结符和非终结符 */
    for (auto &i : grammer)
    {
        v_n.insert(i.left);
    }
    for (auto &i : grammer)
    {
        for (string &j : i.right)
        {
            if (!v_n.count(j) && j != eps)
                v_t.insert(j);
        }
    }

    /* 分别求出 FIRST 和 FOLLOW 集合 */

    // 初始化 FIRST 和 FOLLOW 集合，终结符 first 集合为本身，起始文法 follow 添加 $ ,其余为空
    for (string str : v_t)
    {
        set<string> s;
        s.insert(str);
        first[str] = s;
    }

    follow[start].insert("$");

    // 求 first 集合
    unordered_map<string, set<string>> first_temp;
    while (first != first_temp)
    {
        first_temp = first;
        for (auto &gram : grammer)
        {
            /**
             * is_eps 的作用：用于标志该文法之后的文法是否全有epsilon
             * 之后文法其中一个没有 epsilon 都会置于 false
             * 如果有则继续下一个文法，到了最后一个还有的话将 epsilon 加入 first 中
             */
            bool is_eps = false;
            for (string str : gram.right)
            { // 产生式右边的每一条文法
                if (str == eps)
                { // 产生式右边为 epsilon
                    first[gram.left].insert(eps);
                    break;
                }
                first[gram.left].insert(first[str].begin(), first[str].end()); // 将 first 添加进 follow
                first[gram.left].erase(eps);
                if (!first[str].count(eps))
                { // 当前不存在 eps
                    break;
                }
                if (str == gram.right.back())
                { // 当前符号为产生式右边最后一个
                    is_eps = true;
                }
            }
            if (is_eps)
                first[gram.left].insert(eps);
        }
    }

    // 求 follow 集合
    unordered_map<string, set<string>> follow_temp;
    while (follow != follow_temp)
    { // 对所有 follow 集合进行更新，直到不再变化
        follow_temp = follow;
        for (auto &gram : grammer)
        { // 遍历每一条文法规则
            if (gram.right.front() == eps)
                continue;

            // i 指向需要求 follow 集合的字符串
            // j 指向 i 之后的字符串
            int i, j;

            for (i = 0; i < gram.right.size() - 1; i++)
            { // i 指向需要求 follow 集合的字符串
                if (v_t.count(gram.right[i]))
                    continue;
                bool is_eps = true;
                for (j = i + 1; j < gram.right.size(); j++)
                {                                                                                           // j 指向 i 之后的字符串
                    follow[gram.right[i]].insert(first[gram.right[j]].begin(), first[gram.right[j]].end()); // 将 j 的 first 集合添加进 i 的 follow 集合中
                    follow[gram.right[i]].erase(eps);
                    if (!first[gram.right[j]].count(eps))
                    {
                        is_eps = false;
                        break;
                    }
                }
                if (is_eps && j == gram.right.size())
                    follow[gram.right[i]].insert(follow[gram.left].begin(), follow[gram.left].end());
            }
            if (!v_t.count(gram.right[i]))
                follow[gram.right[i]].insert(follow[gram.left].begin(), follow[gram.left].end());
        }
    }

    /* 根据文法计算出 LR_1_DFA */
    // 初始化 LR_1_DFA
    shred_data::grammer = grammer;
    shred_data::v_t = v_t; // 终结符集合
    shred_data::v_n = v_n; // 非终结符集合
    shred_data::first = first;
    shred_data::follow = follow;

    LR_1_DFA dfa;
    dfa.init();
    // dfa.show();

    /* 根据 LR_1_DFA 构建 LR(0) 分析表 */
    for (auto &n : dfa.head_list.list)
    {
        for (auto &g : n.grammers)
        {
            if (g.right.front() == eps)
            { // 产生式右边为 epsilon 填入 follow 集合中
                for (string str : follow[g.left])
                {
                    table_item item("reduce", g);
                    LR_1_prasing_table[n.id][str] = item;
                }
            }
            if (g.dot == g.right.size())
            { // 规约项
                if (g.left == start + "\'")
                {
                    table_item item("acc", -1);
                    LR_1_prasing_table[n.id]["$"] = item;
                }
                else
                {
                    table_item item("reduce", g);
                    LR_1_prasing_table[n.id][g.search] = item;
                }
            }
        }
        for (auto &i : n.path)
        { // 路径
            if (v_t.count(i.first))
            { // 是终结符
                table_item item("shift", i.second);
                LR_1_prasing_table[n.id][i.first] = item;
            }
            else if (v_n.count(i.first))
            { // 是非终结符
                table_item item("goto", i.second);
                LR_1_prasing_table[n.id][i.first] = item;
            }
        }
    }
}
void LR_1::show_dfa()
{
    dfa.show();
}
void LR_1::show_grammer()
{
    /** 输出该文法规则的相关信息
     *
     * 1. 输出文法改写后的所有产生式
     * 2. 输出终结符和非终结符
     * 3. 输出 first 集合
     * 4. 输出 follow 集合
     * 5. 输出 LR(1) 分析表
     * 6. 输出 LR(1) 分析过程
     */
    cout << "grammer:" << endl;
    for (auto &i : grammer)
    {
        cout << i.to_string() << endl;
    }
    cout << endl
         << "terminal:" << endl;
    for (string str : v_t)
    {
        cout << str << " ";
    }
    cout << endl
         << "non-terminal:" << endl;
    for (string str : v_n)
    {
        cout << str << " ";
    }
    cout << endl
         << endl;

    cout << "First:" << endl;
    for (auto &i : first)
    {
        if (v_t.count(i.first))
            continue;
        cout << i.first << " : { ";
        for (string j : i.second)
        {
            cout << j << " ";
        }
        cout << "}" << endl;
    }
    cout << endl
         << "Follow:" << endl;
    for (auto &i : follow)
    {
        cout << i.first << " : { ";
        for (string j : i.second)
        {
            cout << j << " ";
        }
        cout << "}" << endl;
    }
    cout << endl
         << "LR(1) Parsing Table:" << endl;
    for (int i = 0; i < LR_1_prasing_table.size(); i++)
    {
        cout << i << " : { ";
        for (auto &j : LR_1_prasing_table[i])
        {
            cout << j.first << " : " << j.second.to_string() << " ";
        }
        cout << "}" << endl;
    }
    // for (auto &i : LR_1_prasing_table)
    // {
    //     cout << i.first << " : { ";
    //     for (auto &j : i.second)
    //     {
    //         cout << j.first << " : " << j.second.action + to_string(j.second.next) << " ";
    //     }
    //     cout << "}" << endl;
    // }

    cout << endl;
}
void LR_1::DFS(tree_node *root)
{
    out << root->id << " [label=\"" << root->name << "\\n"
        << root->value << "\"];" << endl;
    if (root->children.empty())
        out << root->id << " [shape=box];" << endl;
    for (auto &i : root->children)
    {
        out << root->id << " -> " << i->id << endl;
        DFS(i);
    }
}
bool LR_1::LR_1_check(queue<string> &token_name, queue<string> &token_value)
{
    /** LR(1) 分析过程
     *
     * 1. 初始化分析栈
     * 2. 初始化输入串
     * 3. 初始化分析表
     * 4. 初始化分析过程
     * 5. 分析过程
     */
    stack<tree_node *> tree_stack;
    stack<string> stack;
    stack.push("$");
    stack.push("0");
    token_name.push("$");
    out2 << "stack: ";
    show_stack(stack);
    out2 << endl
         << "queue: ";
    show_queue(token_name);
    out2 << endl;
    while (true)
    {
        int node_id = stoi(stack.top());
        string input = token_name.front();
        table_item item = LR_1_prasing_table[node_id][input];
        out2 << "action: " << item.to_string() << endl
             << endl;

        if (item.action == "shift")
        {
            stack.push(input);
            stack.push(to_string(item.next));
            token_name.pop();
        }
        else if (item.action == "reduce")
        {
            int n;
            if (item.gram.right.front() == eps)
                n = 0;
            else
                n = item.gram.right.size() * 2;
            for (int i = 0; i < n; i++)
                stack.pop();
            node_id = stoi(stack.top());
            stack.push(item.gram.left);
            stack.push(to_string(LR_1_prasing_table[node_id][item.gram.left].next));

            tree_node *node = new tree_node(item.gram.left);
            node->children.resize(item.gram.right.size());
            for (int i = item.gram.right.size() - 1; i >= 0; i--)
            {
                if (v_t.count(item.gram.right[i]))
                {
                    // node->children[i] = new tree_node(item.gram.right[i]);
                    // node->children[i]->value = token_value.front();
                    // token_value.pop();

                    node->children[i] = new tree_node(item.gram.right[i]);
                    node->children[i]->value = item.gram.right[i];
                }
                else if (item.gram.right[i] == eps)
                {
                    node->children[i] = new tree_node(item.gram.right[i]);
                    node->children[i]->value = item.gram.right[i];
                }
                else
                {
                    node->children[i] = tree_stack.top();
                    tree_stack.pop();
                }
            }
            // if (node->is_leaf())
            //     node->value = node->name;
            // else
            // {
            //     for (auto &i : node->children)
            //         if (i->value != eps && i->value != "")
            //             node->value += " " + i->value;
            // }
            tree_stack.push(node);
        }
        else if (item.action == "acc")
        {
            cout << "acc" << endl;
            root = tree_stack.top();
            root->DFS(token_value);
            out << "digraph G {" << endl;
            DFS(root);
            out << "}" << endl;
            return true;
        }
        else
        {
            cout << "error" << endl;
            return false;
        }
        out2 << "stack: ";
        show_stack(stack);
        out2 << endl
             << "queue: ";
        show_queue(token_name);
        out2 << endl;
    }
}

/* 输出格式控制 */
void LR_1::show_stack(stack<string> &s)
{
    for (stack<string> temp = s; !temp.empty(); temp.pop())
        out2 << temp.top() << " ";
}
void LR_1::show_queue(queue<string> &q)
{
    for (queue<string> temp = q; !temp.empty(); temp.pop())
        out2 << temp.front() << " ";
}
int tree_node::count = 0;
void tree_node::DFS()
{
    if (is_leaf())
        cout << name << endl;
    for (auto &i : children)
    {
        i->DFS();
    }
}
void tree_node::DFS(queue<string> &token_value)
{
    if (is_leaf() && value != eps)
    {
        value = token_value.front();
        token_value.pop();
        if (name == "string")
        {
            value.replace(0, 1, "\\\"");
            value.replace(value.size() - 1, 1, "\\\"");
        }
    }
    for (auto &i : children)
    {
        i->DFS(token_value);
        if (i->value != eps && i->value != "")
            if (value == "")
                value += i->value;
            else
                value += " " + i->value;
    }
}