#include <stdio.h>      /* printf, scanf, puts, NULL */


#include <vector>
#include <algorithm>    // std::sort
#include <iostream>
#include <iterator>
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
                dealer.print_card(*dealer.deck_it);
                dealer.shared_cards.push_back(*(dealer.deck_it++));
            }
        }
        else
        {

            if(dealer.shared_cards.size() == 3)
                cout<<"Turn"<<endl;
            else
                cout<<"River"<<endl;
            dealer.shared_cards.push_back(*(dealer.deck_it++));
            for(unsigned int i=0;i < dealer.shared_cards.size();i++)
                dealer.print_card(dealer.shared_cards[i]);
        }
        cout<<endl;
        betting(dealer,players);

    }while(dealer.remain_players >1 && dealer.shared_cards.size()!=5);

    //judge players card , create hash strength
    vector<Player*> ply_hash_greater;
    ply_hash_greater.clear();
    for(int i =0;i<PLAYERS;i++)
    {
        if(!players[i].isFold)
        {
            players[i].hash_val = hash_rank(dealer.judge(players[i].hole_card));
            ply_hash_greater.push_back(&players[i]);
            cout<<"Player "<<ply_hash_greater.back()->ID<<" hash  "<<ply_hash_greater.back()->hash_val <<endl;
        }
    }
    sort(ply_hash_greater.begin(), ply_hash_greater.end(), Player::hash_val_greater);

    //win pot field
    //players get main pot, side pot...etc
    //if tie , split pot

    for(unsigned int i =0;i<dealer.pots.size();i++)
        cout<<"pot "<<i<<" "<<dealer.pots[i]<<endl;
    for(unsigned int i =0;i<ply_hash_greater.size();i++)
        cout<<"Player "<<ply_hash_greater[i]->ID<<" potID = "<<ply_hash_greater[i]->pot_ID<<endl;

    //start from the highest pot ID
    //if players have pot ID >=current pot ID
    //he can join current pot competition
    vector<Player*> join_comp;
    vector<Player*> split_player;
    while(!dealer.pots.empty())
    {
        split_player.clear();
        join_comp.clear();
        for(unsigned int i =0;i<ply_hash_greater.size();i++)
        {
            if(ply_hash_greater[i]->pot_ID >= (int)dealer.pots.size() -1)
            {
               join_comp.push_back(ply_hash_greater[i] );
            }
        }
        for(unsigned int i= 0;i<join_comp.size();i++)
        {
            if(join_comp[i]->hash_val==join_comp.back()->hash_val)
                split_player.push_back(join_comp[i]);
        }
        if(split_player.size() > 1)
            cout<<"split the pot\n";
        if(split_player.size() == 0)
        {
            cout<<" split_player.size() == 0\n";
            cout<<"dealer.pots.back() / split_player.size() error"<<endl;
            break;
        }
        int final_chip = dealer.pots.back() / split_player.size();
        for(unsigned int i=0;i<split_player.size();i++)
           split_player[i]->chip += final_chip;
        dealer.pots.pop_back();
    }

    for(int i=0;i<PLAYERS;i++)
    {
        cout<<"Player "<<i<<" ";
        dealer.print_card(players[i].hole_card[0]);
        dealer.print_card(players[i].hole_card[1]);
        cout<<players[i].chip<<"$"<<endl;
    }
  return 0;
}

