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
}
void Player::print_hole_cards()
{
    cout<<card_val[hole_card[0].val]<<card_suit[hole_card[0].suit]<<" ";
    cout<<card_val[hole_card[1].val]<<card_suit[hole_card[1].suit];
}
bool Player::fold(Dealer *dealer)
{
    if(dealer->remain_players > 1)
    {
        dealer->remain_players--;
        isFold = true;
        cout<<"player "<<name<<" fold"<<endl;
    }
    else
        cout<<"you are last player in the ring"<<endl;
    return true;

}

//bool Player::check(int call_to_size)
//{
//    if( bet==call_to_size)
//    {
//        cout<<"you check\n";
//        return true;
//    }
//    else
//    {
//
//        cout<<"you can't check\n";
//        cout<<"your bet = "<<bet<<"\n";
//        cout<<"call to size = "<<call_to_size<<"\n";
//
//        return false;
//    }
//
//}
bool Player::check_call(int call_to_size)
{
    if(bet+chip<call_to_size)
    {
        cout<<"you go all in"<<endl;
        isAll_in = true;
        bet += chip;
        chip = 0;
    }
    else if (bet == call_to_size)
        cout<<"you check "<<call_to_size<<"$"<<endl;
    else
    {
        cout<<"you call "<<call_to_size<<"$"<<endl;
        chip -= (call_to_size-bet);
        bet = call_to_size;

    }
    return true;
}
int  Player::blind_bet(int blind)
{
    if(chip <= blind)
    {
        cout<<"Player "<<name<<" all in\n";
        bet = chip;
        chip = 0;
        isAll_in = true;
    }
    else
    {
        bet = blind;
        chip -= bet;
        cout<<"Player "<<name<<" (blind)" << blind <<"$\n";
    }
    return bet;
}
bool Player::raise(int raise_to,Dealer *d)
{
    int call_to_size = d->call_to_size;
    if(chip+bet<=raise_to || (chip+bet)<=call_to_size )
    {
        cout<<"You go all in "<<chip<<"$"<<endl;
        isAll_in = true;
        bet += chip;
        chip = 0;
    }
    else
    {
        if(raise_to < call_to_size*2)
        {
            cout<<"Your raise is smaller than min raise size(";
            cout<<  call_to_size * 2 <<")"<<endl;
            cout<<"please try other action"<<endl;
            return false;
        }
        else
        {
            cout<<"you raise from"<<bet<<"$ to"<<raise_to<<"$"<<endl;
            chip-=raise_to - bet;
            bet = raise_to;
        }
    }
    d->bet_leader = d->act_player;
    d->call_to_size = bet;
    return true;
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
//return number of chips pushed out,not total bet
//EX: A bet 5 ,B raise to 10,A call,in this case return 10-5=5
int Player::action(Dealer *d)
{
    bool done_act ;
    int orig_bet = bet;
    char act;
    do
    {
        cout<<"Your action?"<<endl;
        cin>>act;
         switch(act)
        {
            case 'f':
                done_act = fold(d);
            break;
            case 'c':
                done_act = check_call(d->call_to_size);
            break;
            case 'r':
                char bet_char[16];
                int raise_to;
                if(bet + chip > d->call_to_size)
                {
                    cout<<"amount?"<<endl;
                    cin >>bet_char;
                    if(!isNumber(bet_char))
                    {
                        cout<<"invalid input\n";
                        done_act = false;
                        break;
                    }
                    raise_to = atoi(bet_char);
                }
                else    //ALL IN
                    raise_to = bet + chip;
                done_act = raise(raise_to,d);
            break;
            default:
                d->print_help();
                done_act = false;
            break;
        }
    }while(!done_act);

    if(bet != 0)
        record_bet(bet,d->shared_cards.size());

    return  bet-orig_bet;

}
