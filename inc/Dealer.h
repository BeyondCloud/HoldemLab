#ifndef JUDGE_H
#define JUDGE_H
#include "myStruct.h"
class Dealer
{
    public:
        Dealer();
        vector<card_t> deck;
        vector<card_t> cards7;
        strength_t strength;
        strength_t judge(vector<card_t> &c);
    private:

        int check_straight(vector<card_t> c);
        void new_deck();
        void shuffle();
        vector<card_t> cardsFlush;
};
#endif
