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

//    AIplayer test(namelist[0],1000);
//    Player* ptr;
//    ptr = &test;
//    ptr->thinking();
//    return 0;
    vector<Player*> players;
    Player user(namelist[0],1000);
    players.push_back(&user);
    AIplayer AI_1(namelist[1],1000);
    AIplayer AI_2(namelist[2],1000);
   //initialize player's name and chips
  //  for(int i=1;i<TBL_SEATS;i++)
    players.push_back(&AI_1);
    players.push_back(&AI_2);

    players.back()->thinking();

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

