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

    dealer.distribute_pot(players);

    for(int i=0;i<PLAYERS;i++)
    {
        cout<<"Player "<<i<<" ";
        dealer.print_card(players[i].hole_card[0]);
        dealer.print_card(players[i].hole_card[1]);
        cout<<players[i].chip<<"$"<<endl;
    }
  return 0;
}

