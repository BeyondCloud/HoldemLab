#ifndef DEALER_H
#define DEALER_H
#include "myStruct.h"
#include "myDefine.h"
#include "Player.h"
#include "iostream"

class Dealer
{
    public:
        Dealer();
        vector<card_t> deck;
        vector<card_t> shared_cards;
        rank_t rank;
        rank_t judge(vector<card_t> c);
        void next_round(Player (&players)[PLAYERS]);
        void wake_up(Player &player);
        int deck_ptr;
        int sb_size;
        int bb_size;
        int call_to_size;
        int btn_player;
        int act_player;
        int bet_leader;
        int remain_players;
        int total_pot;
        vector<int> pots;
        int cur_pot_ID;


        void collect_bets(Player (&players)[PLAYERS]);
        void print_card(card_t c){
            std::cout<<card_val[c.val];
            std::cout<<card_suit[c.suit]<<" ";};

   private:
        int check_straight(vector<card_t> c);
        vector<card_t> cardsFlush;
};

#endif
