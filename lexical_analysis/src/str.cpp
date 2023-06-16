#include "str.h"

/**运算符优先级**/
int str::priority(char c)
{
    if (c == '`')
        return 3;
    else if (c == '*')
        return 2;
    else if (c == '|')
        return 1;
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 0; // 字母为0
    else
        return -1;
}

/**将中缀表达式标准化**/
string str::std_in_exp(string s)
{
    string result = "";
    result += s[0];
    for (int i = 1; i < s.length(); i++)
    { // 从第二个字符开始遍历
        char c = s[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '(')
        { // 当前字符为字母或者左括号
            if ((s[i - 1] >= 'a' && s[i - 1] <= 'z') || (s[i - 1] >= 'A' && s[i - 1] <= 'Z') || s[i - 1] == '`' || s[i - 1] == ')')
            { // 当前字符前一个字母为1.字母2.重复运算3.右括号时添加乘号
                result += '*';
                result += c;
            }
            else
            {
                result += c;
            }
        }
        else
            result += c;
    }
    return result;
}

/**中缀->后缀**/
string str::in_to_post()
{ // 利用调度场算法将输入的中缀表达式转为后缀表达式
    string result = "";
    stack<char> operate_stack;
    for (char c : in_exp)
    { // 遍历字符串
        if (priority(c) > 0)
        { // 字符为运算符
            if (operate_stack.empty() || priority(operate_stack.top()) < priority(c))
                operate_stack.push(c); // 直接入栈：1.空栈 2.栈顶优先级小于当前运算符
            else
            { // 栈顶优先级不小于当前运算符
                while (!operate_stack.empty() && priority(operate_stack.top()) >= priority(c))
                { // 一直出栈直到栈顶优先级小于当前运算符
                    result += operate_stack.top();
                    operate_stack.pop();
                }
                operate_stack.push(c);
            }
        }
        else if (priority(c) == 0)
            result += c; // 字符直接输出
        else
        { // 字符为 ‘（’ 和 ‘）’
            if (c == '(')
            {
                operate_stack.push(c);
            }
            else if (c == ')')
            {
                while (!operate_stack.empty() && operate_stack.top() != '(')
                {
                    result += operate_stack.top();
                    operate_stack.pop();
                }
                operate_stack.pop();
            }
        }
    }
    while (!operate_stack.empty())
    {
        result += operate_stack.top();
        operate_stack.pop();
    }
    return result;
}

/**构造函数**/
str::str() {}
str::str(string s)
{
    in_exp = std_in_exp(s);
    // in_exp=s;
    post_exp = in_to_post();
}

// 成员函数

/**gets**/
string str::get_in_exp()
{
    return in_exp;
}
string str::get_post_exp()
{
    return post_exp;
}
