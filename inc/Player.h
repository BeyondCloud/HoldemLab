#ifndef PLAYER_H
#define PLAYER_H
#include "myStruct.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;
class Dealer;
class Player
{
    public:
        Player(string n,int c):name(n),chip(c){is_AI=false;};
        ~Player();

        //call before each run
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
        //auto input
        //AI can implement it's own method by rewrite this
        virtual act_t thinking(){cout<<"base";act_t a;return a;};

        void print_hole_cards();
        void print_info();


        vector<card_t> hole_card;
        string name;
        int chip;
        int bet;
        int win_cnt=0;
        int pot_ID;
        bool fold();
        bool  check_call(int call_size);
        bool all_in();
        bool  raise(int raise_to);
        int  blind_bet(int blind);
        int action(); //return push out chip,not total bet
        int action(act_t act); //auto input
        Dealer *d;
        bool isFold;
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
