#ifndef DEALER_H
#define DEALER_H
#include "myStruct.h"
#include "myDefine.h"
#include "Player.h"
#include "iostream"
const char card_val[15] = {'\0','\0','2','3','4','5','6','7','8','9','T','J','Q','K','A'};
const char card_suit[4] = {'c','d','h','s'};
const string card5_name[10] = {"","high card","pair","two pair","set",
                            "straight","flush","full house","quad","straight flush"};

class Dealer
{
    public:
        Dealer();
        vector<card_t> deck;
        vector<card_t> shared_cards;
        strength_t strength;
        strength_t judge(vector<card_t> c);
        void next_round(Player (&players)[PLAYERS]);
        void wake_up(Player &player);
        int pot;
        int deck_ptr;
        int sb_size;
        int bb_size;
        int call_size;
        int btn_player;
        int act_player;
        int bet_leader;
        int remain_players;
        void collect_bets(Player (&players)[PLAYERS]);
        void print_card(card_t c){
            std::cout<<card_val[c.val];
            std::cout<<card_suit[c.suit]<<" ";};
    private:
        int check_straight(vector<card_t> c);

        vector<card_t> cardsFlush;
};

#endif
