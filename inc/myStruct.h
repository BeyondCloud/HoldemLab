#ifndef MYSTRUCT_H
#define MYSTRUCT_H
#include <vector>
using namespace std;
//struct position_t is in PositionMgr
struct card_t
{
    int val;
    int suit; //c,d,h,s
};
struct act_t
{
    char type;
    int raise_to;
    act_t()
    {
        raise_to = 0;
    }
};
struct rank_t
{
    int type;
    int hash_val;
    vector<int> kicker;
};
struct Greater
{
    bool operator()( const card_t& l, const card_t& r ) const {
        return l.val < r.val;
    }
};


#endif
