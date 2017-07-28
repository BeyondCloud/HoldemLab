#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "Player.h"
#include "Dealer.h"

class AIplayer: public Player
{
    public:
    AIplayer(string n,int c):Player(n,c)
                            {is_AI = true;};
    act_t getStrategy();
    private:
    act_t pflop_act();
};
#endif
