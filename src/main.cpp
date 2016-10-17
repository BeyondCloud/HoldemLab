#include <stdio.h>      /* printf, scanf, puts, NULL */


#include <vector>
#include <iostream>
#include "myStruct.h"
#include "myDefine.h"
#include "Dealer.h"
#include "Player.h"
#include "PlayerMgr.h"

using namespace std;
const char card_val[15] = {'\0','\0','2','3','4','5','6','7','8','9','T','J','Q','K','A'};
const char card_suit[4] = {'c','d','h','s'};
void print_card(card_t c)
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
    PlayerMgr playerMgr(players);
    for(int i=0;i<7;i++)
    {
        dealer.cards7.push_back(dealer.deck[i]);
    }
      dealer.strength=dealer.judge(dealer.cards7);
      for(int i=0;i<7;i++)
      {
         print_card(dealer.cards7[i]);
      }
      cout<<endl;
      cout<<"type"<< dealer.strength.type<<endl;

    for(int i=0;i<PLAYERS;i++)
    {
        players[i].ID=i;
    }
    for(int i=0;i<PLAYERS;i++)
    {
        cout<<players[i].prev->ID<<" ";
        cout<<players[i].next->ID<<endl;

    }
    playerMgr.out(players[2]);
    for(int i=0;i<PLAYERS;i++)
    {
        cout<<players[i].prev->ID<<" ";
        cout<<players[i].next->ID<<endl;
    }


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

