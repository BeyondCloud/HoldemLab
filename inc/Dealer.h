#ifndef DEALER_H
#define DEALER_H
#include "myStruct.h"
#include "myDefine.h"
#include "Player.h"
#include "iostream"

class Dealer
{
    public:
        Dealer(vector<Player*> players);
        rank_t rank;
        rank_t judge(vector<card_t> c);
        Player* bet_leader;
        vector<card_t> deck;
        vector<card_t> shared_cards;
        vector<card_t>::iterator deck_it;
        vector<Player*>::iterator ply_it;//for general purpose
        vector<Player*>::reverse_iterator ply_rit;//for general purpose
        vector<Player*>::iterator act_ply;
        //vector<Player> *p;//won't use this , since we need to delete
                            //player when it's chip = 0
                            //we only want to kill pointer
        vector<Player*> plys;   //pointer to players send into table
                                //erase player if chip<0

        vector<Player*> ply_nf_seq;
                                //players not fold action sequence
                                //former player act earlier.
                                 // players>3 case:[0]=SB,[1]=BB...,[N]=BTN
                                 // players=2 case:[0]=BB,[1]=SB
                                 //erase if fold
        int sb;
        int bb;
        uint16_t round_cnt;
        int call_to_size;
        //int btn_player; //won't use player pointer
                        //since player will be remove at any time
                        //this will record the index of ply pos
        int total_pot;
        vector<int> pots; //use pot vector to store side pots if needed
                          //if A bet 45$ B all in 30$
                          //side pot 15$ will push_back this vector
                          //later dealer will return 15 to A in distribute_pot()
        int cur_pot_ID;
        int stage;
        int all_in_plys_cnt;

        void game_cycle(); //return false is player <=1
        void collect_bets();
        void start_betting();
        void print_card(card_t c){
            std::cout<<card_val[c.val];
            std::cout<<card_suit[c.suit]<<" ";};
        void print_help();

        void players_join(vector<Player*> players);

        void print_public_cards();
        void print_round_info();
        void set_blind(int sb_val,int bb_val);
        void next_ply();
        int hash_rank(const rank_t &str);
   private:
        static bool bet_smaller(Player *l, Player *r) { return (l->bet > r->bet); }
        static bool hash_val_greater(Player *l, Player *r) { return (l->hash_val <r->hash_val); }
        void remove_0chip_players();
        void wake_up(vector<Player*>::iterator);
        void split_pot(vector<Player*> &split_player);
        void distribute_pot();
        void position_shift();  //means pass down button to next player
        int stage_init_pot_size;
        int check_straight(vector<card_t> c);

};
inline void Dealer::players_join(vector<Player*> players)
{
    plys = players;
    for(uint8_t i=0;i<plys.size();i++)
        plys[i]->d = this;
}

inline int Dealer::hash_rank(const rank_t &rnk)
{
    int tmp = 0;
    tmp += rnk.type * pow_13[5];
    int kicker_size = rnk.kicker.size();
    for(int i = 0;i <kicker_size ;i++)
    {
        tmp+= (rnk.kicker[i]-1)*pow_13[kicker_size -i-1];
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
inline void Dealer::position_shift()
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
    if(act_ply+1 ==ply_nf_seq.end())
        act_ply =ply_nf_seq.begin();
    else
        act_ply++;
}
inline void Dealer::split_pot(vector<Player*> &split_player)
{
    cout<<"split the pot\n";
    int final_chip = pots.back() / split_player.size();
    //handling odd chip split
    //in this case ,the earliest player got odd chip
    int odd_chip = pots.back()-final_chip*split_player.size();
    for(uint8_t i=0;i<split_player.size();i++)
    {
        cout<<"Player"<<split_player[i]->name<<" takes "<< final_chip<<"$"<<endl;

        split_player[i]->chip += final_chip;

        if(split_player[i] == ply_nf_seq.front())
             split_player[i]+= odd_chip;
    }

    pots.pop_back();
}

#endif
