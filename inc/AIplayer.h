#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "Player.h"

#include "myStruct.h"

class AIplayer: public Player
{
    public:
    AIplayer(string n,int c):Player(n,c)
                            {is_AI = true;};
    virtual act_t thinking();
};
#endif
