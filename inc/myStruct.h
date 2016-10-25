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
struct strength_t
{
    int ID;
    int type;
    int hash_kicker;
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
struct type_Greater
{
    bool operator()( const strength_t& l, const strength_t& r ) const {
        return l.type < r.type;
    }
};

#endif
