#include "DFA.h"

vector<state> DFA::find_closures(state &s)
{ // 从当前节点找闭包
    vector<state> closure;
    closure.push_back(s);
    queue<state> q;
    q.push(s);
    while (!q.empty())
    {
        for (edge &i : edge_vector)
        {
            if (i.from.id == q.front().id && i.info == '#')
            {
                bool flag = false;
                for (state &j : closure)
                {// 防止在nfa中形成环
                    if (j.id == i.to.id)
                        flag = true;
                }
                if (!flag)
                {
                    q.push(i.to);
                    closure.push_back(i.to);
                }
            }
        }
        q.pop();
    }
    return closure;
}
int DFA::is_count(state s)
{ // 判断当前状态是否存在
    for (state i : dfa_state)
    {
        if (i.equals(s))
            return i.id;
    }
    return -1;
}
int DFA::is_count_min(state s)
{ // 判断当前状态是否存在
    for (state i : min_dfa_state)
    {
        if (i.id == s.id)
            return i.id;
    }
    return -1;
}
state DFA::count_id(state s)
{ // 判断当前状态是否存在
    for (state i : dfa_state)
    {
        if (i.equals(s))
            return i;
    }
    state a(-1);
    return a;
}
void DFA::nfa_2_dfa()
{
    vector<state> v = find_closures(start);
    state s(dfa_state_num++, v);
    dfa_start = s;
    // start=s;
    temp.push(s);
    dfa_state.push_back(s);

    while (!temp.empty())
    {
        for (char c : letter)
        {                                      // 字符集中每个字符的路径
            state temp_state(dfa_state_num++); // 新建一个状态
            for (edge i : edge_vector)
            { // 遍历所有边
                if (i.info == c && temp.front().is_count(i.from.id))
                { // 边上有当前字符并且来自队列头的闭包
                    v = find_closures(i.to);
                    temp_state.add_state(v);
                }
            }
            if (temp_state.state_set.empty())
            {
                dfa_state_num--;
                continue; // 没找到到达闭包
            }
            if (is_count(temp_state) < 0)
            { // 如果当前状态不存在则入队列，并记录
                dfa_state.push_back(temp_state);
                temp.push(temp_state);
            }
            else
            { // 存在计数减1，找到已存在的序号
                dfa_state_num--;
                temp_state = count_id(temp_state);
            }
            edge e(temp.front(), temp_state, c); // 新建边
            dfa_edge.push_back(e);
        }
        temp.pop();
    }
    for (state &i : dfa_state)
    {
        if (i.is_count(end.id))
            i.set_is_final(true);
    }
}

