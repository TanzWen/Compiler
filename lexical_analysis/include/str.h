#ifndef STR_H
#define STR_H
#include <iostream>
#include <string>
#include <stack>
using namespace std;
class str
{
private:
    string in_exp;   // 中缀表达式
    string post_exp; // 后缀表达式

    /**运算符优先级**/
    int priority(char c);

    /**将中缀表达式标准化**/
    string std_in_exp(string s);

    /**中缀->后缀**/
    string in_to_post();

public:
    /**构造函数**/
    str();
    str(string s);

    // 成员函数

    /**gets**/
    string get_in_exp();
    string get_post_exp();
};

#endif