#include "state.h"

state::state() {}
state::state(int num)
{
    id = num;
    is_final = false;
}
state::state(int num, bool is_final)
{
    id = num;
    this->is_final = is_final;
}
state::state(int id, vector<state> &v)
{
    this->id = id;
    for (state i : v)
    {
        state_set.insert(i.id);
        if (i.is_final)
            set_is_final(i.is_final);
    }
}
void state::set_is_final(bool is_final)
{
    this->is_final = is_final;
}
bool state::equals(state s)
{
    // if(state_set.empty()&&s.state_set.empty()) return id==s.id;
    return state_set == s.state_set;
}
bool state::is_count(int state_id)
{
    return state_set.count(state_id);
}
void state::add_state(vector<state> &v)
{
    for (state i : v)
        state_set.insert(i.id);
}
void state::add_state(int v)
{
    state_set.insert(v);
}
void state::add_state(state s)
{
    for (int i : s.state_set)
    {
        state_set.insert(i);
    }
}