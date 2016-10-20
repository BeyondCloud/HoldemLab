#include "Player.h"
#include "Dealer.h"
#include "myStruct.h"
#include "myDefine.h"
#include "PositionMgr.h"
#include <iostream>

using namespace std;
Player::Player()
{

}
void Player::fold()
{
    if(this->next != this)
    {
        out_ring(this);
        cout<<"player "<<ID<<" fold"<<endl;
    }
    else
        std::cout<<"you are the last player in the ring"<<endl;
}
void Player::check()
{

}
int Player::raise(int amount,int min_raise_size)
{
    if(amount)
    chip-=amount;
    cout<<"player "<<ID<<" raise"<<amount<<endl;
    return amount;
}
void Player::action(Dealer *d)
{
    d->pot+=raise(100,d->min_bet_size);
}
