#include "PositionMgr.h"
#include "myStruct.h"
#include "myDefine.h"
PositionMgr::PositionMgr(Player (&players)[PLAYERS])
{
    init_players_ring(players);
    new_round(players[0]);


}
void PositionMgr::out(Player player)
{
    player.next->prev = player.prev;
    player.prev->next = player.next;

}
void PositionMgr::init_players_ring(Player (&players)[PLAYERS])
{
    for(int i=0;i<PLAYERS;i++)
    {
       players[i].next = &players[i==PLAYERS-1?0:i+1];
       players[i].prev = &players[i==0?PLAYERS-1:i-1];
    }

}
void PositionMgr::new_round(Player &player)
{
    btn = &player;
    sb = btn->next;
    bb = sb->next;
}
void PositionMgr::new_round()
{
    btn = btn->next;
    sb = btn->next;
    bb = sb->next;
}
