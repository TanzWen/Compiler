#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "LALR_1.h"
#include "DFA.h"

using namespace std;

int main()
{
    fstream input;
    input.open("./input/lexicon.txt", ios::in);
    string line;
    vector<DFA *> lex;
    DFA *op;
    while (getline(input, line))
    {
        if(line=="")
            continue;
        stringstream ss(line);
        string name;
        string exp;
        ss >> name >> exp;
        DFA *dfa = new DFA(exp, name);
        lex.push_back(dfa);
        if (name == "operator")
            op = dfa;
    }
    for(auto &i:lex)
    {
        cout<<i->get_token_name()<<endl;
    }

    return 0;
}