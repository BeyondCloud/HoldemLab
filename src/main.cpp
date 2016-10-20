#include <stdio.h>      /* printf, scanf, puts, NULL */


#include <vector>
#include <iostream>
#include "myStruct.h"
#include "myDefine.h"
#include "Dealer.h"
#include "Player.h"
#include "PositionMgr.h"

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
//return the top card of straight if it is
//else return 0

int main ()
{
    Dealer dealer;
    Player players[PLAYERS];
    position_t position;
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].ID=i;
        players[i].chip=1500;
    }

    init_players_ring(players); //setup double link list

    //game cycle start here
    position = set_btn(&players[0],position);
    position = new_round(position);
    dealer.new_game(players); //init player ring,shuffle and set deck ptr to 0

    //deal card
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].strength.kicker.clear();
        players[i].hole_card[0] = dealer.deck[dealer.deck_ptr++];
        players[i].hole_card[1] = dealer.deck[dealer.deck_ptr++];
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

    //pre-flop bet until big blind
    //set bb to bet leader initially.If someone raise,bet leader will reset
    while(true)
    {
        if(position.actor != position.actor->next)
            position.actor->action(&dealer);
        if(position.actor == position.last_bet)
            break;
        position.actor = position.actor->next;
    }
    cout<<"flop"<<endl;
    if(position.actor->next != position.actor)
    {
        for(int i=0;i<3;i++)
        {
            dealer.shared_cards.push_back(dealer.deck[dealer.deck_ptr++]);
        }
        while(true)
        {
            position.actor->action(&dealer);
            if(position.actor == position.last_bet)
                break;
            position.actor = position.actor->next;
        }

    }
    //turn and river
    do
    {
        dealer.shared_cards.push_back(dealer.deck[dealer.deck_ptr++]);

        if(dealer.shared_cards.size()==4)
             cout<<"turn"<<endl;
        else if(dealer.shared_cards.size()==5)
             cout<<"river"<<endl;

        while(true)
        {
            position.actor->action(&dealer);

            if(position.actor == position.last_bet)
                break;
            position.actor = position.actor->next;
        }
    }while(position.actor->next != position.actor &&dealer.shared_cards.size()!=5);

    init_players_ring(players); //setup double link list

    cout<<"pot"<<dealer.pot<<" player$ "<<players[0].chip;
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

