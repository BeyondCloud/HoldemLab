#ifndef JUDGE_H
#define JUDGE_H
#include "myStruct.h"
class Dealer
{
    public:
        Dealer();
        vector<card> deck;
        vector<card> cards7;
        strength_t strength;
        strength_t judge(vector<card> &c);
    private:

        int check_straight(vector<card> c);
        void new_deck();
        void shuffle();
        vector<card> cardsFlush;
};
#endif
