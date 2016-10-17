#ifndef PLAYERMGR_H
#define PLAYERMGR_H
#include "myDefine.h"
#include "Player.h"
#include "myStruct.h"
class PlayerMgr
{
    public:
        PlayerMgr(Player (&players)[PLAYERS]);
        void out(Player player);
        void init_players_ring(Player (&players)[PLAYERS]);
};
#endif
