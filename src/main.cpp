#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <vector>
#include <algorithm>    // std::sort
#include <iostream>
#include <iterator>


#include "myStruct.h"
#include "myDefine.h"
#include "Dealer.h"
#include "Player.h"
#include "AIplayer.h"

using namespace std;
string namelist[TBL_SEATS] = {"A","B","C"};
int main ()
{

/*
    string test = "a100";
    char A;
    string B;
    A = test[0];
    B = test.substr(1,test.size()-1);
    cout<<A;
    cout<<B;
    return 0;
    */

    vector<Player*> players;
    players.push_back(new Player(namelist[0],1000));
   //initialize player's name and chips
    for(int i=1;i<TBL_SEATS;i++)
        players.push_back(new AIplayer(namelist[i],1000));


    Dealer dealer(players);
    //game cycle start here
    while(dealer.game_cycle()){}
    /*
    for(int i=0;i<TBL_SEATS;i++)
    {
        cout<<"Player "<<i<<" ";
        players[i].print_hole_cards();
        cout<<players[i].chip<<"$"<<endl;
    }
    */
  return 0;
}

