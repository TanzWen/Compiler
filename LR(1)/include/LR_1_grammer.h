#ifndef LR_1_GRAMMER
#define LR_1_GRAMMER


# include <vector>
# include <string>
# include <functional>

using namespace std;

class LR_1_grammer
{
public:
    string left;
    vector<string> right;
    int gram_size;
    string search;
    
    int dot;

    LR_1_grammer();
    LR_1_grammer(string l, vector<string> ri);
    LR_1_grammer(string l, vector<string> ri,int d,string f); 
    ~LR_1_grammer();

    void set_grammer(string l, vector<string> r);
    void set_left(string l);
    void set_right(vector<string> r);
    void set_search(string f);
    string to_string();

    bool operator==(const LR_1_grammer& g) const {
        return left == g.left && right == g.right && dot == g.dot && search == g.search;
    }
};

namespace std
{
    template <>
    struct hash<LR_1_grammer>
    {
        size_t operator()(const LR_1_grammer& g) const {
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