#include <stdio.h>      /* printf, scanf, puts, NULL */


#include <vector>
#include <algorithm>    // std::sort
#include <iostream>
#include "myStruct.h"
#include "myDefine.h"
#include "Dealer.h"
#include "Player.h"
using namespace std;
inline void betting(Dealer &dealer,Player (&players)[PLAYERS])
{
    do
    {
        if(!players[dealer.act_player].isFold && players[dealer.act_player].chip > 0)
            dealer.wake_up(players[dealer.act_player]);
        //next one act
        dealer.act_player = (dealer.act_player+1)%PLAYERS;
    }while(dealer.act_player != dealer.bet_leader);
    dealer.collect_bets(players);
}


inline int hash_rank(const rank_t &str)
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

int main ()
{
    Dealer dealer;
    Player players[PLAYERS];
    cout<<"How to play: "<<endl;
    cout<<"c(space)any positive int=check"<<endl;
    cout<<"c = call"<<endl;
    cout<<"f = fold,"<<endl;
    cout<<"r(space)raise amount)=raise"<<endl;

    for(int i=0;i<PLAYERS;i++)
    {
        players[i].ID=i;
        players[i].chip=500*(i+1);
    }

    //game cycle start here
    dealer.next_round(players); //init player ring,shuffle and set deck ptr to 0

//    for(int i=0;i<PLAYERS;i++)
//    {
//        dealer.cards7.push_back(players[i].hole_card[0]);
//        dealer.cards7.push_back(players[i].hole_card[1]);
//        players[i].rank = dealer.judge(dealer.cards7);
//        dealer.cards7.pop_back();
//        dealer.cards7.pop_back();
//    }

//    for(int i=0;i<PLAYERS;i++)
//    {
//        cout<<endl;
//        cout<< "player "<< i <<"   "<<card5_name[players[i].rank.type]<<endl;
//        cout<< "kicker: ";
//        for(int j =0;j<(int)players[i].rank.kicker.size();j++)
//            cout<<players[i].rank.kicker[j]<<" ";
//
//    }
    cout<<"pre Flop"<<endl;
    dealer.act_player = (dealer.btn_player+3)%PLAYERS;
    betting(dealer,players);

    do
    {
        dealer.call_to_size = 0;
        dealer.act_player = (dealer.btn_player+1)%PLAYERS;
        dealer.bet_leader = dealer.act_player;
        if(dealer.shared_cards.empty())
        {
           cout<<"Flop"<<endl;

           for(int i=0;i<3;i++)
            {
                dealer.print_card(dealer.deck[dealer.deck_ptr]);
                dealer.shared_cards.push_back(dealer.deck[dealer.deck_ptr++]);
            }
        }
        else
        {

            if(dealer.shared_cards.size() == 3)
                cout<<"Turn"<<endl;
            else
                cout<<"River"<<endl;
            dealer.shared_cards.push_back(dealer.deck[dealer.deck_ptr++]);
            for(int i=0;i<dealer.shared_cards.size();i++)
                dealer.print_card(dealer.shared_cards[i]);
        }
        cout<<endl;
        betting(dealer,players);

    }while(dealer.remain_players >1 && dealer.shared_cards.size()!=5);

    if(dealer.remain_players == 1)
    {
        players[dealer.bet_leader].chip += dealer.total_pot;
        cout<<"Player"<<dealer.bet_leader<<"win the pot "<<dealer.total_pot<<"$\n";
    }
    else
    {
        vector<rank_t> card_rnk;
        vector<int> remain_playerID;
        for(int i =0;i<PLAYERS;i++)
        {
            if(!players[i].isFold)
            {
                card_rnk.push_back(dealer.judge(players[i].hole_card));
                card_rnk[i].hash_val  = hash_rank(card_rnk[i]);
                cout<<"Player "<<i<<" "<<"hash "<<card_rnk[i].hash_val <<endl;
                card_rnk[i].ID = players[i].ID;
            }
        }
        sort(card_rnk.begin(),card_rnk.end(),rank_Greater());
        //win pot field
        //players get main pot, side pot...etc
        //if tie , split pot
        int pots_taken = 0;
        int rnkptr = -1;
        bool is_pots_clean = false;
        cout<<"best pot ID"<<players[card_rnk.back().ID].pot_ID<<endl;
        for(int i =0;i<dealer.pots.size();i++)
            cout<<"pot "<<i<<" "<<dealer.pots[i]<<endl;
        for(int i =0;i<card_rnk.size();i++)
        {
             cout<<"player"<<players[card_rnk[i].ID].ID
            <<" potID "<<players[card_rnk[i].ID].pot_ID<<endl;
        }
        //start from the highest pot ID
        //if players have pot ID >=current pot ID
        //he can join current pot competition
        vector<int> join_comp;
        vector<int> split_player;
        while(!dealer.pots.empty())
        {
            for(int i =0;i<card_rnk.size();i++)
            {
                if(players[card_rnk[i].ID].pot_ID >= dealer.pots.size() -1)
                {
                   join_comp.push_back(players[card_rnk[i].ID].ID );
                }
            }
            int chips = 0;
            for(int i= 0;i<join_comp.size();i++)
            {
                if(card_rnk[join_comp[i]].hash_val ==card_rnk[join_comp.back()].hash_val)
                    split_player.push_back(join_comp[i]);
            }

            if(split_player.size() > 1)
                cout<<"split the pot\n";
            int final_chip = dealer.pots.back() / split_player.size();
            for(int i=0;i<split_player.size();i++)
            {
               players[split_player[i]].chip += final_chip;

            }
            dealer.pots.pop_back();
        }
    }

    for(int i=0;i<PLAYERS;i++)
    {
        cout<<"Player "<<i<<" ";
        dealer.print_card(players[i].hole_card[0]);
        dealer.print_card(players[i].hole_card[1]);
        cout<<players[i].chip<<"$"<<endl;
    }
//    for(int i=0;i<PLAYERS;i++)
//    {
//        cout<<players[i].prev->ID<<" ";
//        cout<<players[i].next->ID<<endl;
//
//    }
//    PositionMgr.out(players[2]);
//    for(int i=0;i<PLAYERS;i++)
//    {
//        cout<<players[i].prev->ID<<" ";
//        cout<<players[i].next->ID<<endl;
//    }


//    int record[10] = {0};
//    for(int cnt =0;cnt<1000;cnt++)
//    {
//
//        cards7.clear();
//        for(int i=0;i<7;i++)
//        {
//            cards7.push_back(deck[i]);
//        }
//        rank=judge(cards7);
//        record [rank.type]++;
//    }
//
//    for(int i =1;i<=9;i++)
//        cout<<"type"<<i<<" "<<record[i]<<endl;
//


  return 0;
}

