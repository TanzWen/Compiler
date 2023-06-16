#include "NFA.h"

void NFA::letter_set()
{
    for (char c : post_exp)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            letter.insert(c);
    }
}
void NFA::re_2_nfa()
{
    for (char c : post_exp)
    {
        switch (c)
        {
        case '*':
        { /* 连接符号 */

            // 取出栈顶两个元素
            pair<state, state> pair1 = s.top();
            s.pop();
            pair<state, state> pair2 = s.top();
            s.pop();
            for (state &i : state_vector)
            {
                if (i.id == pair1.second.id || i.id == pair2.second.id)
                {
                    i.set_is_final(false);
                }
            }

            // 创建新状态以及对应的边
            /*
             *  s1--(a)-->s2 与 s3--(b)-->s4进行连接
             *  s_new_1--(eps)-->s1--(a)-->s2--(eps)-->s3--(b)-->s4--(eps)-->s_new_2
             */
            state b(state_num++);
            state e(state_num++, true);
            start = b;
            end = e;
            state_vector.push_back(b);
            state_vector.push_back(e);
            s.push(make_pair(b, e));
            edge e1(b, pair2.first, '#');
            edge e2(pair2.second, pair1.first, '#');
            edge e3(pair1.second, e, '#');

            edge_vector.push_back(e1);
            edge_vector.push_back(e2);
            edge_vector.push_back(e3);
            break;
        }
        case '|':
        { /* 选择符号 */

            // 取出栈顶两个元素
            pair<state, state> pair1 = s.top();
            s.pop();
            pair<state, state> pair2 = s.top();
            s.pop();

            for (state &i : state_vector)
            {
                if (i.id == pair1.second.id || i.id == pair2.second.id)
                {
                    i.set_is_final(false);
                }
            }

            // 创建新状态以及对应的边
            /*
             *  s1--(a)-->s2 与 s3--(b)-->s4进行选择
             *  s_new_1--(eps)-->s1--(a)-->s2--(eps)-->s_new_2
             *         --(eps)-->s3--(b)-->s4--(eps)-->
             */
            state b(state_num++);
            state e(state_num++, true);
            start = b;
            end = e;
            state_vector.push_back(b);
            state_vector.push_back(e);
            s.push(make_pair(b, e));

            edge e1(b, pair1.first, '#');
            edge e2(b, pair2.first, '#');
            edge e3(pair1.second, e, '#');
            edge e4(pair2.second, e, '#');
            edge_vector.push_back(e1);
            edge_vector.push_back(e2);
            edge_vector.push_back(e3);
            edge_vector.push_back(e4);
            break;
        }
        case '`':
        {
            // 取出栈顶元素
            pair<state, state> pair1 = s.top();
            s.pop();

            // 创建新状态以及对应的边
            /*
             *
             *
             */
            state b(state_num++);
            state e(state_num++, true);
            start = b;
            end = e;
            state_vector.push_back(b);
            state_vector.push_back(e);
            s.push(make_pair(b, e));

            edge e1(b, e, '#');
            edge e2(b, pair1.first, '#');
            edge e3(pair1.second, e, '#');
            edge e4(pair1.second, pair1.first, '#');
            edge_vector.push_back(e1);
            edge_vector.push_back(e2);
            edge_vector.push_back(e3);
            edge_vector.push_back(e4);
            break;
        }
        default:
        { /* 字母 */
            state begin(state_num++);
            state end(state_num++, true);
            edge e(begin, end, c);
            state_vector.push_back(begin);
            state_vector.push_back(end);
            edge_vector.push_back(e);
            s.push(make_pair(begin, end));
            break;
        }
        }
    }
    for (state &i : state_vector)
    {
        if (i.id != end.id)
        {
            i.set_is_final(false);
        }
    }
    for (edge &i : edge_vector)
    {
        if (i.from.id != end.id)
        {
            i.from.set_is_final(false);
        }
        if (i.to.id != end.id)
        {
            i.to.set_is_final(false);
        }
    }
}

NFA::NFA() {}
NFA::NFA(string pe)
{
    str s(pe);
    in_exp = pe;
    post_exp = s.get_post_exp();
    state_num = 0;
    letter_set();
    re_2_nfa();
}
void NFA::show_nfa()
{
    string file_name = "output/NFA.dot";
    remove("output/NFA.dot");
    nfa_file.open("output/NFA.dot", ios::out);
    nfa_file.clear();
    nfa_file << "digraph{" << endl
             << "rankdir = LR;" << endl
             << "label = \"NFA:" + in_exp + "\";" << endl;
    // for (state &i : state_vector)
    // {
    //     cout << i.id << " ";
    // }
    cout << endl;
    for (edge &i : edge_vector)
    {
        // cout << i.from.id << "--" << i.info << "-->" << i.to.id << endl;
        char c = i.info;
        if (c == '#')
            c = '0';
        cout << i.from.id << " -> " << i.to.id << " [label=" << c << "];" << endl;
        nfa_file << i.from.id << " -> " << i.to.id << " [label=" << c << "];" << endl;
    }
    nfa_file << "}" << endl;
    // system(("dot -Tpdf "+file_name+" -o NFA.pdf").c_str());
    cout << "begin:" << start.id << endl;
    cout << "end:" << end.id << endl;
    nfa_file.close();
}
