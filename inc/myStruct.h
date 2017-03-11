#ifndef MYSTRUCT_H
#define MYSTRUCT_H
#include <vector>
using namespace std;
//struct position_t is in PositionMgr
struct card_t
{
    int val;
    int suit;
};
struct rank_t
{
    int type;
    int hash_val;
    vector<int> kicker;
};
struct pot_ID_t
{
    int ID;
    int bet;
};
struct Greater
{
    bool operator()( const card_t& l, const card_t& r ) const {
        return l.val < r.val;
    }
};


#endif
