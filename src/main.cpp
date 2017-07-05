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
    Player players[PLAYERS];//this means players sit around poker table
                            //player with 0 chip wasn't able to join the game

    cout<<"press h to see help window\n";

    //initialize player's name and chips
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].name=to_string(i);
        players[i].chip=500;
    }

    //game cycle start here
    dealer.new_round(players); //init player ring,shuffle and set deck ptr to 0
    dealer.start_betting(players);
    dealer.distribute_pot(players);
    for(int i=0;i<PLAYERS;i++)
    {
        cout<<"Player "<<i<<" ";
        players[i].print_hole_cards();
        cout<<players[i].chip<<"$"<<endl;
    }

  return 0;
}

