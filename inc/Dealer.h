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
        vector<card_t> cards7;
        vector<card_t> shared_cards;
        strength_t strength;
        strength_t judge(vector<card_t> c);
        int deck_ptr;
        int min_bet_size;
        void new_game(Player (&players)[PLAYERS]);
        int pot;
        int sb;
        int bb;
    private:
        int check_straight(vector<card_t> c);
        vector<card_t> cardsFlush;
};
#endif
