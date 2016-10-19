#ifndef PositionMgr_H
#define PositionMgr_H
#include "myDefine.h"
#include "Player.h"
#include "myStruct.h"
class PositionMgr
{
    public:
        PositionMgr(Player (&players)[PLAYERS]);
        void out(Player player);
        void init_players_ring(Player (&players)[PLAYERS]);
        void new_round();
        void new_round(Player &player);
        Player *btn;
        Player *bb;
        Player *sb;
};
#endif
