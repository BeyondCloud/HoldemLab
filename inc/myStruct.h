#ifndef MYSTRUCT_H
#define MYSTRUCT_H
#include <vector>
using namespace std;
struct card
{
    int val;
    int suit;
};
struct strength_t
{
    int type;
    vector<int> kicker;
};
struct Greater
{
    bool operator()( const card& l, const card& r ) const {
        return l.val < r.val;
    }
};
struct int_Greater
{
    bool operator()( const int& l, const int& r ) const {
        return l < r;
    }
};


#endif
