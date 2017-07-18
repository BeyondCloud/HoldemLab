#include "AIplayer.h"
#include <iostream>
#include <conio.h>

using namespace std;

act_t AIplayer::thinking()
{
    act_t act;
    if(hole_card[0].val+hole_card[1].val>14)
        act.type = 'a';
    else
        act.type = 'f';



    return act;

}
