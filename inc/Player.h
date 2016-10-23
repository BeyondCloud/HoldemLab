#ifndef PLAYER_H
#define PLAYER_H
#include "myStruct.h"

#include <vector>
using namespace std;
class Dealer;
class Player
{
    public:
        Player();
        card_t hole_card[2];
        strength_t strength;

        int chip;
        int ID;

        void fold();
        int call(int call_size);
        void action(Dealer *dealer);
        int raise(int bet,int min_raise_size);
        bool isFold;

        vector<int> pflop_bet;
        vector<int> flop_bet;
        vector<int> turn_bet;
        vector<int> river_bet;
    private:
        void record_bet(int bet,int stage);
};
#endif
