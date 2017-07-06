#ifndef PLAYER_H
#define PLAYER_H
#include "myStruct.h"
#include <vector>
#include <string>
using namespace std;
class Dealer;
class Player
{
    public:
        Player(string n,int c);
        void init(){
            bet = 0;
            pot_ID = -1;
            isAll_in = false;
            isFold = false;
            pflop_bet.clear();
            flop_bet.clear();
            turn_bet.clear();
            river_bet.clear();
        };
        void print_hole_cards();
        static bool hash_val_greater(Player *l, Player *r) { return (l->hash_val <r->hash_val); }
        static bool bet_smaller(Player *l, Player *r) { return (l->bet > r->bet); }

        vector<card_t> hole_card;
        string name;
        int chip;
        int bet;
        int pot_ID;
        bool fold(Dealer *dealer);
        bool  check_call(int call_size);
       // bool check(int call_size);
        bool  raise(int raise_to,Dealer *dealer);
        int  blind_bet(int blind);
        int action(Dealer *dealer); //return push out chip,not total bet
        bool isFold;
        bool isRaise;

        bool isAll_in;
        int hash_val;
        int position;

        vector<int> pflop_bet;
        vector<int> flop_bet;
        vector<int> turn_bet;
        vector<int> river_bet;
    private:
        void record_bet(int bet,int stage);
};

#endif
