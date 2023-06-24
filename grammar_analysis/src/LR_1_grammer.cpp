#include "LR_1_grammer.h"

LR_1_grammer::LR_1_grammer(string l, vector<string> r) : left(l), right(r), dot(0) {}
LR_1_grammer::LR_1_grammer(string l, vector<string> r, int d, string f) : left(l), right(r), dot(d),search(f) {}
LR_1_grammer::LR_1_grammer() {}
LR_1_grammer::~LR_1_grammer() {}

void LR_1_grammer::set_grammer(string l, vector<string> r)
{
    left = l;
    right = r;
}
void LR_1_grammer::set_left(string l)
{
    left = l;
}
void LR_1_grammer::set_right(vector<string> r)
{
    right = r;
}
void LR_1_grammer::set_search(string f)
{
   search = f;
}
string LR_1_grammer::to_string()
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
    if (search != "")
        s += ", " + search + " ";
    return s;
}