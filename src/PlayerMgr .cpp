#include "PlayerMgr.h"
#include "myStruct.h"
#include "myDefine.h"
PlayerMgr::PlayerMgr(Player (&players)[PLAYERS])
{
    init_players_ring(players);
}
void PlayerMgr::out(Player player)
{
    player.next->prev = player.prev;
    player.prev->next = player.next;

}
void PlayerMgr::init_players_ring(Player (&players)[PLAYERS])
{
    for(int i=0;i<PLAYERS;i++)
    {
       players[i].next = &players[i==PLAYERS-1?0:i+1];
       players[i].prev = &players[i==0?PLAYERS-1:i-1];
    }
}
