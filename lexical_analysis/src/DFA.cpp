#include "DFA.h"
#define eps "\u03b5"

set<int> DFA::find_closures(int id)
{
    set<int> closure;
    closure.insert(id);
    queue<int> q;
    q.push(id);
    while (!q.empty())
    {
        for (auto &n : nfa_graph[q.front()].next)
        {
            if (!closure.count(n->id) && n->path == eps)
            {
                q.push(n->id);
                closure.insert(n->id);
            }
        }
        q.pop();
    }
    return closure;
}

void DFA::nfa_2_dfa()
{

    set<int> x = find_closures(start_id);

    head_node h;
    h.id = dfa_graph.size();
    start_id = h.id;
    h.include = x;

    // start=s;

    queue<int> temp_queue;
    temp_queue.push(h.id);

    dfa_graph.push_back(h);

    while (!temp_queue.empty())
    {
        int current_id = temp_queue.front();
        temp_queue.pop();
        for (char ch : letter)
        {
            string c = "";
            c += ch;
            set<int> temp_set;
            for (int id : dfa_graph[current_id].include)
            {
                for (auto &n : nfa_graph[id].next)
                {
                    if (n->path == c)
                    {
                        set<int> s = find_closures(n->id);
                        temp_set.insert(s.begin(), s.end());
                    }
                }
            }
            if (temp_set.empty())
                continue;
            bool flag = false;
            for (auto &h : dfa_graph)
            {
                if (h.include == temp_set)
                {
                    flag = true;
                    edge *n = new edge(h.id, c);
                    dfa_graph[current_id].next.push_back(n);
                    break;
                }
            }
            if (!flag)
            {
                head_node h;
                h.id = dfa_graph.size();
                h.include = temp_set;
                dfa_graph.push_back(h);
                edge *n = new edge(h.id, c);
                dfa_graph[current_id].next.push_back(n);
                temp_queue.push(h.id);
            }
        }
    }
    for (auto &he : dfa_graph)
    {
        for (auto &n : he.include)
        {
            if (nfa_graph[n].is_final)
            {
                he.is_final = true;
                break;
            }
        }
    }
}

DFA::DFA(string pe) : NFA(pe)
{
    dfa_state_num = 0;
    nfa_2_dfa();
    min_dfa();
}
DFA::DFA(string pe, string name) : NFA(pe),token_name(name)
{
    dfa_state_num = 0;
    nfa_2_dfa();
    min_dfa();
}
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
    cout << endl;

    for (auto &i : min_dfa_graph)
    {
        for (edge *j : i.next)
        {
            cout << i.id << " -> " << j->id << " [label=" << j->path << "];" << endl;
            dfa_file << i.id << " -> " << j->id << " [label=" << j->path << "];" << endl;
        }
        if (i.is_final)
            dfa_file << i.id << " [shape=doublecircle];" << endl;
    }
    dfa_file << "}" << endl;
    // system(("dot -Tpdf "+file_name+" -o NFA.pdf").c_str());
    // cout << "begin:" << start.id << endl;
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
string DFA::get_token_name()
{
    return token_name;
}
void DFA::min_dfa()
{
    unordered_map<string, int> l;
    int k = 1;
    for (char c : letter)
    {
        string ch = "";
        ch += c;
        l[ch] = k;
        k++;
    }
    vector<vector<int>> mat(letter.size() + 1, vector<int>(dfa_graph.size(), -1));
    for (int i = 0; i < dfa_graph.size(); i++)
    {
        mat[0][i] = dfa_graph[i].is_final;
    }
    vector<int> temp_vector;
    while (temp_vector != mat[0])
    {
        temp_vector.clear();
        for (auto &i : dfa_graph)
        {
            for (auto &j : i.next)
            {
                mat[l[j->path]][i.id] = mat[0][j->id];
            }
        }
        int adj = 0;
        unordered_map<vector<int>, int, VectorHash> map;
        for (int i = 0; i < dfa_graph.size(); i++)
        { // 求下一个数组
            vector<int> te;
            for (int j = 0; j < mat.size(); j++)
            {
                te.push_back(mat[j][i]);
            }
            if (map.find(te) == map.end())
            { // 不存在
                map[te] = adj;
                adj++;
            }
            temp_vector.push_back(map[te]);
        }
        mat[0].swap(temp_vector);
    }
    for (int i = 0; i < dfa_graph.size(); i++)
    {
        head_node head;
        head.id = mat[0][i];
        head.is_final = dfa_graph[i].is_final;

        for (char c : letter)
        {
            string ch = "";
            ch += c;
            if (mat[l[ch]][i] != -1)
            {
                edge *n = new edge(mat[l[ch]][i], ch);
                head.next.push_back(n);
            }
        }
        bool flag = false;
        for (auto &h : min_dfa_graph)
        {
            if (h.id == head.id)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            min_dfa_graph.push_back(head);
        }
    }
}
bool DFA::check(string str)
{
    int now = start_id;
    for (char c : str)
    {
        string ch="";
        ch+=c;
        bool flag = false;
        for (auto &i : min_dfa_graph[now].next)
        {
            if (i->path == ch)
            {
                now = i->id;
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            return false;
        }
    }
    return min_dfa_graph[now].is_final;
}