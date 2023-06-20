#include "NFA.h"
#define eps "\u03b5"

void NFA::letter_set()
{
    for (char c : post_exp)
    {
        if (c == '`' || c == '*' || c == '|')
            continue;
        letter.insert(c);
    }
}
void NFA::re_2_nfa()
{
    stack<pair<int, int>> temp_stack;
    for (char c : post_exp)
    {
        switch (c)
        {
        case '*':
        { /* 连接符号 */

            // 取出栈顶两个元素

            // 创建新状态以及对应的边
            /*
             *  s1--(a)-->s2 与 s3--(b)-->s4进行连接
             *  s_new_1--(eps)-->s1--(a)-->s2--(eps)-->s3--(b)-->s4--(eps)-->s_new_2
             */

            pair<int, int> pair_2 = temp_stack.top();
            temp_stack.pop();
            pair<int, int> pair_1 = temp_stack.top();
            temp_stack.pop();

            head_node begin_edge;
            head_node end_edge;

            begin_edge.id = state_num++;
            end_edge.id = state_num++;
            start_id = begin_edge.id;

            end_edge.is_final = true;
            nfa_graph[pair_1.second].is_final = false;
            nfa_graph[pair_2.second].is_final = false;

            edge *edge_1 = new edge(pair_1.first, eps);
            edge *edge_2 = new edge(pair_2.first, eps);
            edge *edge_3 = new edge(end_edge.id, eps);

            begin_edge.next.push_back(edge_1);
            nfa_graph[pair_1.second].next.push_back(edge_2);
            nfa_graph[pair_2.second].next.push_back(edge_3);

            nfa_graph.push_back(begin_edge);
            nfa_graph.push_back(end_edge);

            temp_stack.push(make_pair(begin_edge.id, end_edge.id));

            break;
        }
        case '|':
        { /* 选择符号 */

            // 取出栈顶两个元素

            // 创建新状态以及对应的边
            /*
             *  s1--(a)-->s2 与 s3--(b)-->s4进行选择
             *  s_new_1--(eps)-->s1--(a)-->s2--(eps)-->s_new_2
             *         --(eps)-->s3--(b)-->s4--(eps)-->
             */

            pair<int, int> pair_1 = temp_stack.top();
            temp_stack.pop();
            pair<int, int> pair_2 = temp_stack.top();
            temp_stack.pop();

            head_node begin_edge;
            head_node end_edge;

            begin_edge.id = state_num++;
            end_edge.id = state_num++;
            start_id = begin_edge.id;

            end_edge.is_final = true;
            nfa_graph[pair_1.second].is_final = false;
            nfa_graph[pair_2.second].is_final = false;

            edge *edge_1 = new edge(pair_1.first, eps);
            edge *edge_2 = new edge(pair_2.first, eps);
            edge *edge_3 = new edge(end_edge.id, eps);
            edge *edge_4 = new edge(end_edge.id, eps);

            begin_edge.next.push_back(edge_1);
            begin_edge.next.push_back(edge_2);
            nfa_graph[pair_1.second].next.push_back(edge_3);
            nfa_graph[pair_2.second].next.push_back(edge_4);

            nfa_graph.push_back(begin_edge);
            nfa_graph.push_back(end_edge);
            temp_stack.push(make_pair(begin_edge.id, end_edge.id));

            break;
        }
        case '`':
        {
            // 取出栈顶元素


            // 创建新状态以及对应的边
            /*
             *
             *
             */

            pair<int, int> pair_1 = temp_stack.top();
            temp_stack.pop();

            head_node begin_edge;
            head_node end_edge;

            begin_edge.id = state_num++;
            end_edge.id = state_num++;
            start_id = begin_edge.id;

            end_edge.is_final = true;
            nfa_graph[pair_1.second].is_final = false;

            edge *edge_1 = new edge(pair_1.first, eps);
            edge *edge_2 = new edge(end_edge.id, eps);
            edge *edge_3 = new edge(pair_1.first, eps);
            edge *edge_4 = new edge(end_edge.id, eps);

            begin_edge.next.push_back(edge_1);
            begin_edge.next.push_back(edge_2);
            nfa_graph[pair_1.second].next.push_back(edge_3);
            nfa_graph[pair_1.second].next.push_back(edge_4);

            nfa_graph.push_back(begin_edge);
            nfa_graph.push_back(end_edge);
            temp_stack.push(make_pair(begin_edge.id, end_edge.id));

            break;
        }
        default:
        { /* 字母 */
            head_node begin;
            head_node end;
            begin.id = state_num++;
            end.id = state_num++;
            // begin.id=state_num++;
            // end.id=state_num++;
            end.is_final = true;
            edge *n = new edge();
            n->id = end.id;
            n->path = c;
            begin.next.push_back(n);
            nfa_graph.push_back(begin);
            nfa_graph.push_back(end);
            temp_stack.push(make_pair(begin.id, end.id));
            break;
        }
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

    cout << endl;

    for (head_node &i : nfa_graph)
    {
        for (edge *j : i.next)
        {
            cout << i.id << " -> " << j->id << " [label=" << j->path << "];" << endl;
            nfa_file << i.id << " -> " << j->id << " [label=" << j->path << "];" << endl;
        }
    }
    nfa_file << "}" << endl;
    // system(("dot -Tpdf "+file_name+" -o NFA.pdf").c_str());
    nfa_file.close();
}
