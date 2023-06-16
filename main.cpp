#include <iostream>
#include <string>

#include "LALR_1.h"
#include "DFA.h"  

using namespace std;

int main()
{
    DFA dfa("123`");
    cout<<dfa.check("12333")<<endl;
    LALR_1 lalr_1;
    lalr_1.show_grammer();
    return 0;
}