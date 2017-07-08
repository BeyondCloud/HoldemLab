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
string namelist[TBL_SEATS] = {"A","B","C"};
int main ()
{
    vector<Player> players;

   // Player players[TBL_SEATS];//this means players sit around poker table
                            //player with 0 chip wasn't able to join the game


    //initialize player's name and chips
    for(int i=0;i<TBL_SEATS;i++)
        players.push_back(Player(namelist[i],(i+1)*500));

    //game cycle start here
    Dealer dealer(players);

    while(dealer.new_round()){}

    for(int i=0;i<TBL_SEATS;i++)
    {
        cout<<"Player "<<i<<" ";
        players[i].print_hole_cards();
        cout<<players[i].chip<<"$"<<endl;
    }

  return 0;
}

