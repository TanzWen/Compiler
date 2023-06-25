#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "LR_1.h"
#include "DFA.h"

using namespace std;

int line_num = 0;
int token_num = 0;

int main()
{
    fstream input;
    input.open("./input/lexicon.txt", ios::in);
    string line;
    vector<DFA *> lex;
    DFA *op;
    DFA *non_digit;
    DFA *digit;
    while (getline(input, line))
    {
        line_num++;
        if (line == "")
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
    

    fstream file;
    vector<string> v;
    LR_1 lr_1;
    file.open("./input/grammer.txt", ios::in);
    // 默认文法规则第一行左边为进入式
    while (getline(file, line))
    {
        if (line.empty())
            break;
        v.push_back(line);
    }

    lr_1.add_grammer(v);
    // lr_1.show_grammer();

    fstream input2;
    fstream output;
    input2.open("./sample.tan", ios::in);
    string line2;
    output.open("./output/lex_result.txt", ios::out);
    queue<string> token_name;
    queue<string> token_value;
    int now_line = 0;
    while (getline(input2, line2))
    {
        now_line++;
        string str = "";
        for (int k = 0; k < line2.length(); k++)
        {

            string tmp = "";
            switch (line2[k])
            {
            case '(':
                tmp += "#";
                break;
            case ')':
                tmp += "$";
                break;
            case '*':
                tmp += "@";
                break;
            default:
                tmp += line2[k];
                break;
            }
            if (k+1<line2.size() && op->check(tmp) && op->check(tmp + line2.substr(k + 1, 1)))
            {
                str += " " + tmp + line2[k + 1] + " ";
                k++;
            }
            else if (op->check(tmp))
                str += " " + tmp + " ";
            else
                str += tmp;
        }
        stringstream ss(str);
        string tmp;
        while (ss >> tmp)
        {
            bool flag = false;
            for (auto &i : lex)
            {
                if (i->check(tmp))
                {
                    flag = true;
                    string tn=i->get_token_name();
                    // if(tmp=="==" || tmp=="!=" || tmp==">" || tmp=="<" || tmp==">=" || tmp=="<=")
                    // {
                    //     token_name.push("<Logical-Operator>");
                    //     token_value.push(tmp);
                    // }
                    // else if(tmp=="++"||tmp=="--")
                    // {
                    //     token_name.push("<Suffix-Operator>");
                    //     token_value.push(tmp);
                    // }
                    if(tmp=="@")
                    {
                        token_name.push("*");
                        token_value.push("*");  
                    }
                    else if(tmp=="#")
                    {
                        token_name.push("(");
                        token_value.push("(");  
                    }
                    else if(tmp=="$")
                    {
                        token_name.push(")");
                        token_value.push(")");  
                    }
                    else if(tn=="operator")
                    {
                        token_name.push(tmp);
                        token_value.push(tmp);  
                    }
                    else
                    {
                        token_name.push(i->get_token_name());
                        token_value.push(tmp);   
                    }
                    token_num++;
                    output << "<" << token_name.back() << " , " << token_value.back() << "> ";
                    break;
                }
            }
            if(!flag)
            {
                output << "<error , " << tmp << "> ";
                cout<<"Error: line "<<now_line<<" "<<tmp<<" is not a valid token"<<endl;
                return 0;
            }
        }
        output << endl;
    }
    lr_1.LR_1_check(token_name, token_value);
    return 0;
}