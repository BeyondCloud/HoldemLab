#include "Player.h"
#include "Dealer.h"
#include "myStruct.h"
#include "myDefine.h"
#include "Useful_func.h"

#include <conio.h>
#include <cstdlib>
#include <iostream>

using namespace std;
Player::Player(string n,int c)
{
    is_AI = false;
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
bool Player::fold(Dealer *dealer)
{
    if(dealer->ply_nf_seq.size() > 1)
    {
        isFold = true;
        cout<<"player "<<name<<" fold"<<endl;
    }
    else
        cout<<"you are last player in the ring"<<endl;
    return true;

}
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
//this has nothing to do with game logic
//You can use it to browse betting history
void Player::record_bet(int bet,int stage)
{
    switch(stage)
    {
        case PFLOP:
            pflop_bet.push_back(bet);
        break;
        case FLOP:
            flop_bet.push_back(bet);
        break;
        case TURN:
            turn_bet.push_back(bet);
        break;
        case RIVER:
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
    string bet_str;
    int raise_to;
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
                if(bet + chip > d->call_to_size)
                {
                    cout<<"amount?"<<endl;
                    cin >>bet_str;
                    if(!isNumber(bet_str))
                    {
                        cout<<"invalid input\n";
                        done_act = false;
                        break;
                    }
                    raise_to=stoi( bet_str );
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
        record_bet(bet,d->stage);

    return  bet-orig_bet;

}
//AI action
int Player::action(Dealer *d,act_t a)
{
    bool done_act;
    int orig_bet = bet;

    switch(a.type)
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
            if(bet + chip > d->call_to_size)
                done_act = raise(d,a.raise_to);
            else    //ALL IN
                done_act = raise(d,bet + chip);
        break;
        default:
            done_act = false;
        break;
    }
    if(!done_act)
    {
        cerr<<"illegal act:"<<a.type<<",raise to:"<<a.raise_to<<endl;
        system("pause");
    }
    if(bet != 0)
        record_bet(bet,d->stage);
    return  bet-orig_bet;
}
