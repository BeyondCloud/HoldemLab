#ifndef DEALER_H
#define DEALER_H
#include "myStruct.h"
#include "myDefine.h"
#include "Player.h"
#include "iostream"

class Dealer
{
    public:
        Dealer(vector<Player> &players);
        vector<card_t> deck;
        vector<card_t> shared_cards;
        rank_t rank;
        rank_t judge(vector<card_t> c);
        vector<card_t>::iterator deck_it;

        vector<Player*>::iterator ply_it;//for general purpose
        vector<Player*>::reverse_iterator ply_rit;//for general purpose

        Player* bet_leader;
        vector<Player*>::iterator act_ply;//this point to


        //vector<Player> *p;//won't use this , since we need to delete
                            //player when it's chip = 0
                            //we only want to kill pointer
        vector<Player*> plys;   //pointer to players send into table
                                //erase player if chip<0

        vector<Player*> ply_nf; //players not fold yet
                                 //[0]=SB,[1]=BB...,[N]=BTN
                                 //erase if fold

        int sb;
        int bb;
        int call_to_size;
        //int btn_player; //won't use player pointer
                        //since player will be remove at any time
                        //this will record the index of ply pos
        int total_pot;
        vector<int> pots;
        int cur_pot_ID;
        int stage;
        int all_in_plys_cnt;

        void new_round();
        void wake_up(vector<Player*>::iterator);
        void distribute_pot();

        void collect_bets();
        void start_betting();
        void print_card(card_t c){
            std::cout<<card_val[c.val];
            std::cout<<card_suit[c.suit]<<" ";};
        int hash_rank(const rank_t &str);
        void betting();
        void print_help();
        void print_public_cards();
        void remove_0chip_players();
        void set_blind(int sb_val,int bb_val);
        void passdown_button();
        void next_ply();
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
inline void Dealer::remove_0chip_players()
{
    for(ply_it = plys.begin();ply_it!=plys.end();)
    {
        if((*ply_it)->chip == 0 )
            plys.erase(ply_it);
        else
            ply_it++;
    }
}
inline void Dealer::passdown_button()
{
    plys.push_back(plys.front());
    plys.erase(plys.begin());
}

inline void Dealer::set_blind(int sb_val,int bb_val)
{
    sb = sb_val;
    bb = bb_val;
}
inline void Dealer::next_ply()
{
    if(act_ply+1 ==ply_nf.end())
        act_ply =ply_nf.begin();
    else
        act_ply++;
}


#endif
