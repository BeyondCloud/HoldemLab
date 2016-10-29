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

int main ()
{
    Dealer dealer;
    Player players[PLAYERS];

    cout<<"press h to see help window\n";
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].ID=i;
        players[i].chip=500*(i+1);
    }

    //game cycle start here
    dealer.next_round(players); //init player ring,shuffle and set deck ptr to 0
    dealer.start_betting(players);
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

