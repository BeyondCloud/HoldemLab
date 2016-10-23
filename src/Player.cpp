#include "Player.h"
#include "Dealer.h"
#include "myStruct.h"
#include "myDefine.h"
#include "useful_func.h"
#include <cstdlib>
#include <iostream>

using namespace std;
Player::Player()
{

}
void Player::fold()
{
        isFold = true;
        cout<<"player "<<ID<<" fold"<<endl;
}
int Player::call(int call_size)
{
    if(chip<call_size)
    {
        cout<<"you don't have that much money"<<endl;
        cout<<"assume you all in"<<endl;
        int tmp = chip;
        chip = 0;
        return tmp;
    }
    cout<<"you call "<<chip<<"$"<<endl;
    chip -= call_size;

    return call_size;
}

void Player::record_bet(int bet,int stage)
{

    switch(stage)
    {
        case 0:
            pflop_bet.push_back(bet);
        break;
        case 3:
            flop_bet.push_back(bet);
        break;
        case 4:
            turn_bet.push_back(bet);
        break;
        case 5:
            river_bet.push_back(bet);
        break;
    }
}
int Player::raise(int bet,int call_size)
{
    if(chip<bet)
    {
        cout<<"you don't have that much money(";
        cout<<call_size<<")"<<endl;
        cout<<"assume you all in"<<endl;
        cout<<"you call "<<chip<<"$"<<endl;
        int tmp = chip;
        chip = 0;
        return tmp;
    }
    if(bet<call_size)
    {
        cout<<"Your raise is smaller than call size(";
        cout<<call_size <<")"<<endl;
        cout<<"please try other action"<<endl;
        return -1;
    }
    cout<<"you raise "<<bet<<"$"<<endl;
    return bet;
}
void Player::action(Dealer *dealer)
{
    char act;

    int bet=0;
    do
    {
        cin>>act;
        switch(act)
        {
            case 'f':
                if(dealer->remain_players > 1)
                {
                    dealer->remain_players--;
                    fold();
                }
                else
                    cout<<"you are last player in the ring"<<endl;
            break;
            case 'c':
                if(dealer->call_size != 0)
                    bet = call(dealer->call_size);
                else
                    cout<<"you check"<<endl;
            break;
            case 'r':
                cout<<"amount?"<<endl;
                char bet_char[16];
                cin >>bet_char;
                if(!isNumber(bet_char))
                {
                    bet = -1;
                    break;
                }
                bet = raise(atoi(bet_char),dealer->call_size);
                dealer->bet_leader = ID;
                dealer->call_size = bet;
            break;
        }
    }while(bet == -1);
    if(bet != 0)
    {
        record_bet(bet,dealer->shared_cards.size());
        dealer->pot+=bet;
    }
}
