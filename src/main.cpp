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
#include "AI_allin.h"



using namespace std;
string namelist[3] = {"A","B","C"};
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
    //players.push_back(new Player(namelist[0],1000));
   //initialize player's name and chips
   // for(int i=1;i<TBL_SEATS;i++)
    players.push_back(new AI_allin(namelist[0],1000));
    players.push_back(new AIplayer(namelist[1],1000));
    players.push_back(new AIplayer(namelist[2],1000));

 //   players.push_back(new AI_allin(namelist[1],1000));
 //   players.push_back(new AI_allin(namelist[2],1000));
    //game cycle start here
    Dealer dealer(players);
    for(int i = 0;i<50;i++)
    {
        dealer.game_cycle();

        dealer.plys.front()->win_cnt++;
        for(int i=0;i<players.size();i++)
            players[i]->chip=1000;
        dealer.players_join(players);

    }
    for(int i=0;i<players.size();i++)
       cout<<namelist[i]<<" "<<players[i]->win_cnt<<endl;

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

