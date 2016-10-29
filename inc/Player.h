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

        static bool hash_val_greater(Player *l, Player *r) { return (l->hash_val <r->hash_val); }
        static bool bet_smaller(Player *l, Player *r) { return (l->bet > r->bet); }

        vector<card_t> hole_card;

        int chip;
        int ID;
        int bet;
        int pot_ID;
        void fold(Dealer *dealer);
        void  call(int call_size);
        bool check(int call_size);
        bool  raise(int raise_to,Dealer *dealer);
        int  blind_bet(int blind);
        int action(Dealer *dealer);
        bool isFold;
        bool isAll_in;
        int hash_val;


        vector<int> pflop_bet;
        vector<int> flop_bet;
        vector<int> turn_bet;
        vector<int> river_bet;
    private:
        void record_bet(int bet,int stage);
};

#endif
