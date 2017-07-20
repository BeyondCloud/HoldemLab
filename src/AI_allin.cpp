#include "AI_allin.h"
#include <iostream>
#include <conio.h>

using namespace std;

act_t AI_allin::thinking()
{
    switch(d->stage)
    {
        case PFLOP:
          return pflop_act();
        break;
    }
}
act_t AI_allin::pflop_act()
{
    act_t act;
    //early position
    act.type = 'a';
    return act;
}
