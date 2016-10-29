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
        void distribute_pot(Player (&players)[PLAYERS]);
        vector<card_t>::iterator deck_it;

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
        void start_betting(Player (&players)[PLAYERS]);
        void print_card(card_t c){
            std::cout<<card_val[c.val];
            std::cout<<card_suit[c.suit]<<" ";};
        int hash_rank(const rank_t &str);
        void betting(Player (&players)[PLAYERS]);
        void print_help();



   private:
        int check_straight(vector<card_t> c);
        vector<card_t> cardsFlush;
};
inline int Dealer::hash_rank(const rank_t &str)
{
    int tmp = 0;
    tmp += str.type * pow_13[5];
    int kicker_size = str.kicker.size();
    for(int i = 0;i <kicker_size ;i++)
    {
        tmp+= (str.kicker[i]-1)*pow_13[kicker_size -i-1];
    }
    return tmp;
}
inline void Dealer::betting(Player (&players)[PLAYERS])
{
    do
    {
        if(!players[act_player].isFold && players[act_player].chip > 0)
            wake_up(players[act_player]);
        //next one act
        act_player = (act_player+1)%PLAYERS;
    }while(act_player != bet_leader);
    collect_bets(players);
}

#endif
