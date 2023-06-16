#include "edge.h"

edge::edge() {}
edge::edge(state &start, state &end, char info)
{
    from = start;
    to = end;
    this->info = info;
}
bool edge::operator==(const edge &other)
{
    if (info == other.info && from.id == other.from.id & to.id == other.to.id)
        return true;
    else
        return false;
}