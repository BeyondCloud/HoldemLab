#include "Dealer.h"

#include <iostream>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include <algorithm>
#include <vector>

using namespace std;

Dealer::Dealer()
{
    sb_size = 15;
    bb_size = 30;
    cout<<"btn set to 0"<<endl;
    btn_player = 0;

    //random shuffle,without this will create same value
    srand(time(0));

    //create a deck
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
    const card_t hole[2] = {c[0],c[1]};
    c.insert( c.end(), shared_cards.begin(), shared_cards.end() );

    ASSERT(c.size() > 4, "judge fail,except >4 card,card size = " << c.size());

    sort(c.begin(),c.end(),Greater());

    //pair , set, quad
    int same = 0;
    int prev_val = 0;
    vector<int> pairs;
    int SET = 0;
    int straight = 0;
    int suit[4] = {0};
    int flush = 0;
    for(unsigned int i=0;i<c.size();i++)
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
            for(unsigned int j=0;j<c.size();j++)
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
     for(unsigned int i=0;i<c.size();i++)
    {
        while(c[i].val == prev_val && i<(c.size()))
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
//manage pots , build side pot if needed
void Dealer::collect_bets(Player (&players)[PLAYERS])
{

    vector<Player*> ply_bets_smaller;
    int orig_pot_ID =cur_pot_ID;
    for(int i=0;i<PLAYERS;i++)
    {
        if(players[i].bet != 0)
        {
            if(players[i].isFold)
            {
                pots.back()+=players[i].bet;
                players[i].bet = 0;
            }
            else
            {
                ply_bets_smaller.push_back(&players[i]);
            }
        }
    }
    if(ply_bets_smaller.size() == 0)
        return;
    sort(ply_bets_smaller.begin(),ply_bets_smaller.end(),Player::bet_smaller);

    //if there are non equal bet (someone all in) , create side pot
    int cur_scan_bet = ply_bets_smaller.back()->bet;

    for(int i =ply_bets_smaller.size()-1 ;i >=0; i--)
    {
        if( ply_bets_smaller[i]->bet == cur_scan_bet)
        {
            ply_bets_smaller[i]->pot_ID =cur_pot_ID;
        }
        else
        {
            cout<<"new pot create"<<endl;
            cur_scan_bet = ply_bets_smaller[i]->bet;
            cur_pot_ID++;
            ply_bets_smaller[i]->pot_ID =cur_pot_ID;
            cout<<"player"<<ply_bets_smaller[i]->name<<" set to "<<cur_pot_ID;
            pots.push_back(0);
        }
    }
    // dealer take away the chip from smallest to biggest
    // to build the side pot until no one have bet chips remain

    while(!ply_bets_smaller.empty())
    {
        int bets_cnt = ply_bets_smaller.size();
        int players_bet_clean_cnt = 0;
        int smallest_bet = ply_bets_smaller.back()->bet;
        int sum = ply_bets_smaller.back()->bet * bets_cnt;
        pots[orig_pot_ID]+=sum;
        for(int i = 0;i<bets_cnt;i++)
        {
            ply_bets_smaller[i]->bet -= smallest_bet;
            if(ply_bets_smaller[i]->bet == 0)
               players_bet_clean_cnt++;
        }
        for(int i =0 ;i <players_bet_clean_cnt; i++)
            ply_bets_smaller.pop_back();
        orig_pot_ID++;
    }
}
void Dealer::next_round(Player (&players)[PLAYERS])
{
    cout<<"new round start..."<<endl;
    cout<<"small blind = "<<sb_size<<endl;
    cout<<"big blind = "<<bb_size<<endl;
    //init dealer
    call_to_size = bb_size;
    remain_players = 0;
    total_pot = 0;
    cur_pot_ID = 0;
    btn_player = (btn_player+1)%PLAYERS;
    act_player = (btn_player+3)%PLAYERS; //UTG
    bet_leader = act_player; //UTG
    pots.push_back(0);
    random_shuffle(deck.begin(),deck.end());
    deck_it = deck.begin();
    cout<<"btn is player "<<btn_player<<endl;
    cout<<"player "<<act_player<<" is first to act"<<endl;
    //reset player param
    for(int i=0;i<PLAYERS;i++)
    {
        if(players[i].chip > 0 )
        {
            players[i].bet = 0;
            players[i].pot_ID = -1;
            players[i].isAll_in = false;
            players[i].isFold = false;
            players[i].pflop_bet.clear();
            players[i].flop_bet.clear();
            players[i].turn_bet.clear();
            players[i].river_bet.clear();
            remain_players++;
        }
        else
            players[i].isFold = true;
    }
    //set position
    for(int position=1;position<=PLAYERS;position++)
    {
        players[(btn_player+position)%PLAYERS].position = position;
    }

    //deal card
    for(int i=0;i<PLAYERS;i++)
    {
        players[i].hole_card.push_back(*(deck_it++));
        players[i].hole_card.push_back(*(deck_it++));
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
        cout<<"Player "<<player.name<<" turn"<<endl;
        cout<<"Pot:"<<total_pot<<" ,Your chip: "<<player.chip<<endl;
        total_pot += player.action(this);
        cout<<endl;
}
void Dealer::distribute_pot(Player (&players)[PLAYERS])
{
    vector<Player*> ply_hash_greater;
        ply_hash_greater.clear();

    if(remain_players == 1)
    {
        cout<<"Everyone folded\n";
        for(int i =0;i<PLAYERS;i++)
        {
            if(!players[i].isFold)
            {
                cout<<"Player"<<players[i].name<<" take all "<<endl;
                ply_hash_greater.push_back(&players[i]);
            }
        }
    }
    else
    {
        //judge players card , create hash strength
        for(int i =0;i<PLAYERS;i++)
        {
            if(!players[i].isFold)
            {
                rank_t rnk = judge(players[i].hole_card);
                cout<<"Player"<<players[i].name<<" got "<<card5_name[rnk.type]<<endl;
                players[i].hash_val = hash_rank(rnk);
                ply_hash_greater.push_back(&players[i]);
            }
        }
        sort(ply_hash_greater.begin(), ply_hash_greater.end(), Player::hash_val_greater);

        //win pot field
        //players get main pot, side pot...etc
        //if tie , split pot

        for(unsigned int i =0;i<pots.size();i++)
            cout<<"pot "<<i<<" "<<pots[i]<<endl;
        for(unsigned int i =0;i<ply_hash_greater.size();i++)
            cout<<"Player "<<ply_hash_greater[i]->name<<" potID = "<<ply_hash_greater[i]->pot_ID<<endl;
    }
    //start from the highest pot ID
    //if players have pot ID >=current pot ID
    //he can join competition
    vector<Player*> join_comp;
    vector<Player*> split_player;
    while(!pots.empty())
    {
        split_player.clear();
        join_comp.clear();
        for(unsigned int i =0;i<ply_hash_greater.size();i++)
        {
            if(ply_hash_greater[i]->pot_ID >= (int)pots.size() -1)
            {
               join_comp.push_back(ply_hash_greater[i] );
            }
        }
        for(unsigned int i= 0;i<join_comp.size();i++)
        {
            if(join_comp[i]->hash_val==join_comp.back()->hash_val)
                split_player.push_back(join_comp[i]);
        }
        if(split_player.size() > 1)
            cout<<"split the pot\n";
        if(split_player.size() == 0)
        {
            cout<<" split_player.size() == 0\n";
            cout<<"dealer.pots.back() / split_player.size() error"<<endl;
            break;
        }

        int final_chip = pots.back() / split_player.size();
        //handling odd chip split
        //int this case ,the earliest player got odd chip
        int odd_chip = pots.back()-final_chip*split_player.size();
        int earliest_player = 100;
        int tmp_i;
        for(unsigned int i=0;i<split_player.size();i++)
        {
           split_player[i]->chip += final_chip;

            if(split_player[i]->position<earliest_player)
            {
                earliest_player = split_player[i]->position;
                tmp_i = i;
            }
        }
        split_player[tmp_i]->chip += odd_chip;

        pots.pop_back();
    }
}
void Dealer::start_betting(Player (&players)[PLAYERS])
{
    cout<<"pFlop"<<endl;
    betting(players);
    while(remain_players >1 && shared_cards.size()!=5)
    {
        call_to_size = 0;
        act_player = (btn_player+1)%PLAYERS;
        bet_leader = act_player;
        if(shared_cards.empty())
        {
           cout<<"Flop"<<endl;

           for(int i=0;i<3;i++)
            {
                print_card(*deck_it);
                shared_cards.push_back(*(deck_it++));
            }
        }
        else
        {

            if(shared_cards.size() == 3)
                cout<<"Turn"<<endl;
            else
                cout<<"River"<<endl;
            shared_cards.push_back(*(deck_it++));
            for(unsigned int i=0;i < shared_cards.size();i++)
                print_card(shared_cards[i]);
        }
        cout<<endl;
        betting(players);

    }

}
void Dealer::print_help()
{
    cout<<"How to play: "<<endl;
    cout<<"c = check/call"<<endl;
    cout<<"f = fold,"<<endl;
    cout<<"r(space)raise amount)=raise"<<endl;
}

