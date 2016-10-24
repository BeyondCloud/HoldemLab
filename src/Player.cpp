#include "Player.h"
#include "Dealer.h"
#include "myStruct.h"
#include "myDefine.h"
#include "useful_func.h"
#include <conio.h>
#include <cstdlib>
#include <iostream>

using namespace std;
Player::Player()
{
    bet = 0;
}
void Player::fold()
{
        isFold = true;
        cout<<"player "<<ID<<" fold"<<endl;
}
void Player::call(int call_size)
{
    if(chip<call_size)
    {
        cout<<"you don't have that much money"<<endl;
        cout<<"assume you all in"<<endl;
        bet = chip;
        chip = 0;
    }
    cout<<"you call "<<call_size<<"$"<<endl;
    chip -= call_size;

    bet = call_size;
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
bool Player::raise(int raise_to,int call_size)
{
    if(chip<raise_to)
    {
        cout<<"you don't have that much money\n";
        cout<<"assume you go all in"<<endl;
        cout<<"you call "<<chip<<"$"<<endl;
        bet = chip;
        chip = 0;
    }
    if(raise_to<=call_size*2)
    {
        cout<<"Your raise is smaller than min raise size(";
        cout<<  call_size * 2 <<")"<<endl;
        cout<<"please try other action"<<endl;
        return false;
    }
    cout<<"you raise from"<<bet<<"$ to"<<raise_to<<"$"<<endl;
    return true;
}
void Player::action(Dealer *dealer)
{
    bool valid_act = true;
    do
    {
         switch(getch())
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
                if(bet < dealer->call_size)
                    call(dealer->call_size-bet);
                else
                    cout<<"you check"<<endl;
            break;
            case 'r':
                cout<<"amount?"<<endl;
                char bet_char[16];
                cin >>bet_char;
                if(!isNumber(bet_char))
                {
                    valid_act = false;
                    break;
                }
                int amount = atoi(bet_char);
                valid_act = raise(amount,dealer->call_size);
                dealer->bet_leader = ID;
                dealer->call_size = bet;
            break;
        }
    }while(!valid_act);
    if(bet != 0)
    {
        record_bet(bet,dealer->shared_cards.size());
    }
}
