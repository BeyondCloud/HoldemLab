#ifndef AI_ALLIN_H
#define AI_ALLIN_H
#include "Player.h"
#include "Dealer.h"

class AI_allin: public Player
{
    public:
    AI_allin(string n,int c):Player(n,c)
                            {is_AI = true;};
    act_t getStrategy();
    private:
    act_t pflop_act();
};
#endif
