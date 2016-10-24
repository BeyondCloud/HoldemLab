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
        vector<card_t> hole_card;
        strength_t strength;

        int chip;
        int ID;
        int bet;
        void fold();
        void  call(int call_size);
        void action(Dealer *dealer);
        bool  raise(int raise_to,int call_size);
        bool isFold;

        vector<int> pflop_bet;
        vector<int> flop_bet;
        vector<int> turn_bet;
        vector<int> river_bet;
    private:
        void record_bet(int bet,int stage);
};
#endif
