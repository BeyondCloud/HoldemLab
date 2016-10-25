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
        if(!players[dealer.act_player].isFold && players[dealer.act_player].chip != 0)
            dealer.wake_up(players[dealer.act_player]);
        //next one act
        dealer.act_player = (dealer.act_player+1)%PLAYERS;
    }while(dealer.act_player != dealer.bet_leader);
}


inline int hash_kicker(vector<int> kicker)
{
    int tmp=0;
    for(int i = 0;i<kicker.size();i++)
    {
        tmp += (kicker[i]-1)*pow_13[kicker.size()-i];
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
        players[i].chip=1500;
    }

    //game cycle start here
    dealer.next_round(players); //init player ring,shuffle and set deck ptr to 0

//    for(int i=0;i<PLAYERS;i++)
//    {
//        dealer.cards7.push_back(players[i].hole_card[0]);
//        dealer.cards7.push_back(players[i].hole_card[1]);
//        players[i].strength = dealer.judge(dealer.cards7);
//        dealer.cards7.pop_back();
//        dealer.cards7.pop_back();
//    }

//    for(int i=0;i<PLAYERS;i++)
//    {
//        cout<<endl;
//        cout<< "player "<< i <<"   "<<card5_name[players[i].strength.type]<<endl;
//        cout<< "kicker: ";
//        for(int j =0;j<(int)players[i].strength.kicker.size();j++)
//            cout<<players[i].strength.kicker[j]<<" ";
//
//    }
    cout<<"pre Flop"<<endl;
    dealer.act_player = (dealer.btn_player+3)%PLAYERS;
    betting(dealer,players);
    dealer.collect_bets(players);
    do
    {
        dealer.call_size = 0;
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
            cout<<endl;
            dealer.shared_cards.push_back(dealer.deck[dealer.deck_ptr++]);
            for(int i=0;i<dealer.shared_cards.size();i++)
                dealer.print_card(dealer.shared_cards[i]);
            cout<<endl;

        }
        betting(dealer,players);
        dealer.collect_bets(players);

    }while(dealer.remain_players >1 && dealer.shared_cards.size()!=5);

    if(dealer.remain_players == 1)
    {
        players[dealer.bet_leader].chip += dealer.pot;
        cout<<"Player"<<dealer.bet_leader<<"win the pot "<<dealer.pot<<"$\n";
    }
    else
    {
        vector<strength_t> str;
        vector<int> remain_playerID;
        for(int i =0;i<PLAYERS;i++)
        {
            if(!players[i].isFold)
            {
                str.push_back(dealer.judge(players[i].hole_card));
                str[i].hash_kicker = hash_kicker(str[i].kicker);
                str[i].ID = i;
            }
        }
        sort(str.begin(),str.end(),type_Greater());

        cout<<"best type"<<str.back().type<<endl;
        int current_type = str.begin();
        //==========================

        vector<int> same_type_ID;
        for(int i =0;i<remain_playerID.size();i++)
        {
            if(str[i].type == best_type)
                same_type_ID.push_back(remain_playerID[i]);
        }
        int remain_players = remain_playerID.size();
        int kickers = str[same_type_ID[0]].kicker.size();

        for(int k = 0;k<kickers;k++)
        {
            int best_kicker=0;
            for(int i = 0;i<same_type_ID.size();i++)
            {
                if(str[same_type_ID[i]].kicker[k] >= best_kicker)
                    best_kicker=str[same_type_ID[i]].kicker[k];
            }
            for(int i = 0;i<same_type_ID.size();i++)
            {
                if(same_type_ID[i] != -1)
                {
                    if(str[same_type_ID[i]].kicker[k] != best_kicker)
                    {
                        same_type_ID[i] = -1;
                        remain_players--;
                    }
                }
            }
            if(remain_players==1)
                break;
        }
        for(int i = 0;i<same_type_ID.size();i++)
        {
            if(same_type_ID[i] != -1)
            {
                cout<<"player"<<same_type_ID[i]<<"win"<<endl;
            }
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
//        strength=judge(cards7);
//        record [strength.type]++;
//    }
//
//    for(int i =1;i<=9;i++)
//        cout<<"type"<<i<<" "<<record[i]<<endl;
//


  return 0;
}

