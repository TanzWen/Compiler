#ifndef LALR_1_GRAMMER
#define LALR_1_GRAMMER


# include <vector>
# include <string>
# include <functional>
# include <set>

using namespace std;

class LALR_1_grammer
{
public:
    string left;
    vector<string> right;
    set<string> search;
    
    int dot;

    LALR_1_grammer();
    LALR_1_grammer(string l, vector<string> ri);
    LALR_1_grammer(string l, vector<string> ri,int d,set<string> &f); 
    ~LALR_1_grammer();

    void set_grammer(string l, vector<string> r);
    void set_left(string l);
    void set_right(vector<string> r);
    void insert_search(string f);
    void insert_search(set<string> f);
    
    string to_string();

    bool operator==(const LALR_1_grammer& g) const {
        return left == g.left && right == g.right && dot == g.dot;
    }
    bool operator!=(const LALR_1_grammer& g) const {
        return !(left == g.left && right == g.right && dot == g.dot);
    }
};

namespace std
{
    template <>
    struct hash<LALR_1_grammer>
    {
        size_t operator()(const LALR_1_grammer& g) const {
        // 将左部符号和右部符号的哈希值合并成一个哈希值
        size_t h1 = std::hash<std::string>{}(g.left);
        size_t h2 = 0;
        for (auto& s : g.right) {
            h2 ^= hash<std::string>{}(s) + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
        }
        return h1 ^ h2;
    }
    };
}

#endif // !LL_1_GRAMMER