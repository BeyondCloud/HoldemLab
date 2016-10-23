#ifndef DEALER_H
#define DEALER_H
#include "myStruct.h"
#include "myDefine.h"
#include "Player.h"
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
    private:
        int check_straight(vector<card_t> c);
        vector<card_t> cardsFlush;
};
#endif
