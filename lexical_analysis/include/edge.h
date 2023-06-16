#ifndef EDGE_H
#define EDGE_H

#include "state.h"

class edge
{
public:
    /* 成员变量 */
    char info; // 边的信息
    state from;
    state to;
    edge();
    edge(state &start, state &end, const char info);

    bool operator==(const edge &other);
};

#endif