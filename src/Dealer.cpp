#include "Dealer.h"
#include "myDefine.h"

#include <iostream>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include <algorithm>
#include <vector>

using namespace std;

Dealer::Dealer(vector<Player> &players)
{
    ASSERT(((players).size()>0), "players size = 0");

    for(uint8_t i=0;i<players.size();i++)
        plys.push_back(&players[i]);

    set_blind(15,30);

 //random seed,without this will create same value
    srand(time(0));

    //create deck
    card_t c;
    for(uint8_t i = 2;i<=14;i++)
    {
        for(uint8_t j = 0;j<4;j++)
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
    for(uint8_t i=0;i<c.size();i++)
    {
        suit[c[i].suit]++;
    }
    //flush checker
    for(uint8_t i= 0;i<4;i++)
    {
        if(suit[i] >=5 )
        {
            cardsFlush.clear();
            if(hole[0].suit == i)
                flush = hole[0].val;
            if(hole[1].suit == i && hole[1].val > flush)
                flush = hole[1].val;
            for(uint8_t j=0;j<c.size();j++)
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
     for(uint8_t i=0;i<c.size();i++)
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

    for(uint8_t i=0;i<c.size();i++)
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
//if there are non equal bet (someone all in) , create side pot.
// dealer take away the chip from the smallest to biggest
// to build the side pot until no one have bet chips remain
void Dealer::collect_bets()
{
    vector<Player*> ply_bets_smaller;
    int orig_pot_ID =cur_pot_ID;
    //don't need to worry about fold player
    //it has been add to pots at
    for(ply_it = ply_nf.begin();ply_it!=ply_nf.end();ply_it++)
    {
        if((*ply_it)->bet != 0)
        {
            ply_bets_smaller.push_back(*ply_it);
            (*ply_it)->bet =0;
        }

    }
    if(ply_bets_smaller.size() == 0)
        return;

    sort(ply_bets_smaller.begin(),ply_bets_smaller.end(),Player::bet_smaller);

    //if there are non equal bet (someone all in) , create side pot
    int cur_scan_bet = ply_bets_smaller.back()->bet;

    for(ply_rit  =ply_bets_smaller.rbegin();
        ply_rit !=ply_bets_smaller.rend();ply_rit++)
    {
        if( (*ply_rit)->bet == cur_scan_bet)
        {
            (*ply_rit)->pot_ID =cur_pot_ID;
        }
        else
        {
            cur_scan_bet = (*ply_rit)->bet;
            cur_pot_ID++;
            (*ply_rit)->pot_ID =cur_pot_ID;
            cout<<"player"<<(*ply_rit)->name<<" Pot ID="<<cur_pot_ID<<endl;
            pots.push_back(0);
        }
    }
    // dealer take away the chip from the smallest to biggest
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

void Dealer::new_round()
{
    remove_0chip_players();
    if(plys.size() <= 1)
    {
        cout<<"player<=1 , table closed";
        return;
    }
    cout<<"====new round start====="<<endl;
    cout<<"small blind = "<<sb<<endl;
    cout<<"big blind = "<<bb<<endl;

    //init players position,0=SB,1=BB...
    ply_nf.clear();
    ply_nf = plys;
    for(uint8_t i=0;i<ply_nf.size();i++)
    {
        ply_nf[i]->position = i;
        ply_nf[i]->isFold = false;
        ply_nf[i]->init();
    }

    stage = PFLOP;
    call_to_size = bb;
    total_pot = 0;
    cur_pot_ID = 0;
    all_in_plys_cnt=0;

    act_ply = (ply_nf.size()<=3)?ply_nf.begin():ply_nf.begin()+2;
    bet_leader = *act_ply;

    pots.clear();
    pots.push_back(0);

    random_shuffle(deck.begin(),deck.end());
    deck_it = deck.begin();

    shared_cards.clear();
    //deal card
    for(ply_it =plys.begin();ply_it!=plys.end();ply_it++)
    {
        (*ply_it)->hole_card.clear();
        (*ply_it)->hole_card.push_back(*(deck_it++));
        (*ply_it)->hole_card.push_back(*(deck_it++));
        cout<<"Player "<<(*ply_it)->name<<" ";
        (*ply_it)->print_hole_cards();
        cout<<(*ply_it)->chip<<"$"<<endl;
    }
    //bet small blind
    total_pot += ply_nf[0]->blind_bet(this,sb);
    total_pot += ply_nf[1]->blind_bet(this,bb);
    start_betting();
    distribute_pot();
    passdown_button();
}

void Dealer::distribute_pot()
{
    vector<Player*> ply_hash_greater;
        ply_hash_greater.clear();

    //if one ply remain , don't need judge
    if(ply_nf.size() == 1)
    {
            cout<<"Player"<<ply_nf.front()->name<<" take all "<<endl;
            ply_hash_greater.push_back(ply_nf.front());
            ply_nf.front()->chip += pots.back();
            return;
    }
    else
    {
        //judge players card , create hash strength
        for(ply_it = ply_nf.begin();ply_it != ply_nf.end();ply_it++)
        {
            rank_t rnk = judge((*ply_it)->hole_card);

            cout<<"Player "<<(*ply_it)->name<<" [";
            (*ply_it)->print_hole_cards();
            cout<<"]"<<" got "<<card5_name[rnk.type]<<endl;

            (*ply_it)->hash_val = hash_rank(rnk);
            ply_hash_greater.push_back((*ply_it));
        }
        sort(ply_hash_greater.begin(), ply_hash_greater.end(), Player::hash_val_greater);

        //win pot field
        //players get main pot, side pot...etc
        //if tie , split pot
        for(uint8_t i =0;i<ply_hash_greater.size();i++)
            cout<<"Player "<<ply_hash_greater[i]->name<<" potID = "<<ply_hash_greater[i]->pot_ID<<endl;
    }
    //uint8_t is not able to print by cout
    for(uint16_t i =0;i<pots.size();i++)
        cout<<"pot "<<i<<":"<<pots[i]<<endl;

    //start from the highest pot ID
    //if players have pot ID >=current pot ID
    //he can join competition
    vector<Player*> join_comp;//player's pot ID >=current pot ID
    vector<Player*> split_player;//players with same card strength
    while(!pots.empty())
    {
        split_player.clear();
        join_comp.clear();
        for(uint8_t i =0;i<ply_hash_greater.size();i++)
        {
            if(ply_hash_greater[i]->pot_ID >= (int)pots.size() -1)
               join_comp.push_back(ply_hash_greater[i] );
        }
        for(uint8_t i= 0;i<join_comp.size();i++)
        {
            if(join_comp[i]->hash_val==join_comp.back()->hash_val)
                split_player.push_back(join_comp[i]);
        }
        if(split_player.size() ==1)
        {
            split_player.front()->chip+=pots.back();
            pots.pop_back();
            continue;
        }
        else if(split_player.size() > 1)
            cout<<"split the pot\n";
        else if(split_player.size() == 0)
        {
            cout<<" split_player.size() == 0\n";
            cout<<" this should be at least 1\n";
            cout<<"dealer.pots.back() / split_player.size() error"<<endl;
            break;
        }
        //=========code below will deal with split pot case======

        int final_chip = pots.back() / split_player.size();
        //handling odd chip split
        //in this case ,the earliest player got odd chip
        int odd_chip = pots.back()-final_chip*split_player.size();
        int earliest_player = 100;
        int tmp_i;
        for(uint8_t i=0;i<split_player.size();i++)
        {
           split_player[i]->chip += final_chip;

            if(split_player[i]->position < earliest_player)
            {
                earliest_player = split_player[i]->position;
                tmp_i = i;
            }
        }
        split_player[tmp_i]->chip += odd_chip;

        pots.pop_back();
    }
}
void Dealer::wake_up(vector<Player*>::iterator act)
{
    cout<<"Player "<<(*act)->name<<" turn"<<endl;
    cout<<"Pot:"<<total_pot<<" ,Your chip: "<<(*act)->chip<<endl;
    total_pot += (*act)->action(this);
    cout<<endl;
}
void Dealer::start_betting()
{
    do
    {
        if(stage==PFLOP)
        {
            cout<<"pFlop"<<endl;
        }
        else if(stage == FLOP)
        {
            cout<<"Flop"<<endl;
            for(int i=0;i<3;i++)
                shared_cards.push_back(*(deck_it++));
        }
        else
        {
            if(stage == TURN)
                cout<<"Turn"<<endl;
            else
                cout<<"River"<<endl;
            shared_cards.push_back(*(deck_it++));
        }
        print_public_cards();
        cout<<endl;
        while(ply_nf.size() > 1)
        {
            cout<<"=========="<<all_in_plys_cnt<<" "<<ply_nf.size()<<endl;
            if((*act_ply)->chip!=0 &&all_in_plys_cnt != ply_nf.size()-1)
                wake_up(act_ply);
            if((*act_ply)->isFold)
            {
                if(*act_ply == bet_leader)
                {
                    if((act_ply+1) != ply_nf.end())
                        bet_leader = *(act_ply+1);
                    else
                        bet_leader = ply_nf.front();
                }
                //add dead money to the pot
                pots.back()+=(*act_ply)->bet;
                (*act_ply)->bet = 0;
                //if vector_it erase last element,
                //iterator will auto point to the first element
                ply_nf.erase(act_ply);
            }
            else
            {

                next_ply();
                if(*act_ply == bet_leader )
                    break;
            }

        }
        collect_bets();

        call_to_size = 0;
        act_ply = ply_nf.begin();
        bet_leader = *act_ply;
        stage++;
        cout<<endl;
    } while(shared_cards.size()!=5);

}
void Dealer::print_public_cards()
{
    for(uint8_t i=0;i < shared_cards.size();i++)
        print_card(shared_cards[i]);

}
void Dealer::print_help()
{
    cout<<"How to play: "<<endl;
    cout<<"c = check/call"<<endl;
    cout<<"f = fold,"<<endl;
    cout<<"r(space)raise amount)=raise"<<endl;
}
