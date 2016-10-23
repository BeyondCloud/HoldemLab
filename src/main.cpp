#include <stdio.h>      /* printf, scanf, puts, NULL */


#include <vector>
#include <iostream>
#include "myStruct.h"
#include "myDefine.h"
#include "Dealer.h"
#include "Player.h"

using namespace std;
const char card_val[15] = {'\0','\0','2','3','4','5','6','7','8','9','T','J','Q','K','A'};
const char card_suit[4] = {'c','d','h','s'};
const string card5_name[10] = {"","high card","pair","two pair","set",
                            "straight","flush","full house","quad","straight flush"};

inline void print_card(card_t c)
{
    cout<<card_val[c.val];
    cout<<card_suit[c.suit]<<" ";
}
inline void betting(Dealer &dealer,Player (&players)[PLAYERS])
{
    do
    {
        if(!players[dealer.act_player].isFold)
            dealer.wake_up(players[dealer.act_player]);
        //next one act
        dealer.act_player = (dealer.act_player+1)%PLAYERS;
    }while(dealer.act_player != dealer.bet_leader);
}
//return the top card of straight if it is
//else return 0

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

    //deal card
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].strength.kicker.clear();
        players[i].hole_card[0] = dealer.deck[dealer.deck_ptr++];
        players[i].hole_card[1] = dealer.deck[dealer.deck_ptr++];
        cout<<"Player "<<i<<" ";
        print_card(players[i].hole_card[0]);
        print_card(players[i].hole_card[1]);
        cout<<players[i].chip<<"$"<<endl;
    }

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
    do
    {
        dealer.call_size = 0;
        dealer.act_player = (dealer.btn_player+1)%PLAYERS;
        if(dealer.shared_cards.empty())
        {
           cout<<"Flop"<<endl;
           for(int i=0;i<3;i++)
            {
                print_card(dealer.deck[dealer.deck_ptr]);
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
            print_card(dealer.deck[dealer.deck_ptr]);
            dealer.shared_cards.push_back(dealer.deck[dealer.deck_ptr++]);
        }
        betting(dealer,players);
    }while(dealer.remain_players >1 && dealer.shared_cards.size()!=5);

    if(dealer.remain_players == 1)
    {
        players[dealer.bet_leader].chip += dealer.pot;
        cout<<"Player"<<dealer.bet_leader<<"win"<<dealer.pot;

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

