#include "Dealer.h"

#include <iostream>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include <algorithm>
#include <vector>

using namespace std;

Dealer::Dealer()
{
    srand(time(0));
    //new deck
    card_t c;
    sb_size = 15;
    bb_size = 30;

    cout<<"btn set to 0"<<endl;
    btn_player = 0;

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
//input 2 hole card
//type 1 high card
//type 2 pair
//type 3 two pair
//type 4 set
//type 5 straight
//type 6 flush
//type 7 full house
//type 8 quad
//type 9 straight flush
rank_t Dealer::judge(vector<card_t> c)
{
    rank_t rank;
    c.insert( c.end(), shared_cards.begin(), shared_cards.end() );
    const card_t hole[2] = {c[0],c[1]};
    if(c.size() < 5)
    {
        cout<<"invalid card size,except at least 5 card"<<endl;
        return rank;
    }
    sort(c.begin(),c.end(),Greater());

    //pair , set, quad
    int same = 0;
    int prev_val = 0;
    vector<int> pairs;
    int SET = 0;
    int straight = 0;
    int suit[4] = {0};
    int flush = 0;
    for(int i=0;i<c.size();i++)
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
            for(int j=0;j<c.size();j++)
            {
                if(c[j].suit == i)
                cardsFlush.push_back(c[i]);
            }
            int top_card = check_straight(cardsFlush);
            if(top_card > 0)
            {
                rank.type = 9;
                rank.kicker.push_back(top_card);
                return rank;
            }
            rank.type = 6;
            rank.kicker.push_back(flush);
            return rank;
        }
    }

    //check pair,set,quad
     for(int i=0;i<c.size();i++)
    {
        while(c[i].val == prev_val && i<c.size())
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
                rank.type = 8; //quad
                rank.kicker.push_back(prev_val);
                if(c.back().val == prev_val)
                    rank.kicker.push_back(c.end()[-5].val);
                else
                    rank.kicker.push_back(c.back().val);
                return rank;
            break;
        }
        same = 0;
        prev_val = c[i].val;

    }

    //full house checker
    if(pairs.size() !=0 && SET != 0)
    {
        rank.type = 7;
        rank.kicker.push_back(SET);
        rank.kicker.push_back(*max_element(pairs.begin(),pairs.end()));
        return rank;
    }

    straight = check_straight(c);
    if(straight != 0)
    {
        rank.type = 5;
        rank.kicker.push_back(straight);
        return rank;
    }

    if(SET != 0)
    {
        rank.type = 4;
        rank.kicker.push_back(SET);
        if(c.end()[-1].val == SET)
        {
            rank.kicker.push_back(c.end()[-4].val);
            rank.kicker.push_back(c.end()[-5].val);
        }
        else if(c.end()[-2].val == SET)
        {
            rank.kicker.push_back(c.end()[-1].val);
            rank.kicker.push_back(c.end()[-5].val);
        }
        else
        {
            rank.kicker.push_back(c.end()[-1].val);
            rank.kicker.push_back(c.end()[-2].val);
        }
        return rank;
    }


    //two pair checker
    else if(pairs.size() >= 2)
    {
        rank.type = 3;
        sort(pairs.begin(),pairs.end(),std::greater<int>());
        rank.kicker.push_back(pairs.end()[-1]);
        rank.kicker.push_back(pairs.end()[-2]);
        if(c.end()[-1].val == pairs.back())
        {
            if(c.end()[-3].val == pairs.end()[-2])
                rank.kicker.push_back(c.end()[-5].val);
            else
                rank.kicker.push_back(c.end()[-3].val);
        }
        else
            rank.kicker.push_back(c.end()[-1].val);
        return rank;
    }
    //pair checker
    else if(pairs.size() == 1)
    {
        rank.type = 2;
        rank.kicker.push_back(pairs.back());
        if(c.end()[-1].val == pairs.back())
        {
            for(int i=4;i>=2;i--)
                rank.kicker.push_back(c[i].val);
        }
        else if(c.end()[-2].val == pairs.back())
        {
            rank.kicker.push_back(c.end()[-1].val);
            rank.kicker.push_back(c.end()[-4].val);
            rank.kicker.push_back(c.end()[-5].val);
        }
        else if(c.end()[-3].val == pairs.back())
        {
            rank.kicker.push_back(c.end()[-1].val);
            rank.kicker.push_back(c.end()[-2].val);
            rank.kicker.push_back(c.end()[-5].val);
        }
        else
        {
            for(int i=-1;i>=-3;i--)
                rank.kicker.push_back(c.end()[i].val);
        }
        return rank;
    }
    //high card
    else
    {
        rank.type = 1;
        for(int i=-1;i>=-5;i--)
            rank.kicker.push_back(c.end()[i].val);
        return rank;
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
void Dealer::collect_bets(Player (&players)[PLAYERS])
{
    vector<pot_ID_t> potrnks;
    pot_ID_t potrnk;

    for(int i=0;i<PLAYERS;i++)
    {
        if(players[i].bet != 0)
        {
//            if(players[i].isAll_in)
//                players[i].pot_ID = (int)pots.size();
           potrnk.ID = players[i].ID ;
           potrnk.bet = players[i].bet;
           potrnks.push_back(potrnk);
        }
    }
    //sort all player bet to create side pot
    if(potrnks.size() != 0)
    {
        sort(potrnks.begin(),potrnks.end(),pot_bet_smaller());
        while(players[potrnks[0].ID].bet != 0)
        {
            int sum = 0;
            int smallest_bet;
            bool update_pot_ID = false;
            sum = players[potrnks.back().ID].bet * potrnks.size();
            cout<<"sum"<<sum<<endl;
            smallest_bet = potrnks.back().bet;
            for(int i=0;i<potrnks.size();i++)
            {
                players[potrnks[i].ID].bet -= smallest_bet;
                if(players[potrnks[i].ID].chip == 0)
                {
                    players[potrnks[i].ID].pot_ID = cur_pot_ID;
                    update_pot_ID = true;
                }
            }
            if(update_pot_ID)
                 cur_pot_ID++;
            for(int i=0;i<potrnks.size();i++)
            {
                if(players[potrnks[i].ID].chip != 0)
                    players[potrnks[i].ID].pot_ID = cur_pot_ID;
            }
            pots.back()+=sum;
        };
    }
}
void Dealer::next_round(Player (&players)[PLAYERS])
{
    cout<<"new round start..."<<endl;
    cout<<"small blind = "<<sb_size<<endl;
    cout<<"big blind = "<<bb_size<<endl;



//    if(players[(btn_player+1)%PLAYERS].chip<sb_size)
//    {
//        pot+=players[(btn_player+1)%PLAYERS].chip;
//        players[(btn_player+1)%PLAYERS].chip=0;
//    }
//    else
//    {
//        pot+=sb_size;
//        players[(btn_player+1)%PLAYERS].chip-=sb_size;
//    }
    call_to_size = bb_size;
    remain_players = PLAYERS;
    total_pot = 0;
    deck_ptr = 0;
    cur_pot_ID = 0;
    btn_player = (btn_player+1)%PLAYERS;
    act_player = (btn_player+3)%PLAYERS; //UTG
    bet_leader = act_player; //UTG
    pots.push_back(0);
    random_shuffle(deck.begin(),deck.end());
    cout<<"btn is player "<<btn_player<<endl;
    cout<<"player "<<act_player<<" is first to act"<<endl;
    //reset player param
    for(int i=0;i<PLAYERS;i++)
    {
        if(players[i].chip > 0 )
        {
            players[i].bet = 0;
            players[i].rnk.kicker.clear();
            players[i].isAll_in = false;
            players[i].isFold = false;
        }
        else
            players[i].isFold = true;
    }
    //deal card
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].hole_card.push_back(deck[deck_ptr++]);
        players[i].hole_card.push_back(deck[deck_ptr++]);
        cout<<"Player "<<i<<" ";
        print_card(players[i].hole_card[0]);
        print_card(players[i].hole_card[1]);
        cout<<players[i].chip<<"$"<<endl;
    }
    //bet small blind
    total_pot += players[(btn_player+1)%PLAYERS].blind_bet(sb_size);
    total_pot += players[(btn_player+2)%PLAYERS].blind_bet(bb_size);
}
void Dealer::wake_up(Player &player)
{
        cout<<"Player "<<player.ID<<" turn"<<endl;
        cout<<"Pot:"<<total_pot<<" ,Your chip: "<<player.chip<<endl;
        cout<<"Your action?"<<endl;
        total_pot += player.action(this);
}

