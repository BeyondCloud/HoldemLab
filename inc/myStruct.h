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
    int ID;
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
struct int_Greater
{
    bool operator()( const int& l, const int& r ) const {
        return l < r;
    }
};
struct rank_Greater
{
    bool operator()( const rank_t& l, const rank_t& r ) const {
        return l.hash_val < r.hash_val;
    }
};

#endif
