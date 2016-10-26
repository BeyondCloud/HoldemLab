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
        rank_t rnk;

        int chip;
        int ID;
        int bet;
        int pot_ID;
        void fold();
        void  call(int call_size);
        int  blind_bet(int blind);
        int action(Dealer *dealer);
        bool  raise(int raise_to,int call_to_size);
        bool isFold;
        bool isAll_in;


        vector<int> pflop_bet;
        vector<int> flop_bet;
        vector<int> turn_bet;
        vector<int> river_bet;
    private:
        void record_bet(int bet,int stage);
};
#endif
