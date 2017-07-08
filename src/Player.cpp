#include "Player.h"
#include "Dealer.h"
#include "myStruct.h"
#include "myDefine.h"
#include "useful_func.h"
#include <conio.h>
#include <cstdlib>
#include <iostream>

using namespace std;
Player::Player(string n,int c)
{
    name = n;
    chip = c;

}
void Player::print_hole_cards()
{
    cout<<"["<<card_val[hole_card[0].val]<<card_suit[hole_card[0].suit]<<" ";
    cout<<card_val[hole_card[1].val]<<card_suit[hole_card[1].suit]<<"]";
}
void Player::print_info()
{
    if(position ==0)
        cout<<"(SB)\t";
    else if(position ==1)
        cout<<"(BB)\t";
    else
            cout<<"\t";

    if(isFold)
        cout<<"<FOLD>\t\t";
    else
        cout<<"Player "<<name<<"\t";
    print_hole_cards();
    cout<<chip<<"$\t";
    if(bet != 0)
        cout<<"bet "<<bet<<"$";
    cout<<endl;
}
bool Player::fold(Dealer *dealer)
{
    if(dealer->ply_nf.size() > 1)
    {
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
bool Player::check_call(Dealer *d,int call_to_size)
{
    if(bet+chip<=call_to_size)
        return all_in(d);
    else if (bet == call_to_size)
        cout<<"you check "<<endl;
    else
    {
        cout<<"you call "<<call_to_size<<"$"<<endl;
        chip -= (call_to_size-bet);
        bet = call_to_size;

    }
    return true;
}
int  Player::blind_bet(Dealer *d,int blind)
{
    if(chip <= blind)
        all_in(d);
    else
    {
        bet = blind;
        chip -= bet;
    }
    return bet;
}
bool Player::raise(Dealer *d,int raise_to)
{
    int call_to_size = d->call_to_size;
    if(chip+bet<=raise_to || (chip+bet)<=call_to_size )
        return all_in(d);
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
            d->call_to_size = bet;
            d->bet_leader = *(d->act_ply);
            return true;
        }
    }

}
bool Player::all_in(Dealer *d)
{
    cout<<"You go all in";
    bet +=chip;
    if(bet > d->call_to_size)
    {
        d->call_to_size = bet;
        d->bet_leader = *(d->act_ply);
    }
    chip = 0;
    isAll_in = true;
    d->all_in_plys_cnt++;
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
    string act;
    do
    {
        cout<<"Your action? (enter h to see help)"<<endl;
        cin>>act;
        switch(act[0])
        {
            case 'a':
                done_act = all_in(d);
            break;
            case 'f':
                done_act = fold(d);
            break;
            case 'c':
                done_act = check_call(d,d->call_to_size);
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
                done_act = raise(d,raise_to);
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
