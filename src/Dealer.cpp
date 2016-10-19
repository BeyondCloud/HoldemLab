#include "Dealer.h"
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include <algorithm>
#include <vector>
Dealer::Dealer()
{
    srand(time(0));
    //new deck
    card_t c;
    for(int i = 2;i<=14;i++)
    {
        for(int j = 0;j<4;j++)
        {
            c.val  = i;
            c.suit = j;
            deck.push_back(c);
        }
    }
    shuffle();
}
//input 2 hole card + 5 community card
//type 1 high card
//type 2 pair
//type 3 two pair
//type 4 set
//type 5 straight
//type 6 flush
//type 7 full house
//type 8 quad
//type 9 straight flush
strength_t Dealer::judge(vector<card_t> c)
{
    const card_t hole[2] = {c.end()[-1],c.end()[-2]};
    strength_t strength;

    sort(c.begin(),c.end(),Greater());

    //pair , set, quad
    int same = 0;
    int prev_val = 0;
    vector<int> pairs;
    int SET = 0;
    int straight = 0;
    int suit[4] = {0};
    int flush = 0;
    for(int i=0;i<7;i++)
    {
        suit[c[i].suit]++;
    }
    //flush checker
    for(int i= 0;i<4;i++)
    {
        if(suit[i] >=5 )
        {
            cardsFlush.clear();
            if(hole[0].suit == i)
                flush = hole[0].val;
            if(hole[1].suit == i && hole[1].val > flush)
                flush = hole[1].val;
            for(int j=0;j<7;j++)
            {
                if(c[j].suit == i)
                cardsFlush.push_back(c[i]);
            }
            int top_card = check_straight(cardsFlush);
            if(top_card > 0)
            {
                strength.type = 9;
                strength.kicker.push_back(top_card);
                return strength;
            }
            strength.type = 6;
            strength.kicker.push_back(flush);
            return strength;
        }
    }

    //check pair,set,quad
     for(int i=0;i<7;i++)
    {

        while(c[i].val == prev_val && i<7)
        {
            same++;
            i++;
        }
        switch(same)
        {
            case 1:
                pairs.push_back(prev_val);
            break;
            case 2:
                if(SET == 0 )
                    SET = prev_val;
                else if(SET > prev_val)
                    SET = prev_val;
            break;
            case 3:
                strength.type = 8;
                strength.kicker.push_back(prev_val);
                if(c[6].val == prev_val)
                    strength.kicker.push_back(c[2].val);
                else
                    strength.kicker.push_back(c[6].val);
                return strength;
            break;
        }
        same = 0;
        prev_val = c[i].val;

    }

    //full house checker
    if(pairs.size() !=0 && SET != 0)
    {
        strength.type = 7;
        strength.kicker.push_back(SET);
        strength.kicker.push_back(*max_element(pairs.begin(),pairs.end()));
        return strength;
    }

    straight = check_straight(c);
    if(straight != 0)
    {
        strength.type = 5;
        strength.kicker.push_back(straight);
        return strength;
    }

    if(SET != 0)
    {
        strength.type = 4;
        strength.kicker.push_back(SET);
        if(c[6].val == SET)
        {
            strength.kicker.push_back(c[3].val);
            strength.kicker.push_back(c[2].val);
        }
        else if(c[5].val == SET)
        {
            strength.kicker.push_back(c[6].val);
            strength.kicker.push_back(c[2].val);
        }
        else
        {
            strength.kicker.push_back(c[6].val);
            strength.kicker.push_back(c[5].val);
        }
        return strength;
    }


    //two pair checker
    else if(pairs.size() >= 2)
    {
        strength.type = 3;
        sort(pairs.begin(),pairs.end(),int_Greater());
        strength.kicker.push_back(pairs.back());
        strength.kicker.push_back(pairs.end()[-2]);
        if(c[6].val == pairs.back())
        {
            if(c[4].val == pairs.end()[-2])
                strength.kicker.push_back(c[2].val);
            else
                strength.kicker.push_back(c[4].val);
        }
        else
            strength.kicker.push_back(c[6].val);
        return strength;
    }
    //pair checker
    else if(pairs.size() == 1)
    {
        strength.type = 2;
        strength.kicker.push_back(pairs.back());
        if(c[6].val == pairs.back())
        {
            for(int i=4;i>=2;i--)
                strength.kicker.push_back(c[i].val);
        }
        else if(c[5].val == pairs.back())
        {
            strength.kicker.push_back(c[6].val);
            strength.kicker.push_back(c[3].val);
            strength.kicker.push_back(c[2].val);
        }
        else if(c[4].val == pairs.back())
        {
            strength.kicker.push_back(c[6].val);
            strength.kicker.push_back(c[5].val);
            strength.kicker.push_back(c[2].val);
        }
        else
        {
            for(int i=6;i>=4;i--)
                strength.kicker.push_back(c[i].val);
        }
        return strength;
    }
    //high card
    else
    {
        strength.type = 1;
        for(int i=6;i>=2;i--)
            strength.kicker.push_back(c[i].val);
        return strength;
    }
}
//return the top card of straight if it is
//else return 0
int Dealer::check_straight(vector<card_t> c)
{
    int straight_cnt = 0;
    int prev_val = 0;


    if(c.back().val == 14)
        prev_val = 1;

    for(unsigned int i=0;i<c.size();i++)
    {
        if(c[i].val-prev_val == 1)
            straight_cnt++;
        else if(c[i].val-prev_val > 1)
        {
            if(straight_cnt >= 4)
                return c[i-1].val;
            else
                straight_cnt = 0;
        }
        prev_val = c[i].val;
    }
    return 0;
}
void Dealer::shuffle()
{
    random_shuffle(deck.begin(),deck.end());
}
