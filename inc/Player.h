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
        void init(int pos){
            position = pos;
            bet = 0;
            pot_ID = -1;
            isAll_in = false;
            isFold = false;
            pflop_bet.clear();
            flop_bet.clear();
            turn_bet.clear();
            river_bet.clear();
        };
        act_t thinking(); //auto input

        void print_hole_cards();
        void print_info();

        static bool hash_val_greater(Player *l, Player *r) { return (l->hash_val <r->hash_val); }
        static bool bet_smaller(Player *l, Player *r) { return (l->bet > r->bet); }

        vector<card_t> hole_card;
        string name;
        int chip;
        int bet;
        int pot_ID;
        bool fold(Dealer *dealer);
        bool  check_call(Dealer *dealer,int call_size);
        bool all_in(Dealer *dealer);
       // bool check(int call_size);
        bool  raise(Dealer *dealer,int raise_to);
        int  blind_bet(Dealer *dealer,int blind);
        int action(Dealer *dealer); //return push out chip,not total bet
        int action(Dealer *dealer,act_t act); //auto input

        bool isFold;
        bool isRaise;
        bool isAll_in;
        bool is_AI;
        int hash_val;
        int position; //players>3 :0=SB ,1=BB...N=BTN
                      //players=2 :0=BTN,1=BB


        vector<int> pflop_bet;
        vector<int> flop_bet;
        vector<int> turn_bet;
        vector<int> river_bet;
    private:

        void record_bet(int bet,int stage);
};

#endif