DFA::DFA(string pe) : NFA(pe)
{
    dfa_state_num = 0;
    nfa_2_dfa();
    dfa_state_num = dfa_state.size();
    min_dfa();
}
void DFA::show_closures()
{
    vector<state> v;
    v = find_closures(start);
    for (state i : v)
    {
        cout << i.id << endl;
    }
}
// void DFA::show_dfa()
// {
//     for (state &i : min_dfa_state)
//     {
//         cout << i.id << " ";
//     }
//     cout << endl;
//     for (edge &i : min_dfa_edge)
//     {
//         /* F -> G [label=m]; */
//         // cout << i.from.id << "--" << i.info << "-->" << i.to.id << endl;
//         char c = i.info;
//         if (c == '#')
//             c = '$';
//         cout << i.from.id << " -> " << i.to.id << " [label=" << c << "];" << endl;
//     }
//     cout << "begin:" << dfa_start.id << endl;
//     for (state &i : min_dfa_state)
//     {
//         if (i.is_final)
//             cout << "end:" << i.id << endl;
//     }
// }
void DFA::show_dfa()
{
    string file_name = "output/DFA.dot";
    remove("output/DFA.dot");
    dfa_file.open(file_name, ios::out);
    dfa_file.clear();
    dfa_file << "digraph{" << endl
             << "rankdir = LR;" << endl
             << "label = \"DFA:" + in_exp + "\";" << endl;
    ;
    // for (state &i : min_dfa_state)
    // {
    //     cout << i.id << " ";
    // }
    cout << endl;
    for (edge &i : min_dfa_edge)
    {
        // cout << i.from.id << "--" << i.info << "-->" << i.to.id << endl;
        char c = i.info;
        if (c == '#')
            c = '0';

        // cout << i.from.id << " -> " << i.to.id << " [label=" << c << "];" << endl;
        dfa_file << i.from.id << " -> " << i.to.id << " [label=" << c << "];" << endl;

        if (i.to.is_final)
            dfa_file << i.to.id << " [shape=doublecircle];" << endl;
    }
    dfa_file << "}" << endl;
    // system(("dot -Tpdf "+file_name+" -o NFA.pdf").c_str());
    // cout << "begin:" << start.id << endl;
    for (state &i : min_dfa_state)
    {
        if (i.is_final)
            break;
        // cout << "end:" << i.id << endl;
    }
    cout << "Completed!" << endl;
    dfa_file.close();
}
struct VectorHash
{
    size_t operator()(const std::vector<int> &v) const
    {
        size_t seed = 0;
        for (int i : v)
        {
            seed ^= std::hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
void DFA::min_dfa()
{
    vector<vector<int>> matrix(letter.size() + 1, vector<int>(dfa_state_num, -1)); // 初始化row*column二维动态数组，初始化值为0
    for (state i : dfa_state)
    { // 初始化矩阵
        if (i.is_final)
            matrix[0][i.id] = 1;
        else
            matrix[0][i.id] = 0;
    }
    vector<int> temp(dfa_state_num);
    while (matrix[0] != temp)
    {
        temp.clear();
        for (edge e : dfa_edge)
        { // 根据边的info、from、to填写二维数组
            matrix[distance(letter.begin(), letter.find(e.info)) + 1][e.from.id] = matrix[0][e.to.id];
        }
        int adj = 0;
        unordered_map<vector<int>, int, VectorHash> map;
        for (int i = 0; i < dfa_state_num; i++)
        { // 求下一个数组
            vector<int> te;
            for (int j = 0; j < matrix.size(); j++)
            {
                te.push_back(matrix[j][i]);
            }
            if (map.find(te) == map.end())
            { // 不存在
                map[te] = adj;
                adj++;
            }
            temp.push_back(map[te]);
        }
        matrix[0].swap(temp);
    }
    vector<bool> vb(matrix[0].size(), false);
    for (int i : matrix[0])
    {
        vb[i] = true;
    }
    for (int i = 0; i < matrix[0].size(); i++)
    {
        if (vb[i])
        {
            state s(i);
            min_dfa_state.push_back(s);
        }
    }
    for (state i : dfa_state)
    {
        if (i.is_final)
            min_dfa_state[matrix[0][i.id]].set_is_final(true);
    }
    vector<char> le;
    for (char c : letter)
        le.push_back(c);
    for (int i = 1; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            // 使用迭代器遍历 set 中的元素
            set<char>::iterator it = letter.begin();
            for (int m = 0; m < i - 2 && it != letter.end(); ++m, ++it)
                ;
            char c = *it;
            if (matrix[i][j] != -1)
            {
                edge ed(min_dfa_state.at(matrix[0][j]), min_dfa_state.at(matrix[i][j]), le[i - 1]);
                bool f = false;
                for (edge E : min_dfa_edge)
                {
                    if (E == ed)
                    {
                        f = true;
                        break;
                    }
                }
                if (!f)
                    min_dfa_edge.push_back(ed);
            }
        }
    }
}
bool DFA::check(string str)
{
    state &s=dfa_start;
    bool flag=false;
    for(char c:str)
    {
        for(edge &e:min_dfa_edge)
        {
            if(e.from.id==s.id && e.info==c)
            {
                flag=true;
                s=e.to;
                break;
            }
        }
        if(!flag) return flag;
        flag=false;
    }
    //if(!flag)return flag;
    return s.is_final;
}