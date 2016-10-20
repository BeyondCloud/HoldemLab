#ifndef PositionMgr_H
#define PositionMgr_H
#include "myDefine.h"
#include "Player.h"
#include "myStruct.h"
struct position_t
{
    Player *btn;
    Player *sb;
    Player *bb;
    Player *actor;
    Player *last_bet;
};
void out_ring(Player *player);
void init_players_ring(Player (&players)[PLAYERS]);
position_t new_round(position_t position);
position_t set_btn(Player *player,position_t position);



#endif
