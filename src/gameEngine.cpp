#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <iostream>
#include <algorithm>
#define ROUND 10
#define PLAYERS 2
using namespace std;
struct card
{
    int val;
    int suit;
};
struct strength_t
{
    int type;
    vector<int>kicker;
};
vector<card> deck;
vector<card> cards7;
vector<card> cardsFlush;
const char card_val[15] = {'\0','\0','2','3','4','5','6','7','8','9','T','J','Q','K','A'};
const char card_suit[4] = {'c','d','h','s'};
struct Greater
{
    bool operator()( const card& l, const card& r ) const {
        return l.val < r.val;
    }
};
struct int_Greater
{
    bool operator()( const int& l, const int& r ) const {
        return l < r;
    }
};
void new_deck()
{
    card c;
    for(int i = 2;i<=14;i++)
    {
        for(int j = 0;j<4;j++)
        {
            c.val  = i;
            c.suit = j;
            deck.push_back(c);
        }
    }
}
void print_card(card c)
{
    cout<<card_val[c.val];
    cout<<card_suit[c.suit]<<endl;
}
//return the top card of straight if it is
//else return 0
int check_straight(vector<card> c)
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
                return c[i].val;
            else
                straight_cnt = 0;
        }
        prev_val = c[i].val;
    }
    return 0;
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
strength_t judge(vector<card> c)
{
    card hole[2] = {c[0],c[1]};
    strength_t strength;

    sort(c.begin(),c.end(),Greater());

//    for(int i=0;i<7;i++)
//        print_card(c[i]);

    //pair , set, quad
    int same = 0;
    int prev_val = 0;
    vector<int> pairs;
    int SET = 0;

    int straight = 0;
    int straight_cnt = 0;

    int suit[4] = {0};
    int flush = 0;
    bool isFlush = false;
    int straightFlush = 0;
    for(int i=0;i<7;i++)
    {
        suit[c[i].suit]++;
    }
    //flush checker
    for(int i= 0;i<4;i++)
    {
        if(suit[i] >=5 )
        {
            isFlush = true;
            cardsFlush.clear();
            if(hole[0].suit == i)
                flush = c[0].val;
            if(hole[1].suit == i && hole[1].val > flush)
                flush = c[1].val;
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
        pairs.pop_back();
        strength.kicker.push_back(pairs.back());
        if(c[6].val == pairs[0])
        {
            if(c[4].val == pairs[1])
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
            strength.kicker.push_back(c[4].val);
            strength.kicker.push_back(c[3].val);
            strength.kicker.push_back(c[2].val);
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
            strength.kicker.push_back(c[6].val);
            strength.kicker.push_back(c[5].val);
            strength.kicker.push_back(c[4].val);
        }
        return strength;
    }
    //high card
    else
    {
        strength.type = 1;
        strength.kicker.push_back(c[6].val);
        strength.kicker.push_back(c[5].val);
        strength.kicker.push_back(c[4].val);
        strength.kicker.push_back(c[3].val);
        strength.kicker.push_back(c[2].val);
        return strength;
    }


}
int main ()
{
    new_deck();
    srand(time(0));
    strength_t strength;
    int record[10] = {0};

    for(int cnt =0;cnt<1000;cnt++)
    {
        random_shuffle(deck.begin(),deck.end());
        cards7.clear();
        for(int i=0;i<7;i++)
        {
            cards7.push_back(deck[i]);
        }
        strength=judge(cards7);
        record [strength.type]++;
    }

    for(int i =1;i<=9;i++)
        cout<<"type"<<i<<" "<<record[i]<<endl;


//    cout<<"type"<<strength.type;


  return 0;
}

