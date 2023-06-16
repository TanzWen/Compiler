#include "LALR_1_grammer.h"

LALR_1_grammer::LALR_1_grammer(string l, vector<string> r) : left(l), right(r), dot(0) {}
LALR_1_grammer::LALR_1_grammer(string l, vector<string> r, int d, set<string> &f) : left(l), right(r), dot(d), search(f) {}
LALR_1_grammer::LALR_1_grammer() {}
LALR_1_grammer::~LALR_1_grammer() {}

void LALR_1_grammer::set_grammer(string l, vector<string> r)
{
    left = l;
    right = r;
}
void LALR_1_grammer::set_left(string l)
{
    left = l;
}
void LALR_1_grammer::set_right(vector<string> r)
{
    right = r;
}
void LALR_1_grammer::insert_search(string f)
{
    search.insert(f);
}
void LALR_1_grammer::insert_search(set<string> f)
{
    search.insert(f.begin(), f.end());
}

string LALR_1_grammer::to_string()
{
    string s = left + " -> ";
    for (int i = 0; i < right.size(); i++)
    {
        if (i == dot)
            s += "·";
        if (i == right.size() - 1 && dot == right.size())
            s += right[i] + "·";
        else
            s += right[i] + " ";
    }
    if (!search.empty())
    {
        s += ", ";
        for(auto &i : search)
            s += i + " ";
    }
    return s;
}