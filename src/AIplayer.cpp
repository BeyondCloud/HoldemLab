#include "AIplayer.h"
#include <iostream>
#include <conio.h>

using namespace std;

act_t AIplayer::getStrategy()
{
    switch(d->stage)
    {
        case PFLOP:
          return pflop_act();
        break;
    }
}
act_t AIplayer::pflop_act()
{
    act_t act;

    //early position
    if(hole_card[0].val+hole_card[1].val>22||
       hole_card[0].val==hole_card[1].val)
        act.type = 'a';
    else if(chip>1500)
        act.type = 'a';
    else
        act.type = 'f';
    return act;
}
