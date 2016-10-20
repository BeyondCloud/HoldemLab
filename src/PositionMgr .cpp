#include "PositionMgr.h"
#include "myStruct.h"
#include "myDefine.h"
#include <iostream>
void out_ring(Player *player)
{
    player->next->prev = player->prev;
    player->prev->next = player->next;
}
void init_players_ring(Player (&players)[PLAYERS])
{
    for(int i=0;i<PLAYERS;i++)
    {
       players[i].next = &players[i==PLAYERS-1?0:i+1];
       players[i].prev = &players[i==0?PLAYERS-1:i-1];
    }
}
position_t set_btn(Player *player,position_t position)
{
    position.btn = player;
    position.sb = position.btn->next;
    position.bb = position.sb->next;
    return position;
}
position_t new_round(position_t position)
{
    position.btn = position.btn->next;
    position.sb = position.btn->next;
    position.bb = position.sb->next;
    position.actor = position.bb->next;
    position.last_bet = position.bb;
    return position;
}
