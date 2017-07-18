#include "Dealer.h"
#include "myDefine.h"
#include "Useful_func.h"

#include <iostream>
#include <algorithm> //swap


#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include <algorithm>
#include <vector>
#include <conio.h>

using namespace std;

Dealer::Dealer(vector<Player*> players)
{
    ASSERT((players.size()>0), "players size = 0");
    round_cnt = 1;
    plys = players;
//    for(uint8_t i=0;i<players.size();i++)
//        plys.push_back(&players[i]);

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
    c.insert( c.end(), shared_cards.begin(), shared_cards.end() );

    ASSERT(c.size() >= 5, "judge fail,except at least 5 cards but card size = " << c.size());

    sort(c.begin(),c.end(),Greater());

    int suit_cnt[4] = {0};
    //(straight)flush checker
    //it's impossible for seven card to form flush and full house at the same time

    //accumulate cards with same suit
    for(uint8_t i=0;i<c.size();i++)
        suit_cnt[c[i].suit]++;
    for(uint8_t i= 0;i<4;i++)
    {
        //if one of four suits count > 5,flush exist
        if(suit_cnt[i] >=5 )
        {
            vector<card_t> cardsFlush;
            for(uint8_t j=0;j<c.size();j++)
            {
                if(c[j].suit == i)
                    cardsFlush.push_back(c[i]);
            }
            //=========straight flush checker================
            int top_card = check_straight(cardsFlush);
            if(top_card > 0) //straight flush
            {
                rank.type = 9;
                rank.kicker.push_back(top_card);
                return rank;
            }//===============================================
            else
            {
                rank.type = 6;
                //card value has been sorted greater
                //kicker = last five elements
                for(uint8_t j =-1;j>=-5;j--)
                    rank.kicker.push_back(cardsFlush.end()[j].val);
                return rank;
            }
        }
    }

    //check pair,set,quad
    int same;
    vector<int> pairs;
    int SET = 0;
    int cmp_tar = c[0].val; //compare target

     for(uint8_t i=1;i<c.size();i++)
    {
        same = 0;
        while(c[i].val == cmp_tar && i<(c.size()))
        {
            same++;
            i++;
        }
        switch(same)
        {
            case 1:
                pairs.push_back(cmp_tar);
            break;
            case 2:
                    SET = cmp_tar; //it may be two set in 7 cards, but again,
                                   //card val is sorted in increase order
                                   //so don't worry data overwrite
            break;
            case 3:
                rank.type = 8; //quad
                rank.kicker.push_back(cmp_tar);
                if(c.back().val == cmp_tar)
                    rank.kicker.push_back(c.end()[-5].val);
                else
                    rank.kicker.push_back(c.back().val);
                return rank;
            break;
        }
        cmp_tar = c[i].val;
    }

    //full house checker
    if(pairs.size() !=0 && SET != 0)
    {
        rank.type = 7;
        rank.kicker.push_back(SET);
        rank.kicker.push_back(pairs.back());
        return rank;
    }

    int straight = check_straight(c);
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
    for(ply_it = ply_nf_seq.begin();ply_it!=ply_nf_seq.end();ply_it++)
    {
        if((*ply_it)->bet != 0)
        {
            ply_bets_smaller.push_back(*ply_it);
           // (*ply_it)->bet =0; //don't write this
                                //dealer will take away bet below
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

bool Dealer::game_cycle()
{
    //ensure valid players >=2
    remove_0chip_players();
    if(plys.size() <= 1)
    {
        cout<<"player<=1 , table closed\n";
        return false;
    }
    //Dealer init
    stage = PFLOP;
    call_to_size = bb;
    total_pot = 0;
    cur_pot_ID = 0;
    all_in_plys_cnt=0;
    pots.clear();
    pots.push_back(0);
    shared_cards.clear();

    //init players position,ply_nf_seq[x] x0=SB,x1=BB...
    //blind bet
    ply_nf_seq.clear();
    ply_nf_seq = plys;
    if(ply_nf_seq.size()!=2)
    {
        for(uint8_t pos=0;pos<ply_nf_seq.size();pos++)
            ply_nf_seq[pos]->init(pos);
        //blind
        total_pot += ply_nf_seq[0]->blind_bet(this,sb);
        total_pot += ply_nf_seq[1]->blind_bet(this,bb);
    }
    else
    {
        ply_nf_seq[0]->init(1); //big blind act first
        ply_nf_seq[1]->init(0);
        //blind
        total_pot += ply_nf_seq[0]->blind_bet(this,bb);
        total_pot += ply_nf_seq[1]->blind_bet(this,sb);
    }
    act_ply = (ply_nf_seq.size()<=3)?ply_nf_seq.begin():ply_nf_seq.begin()+2;
    bet_leader = *act_ply;
    //deal card
    random_shuffle(deck.begin(),deck.end());
    deck_it = deck.begin();
    for(ply_it =plys.begin();ply_it!=plys.end();ply_it++)
    {
        (*ply_it)->hole_card.clear();
        (*ply_it)->hole_card.push_back(*(deck_it++));
        (*ply_it)->hole_card.push_back(*(deck_it++));
        if((*ply_it)->hole_card[0].val<(*ply_it)->hole_card[1].val)
            swap((*ply_it)->hole_card[0],(*ply_it)->hole_card[1]);
    }
    start_betting();

    //player stop acting,dealer send chips to winning player.
    system("cls");
    distribute_pot();
    print_round_info();
    system("pause");
    //pass down button to next player
    position_shift();
    round_cnt++;
}

void Dealer::distribute_pot()
{
    vector<Player*> ply_hash_greater;
    ply_hash_greater.clear();

    //if one ply remain , don't need judge
    if(ply_nf_seq.size() == 1)
    {
            cout<<"Player"<<ply_nf_seq.front()->name<<" takes "<< pots.back()<<"$"<<endl;
            ply_nf_seq.front()->chip += pots.back();
            return;
    }
    else
    {
        //judgejudge players card , create hash strength
        for(ply_it = ply_nf_seq.begin();ply_it != ply_nf_seq.end();ply_it++)
        {
            rank_t rnk = judge((*ply_it)->hole_card);
            (*ply_it)->hash_val = hash_rank(rnk);

            cout<<"Player "<<(*ply_it)->name<<" ";
            (*ply_it)->print_hole_cards();
            cout<<" got "<<card5_name[rnk.type]<<"\t"<<"hash code:"<<(*ply_it)->hash_val<<endl;

            ply_hash_greater.push_back((*ply_it));
        }
        sort(ply_hash_greater.begin(), ply_hash_greater.end(), Player::hash_val_greater);

        //win pot field
        //players get main pot, side pot...etc
        //if tie , split pot
        /*
        for(uint8_t i =0;i<ply_hash_greater.size();i++)
            cout<<"Player "<<ply_hash_greater[i]->name<<" potID = "<<ply_hash_greater[i]->pot_ID<<endl;
        */
    }
    //uint8_t is not able to print by cout
    /*
    for(uint16_t i =0;i<pots.size();i++)
        cout<<"pot "<<i<<":"<<pots[i]<<endl;
    */
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
            cout<<"Player"<<split_player.front()->name<<" takes "<<pots.back()<<"$"<<endl;
            split_player.front()->chip+=pots.back();
            pots.pop_back();
            continue;
        }
        else if(split_player.size() == 0)
        {
            cout<<" split_player.size() == 0\n";
            cout<<" this should be at least 1\n";
            cout<<"dealer.pots.back() / split_player.size() error"<<endl;
            break;
        }
        else
            split_pot(split_player);

    }
}

void Dealer::wake_up(vector<Player*>::iterator act)
{
    system("cls");
    print_round_info();
    cout<<"Player "<<(*act)->name<<"'s turn"<<endl;
    cout<<"Pot:"<<total_pot<<" ,Your chip: "<<(*act)->chip<<endl;
    (*act)->print_hole_cards();
    cout<<endl;
    if((*act)->is_AI)
    {
        act_t a = (*act)->thinking();
        total_pot += (*act)->action(this,a);
    }
    else
        total_pot += (*act)->action(this);
    cout<<endl;
}
void Dealer::start_betting()
{
    do
    {
        if(ply_nf_seq.size()-all_in_plys_cnt == 1)
        {
            while(shared_cards.size()!=5)
                shared_cards.push_back(*(deck_it++));
            break;
        }
        if(stage == FLOP)
        {
            for(int i=0;i<3;i++)
                shared_cards.push_back(*(deck_it++));
        }
        else if(stage == TURN || stage ==RIVER)
            shared_cards.push_back(*(deck_it++));

        while(ply_nf_seq.size() > 1)
        {
            if((*act_ply)->chip!=0 )
                wake_up(act_ply);
            if((*act_ply)->isFold)
            {
                if(*act_ply == bet_leader)
                {
                    if((act_ply+1) != ply_nf_seq.end())
                        bet_leader = *(act_ply+1);
                    else
                        bet_leader = ply_nf_seq.front();
                }
                //add dead money to the pot
                pots.back()+=(*act_ply)->bet;
                (*act_ply)->bet = 0;
                ply_nf_seq.erase(act_ply);
                if(act_ply==ply_nf_seq.end())
                    act_ply = ply_nf_seq.begin();
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
        act_ply = ply_nf_seq.begin();
        bet_leader = *act_ply;
        stage = (stage<3)?stage+1:stage;
        cout<<endl;
    } while(shared_cards.size()!=5);

}
void Dealer::print_round_info()
{
    cout<<"====round "<<round_cnt<<"====="<<endl;
    cout<<STAGE_STR[stage];
    print_public_cards();
    cout<<endl;
    cout<<"SB/BB :"<<sb<<"/"<<bb<<endl;
    for(ply_it =plys.begin();ply_it!=plys.end();ply_it++)
    {
        if((*ply_it)==(*act_ply))
        {
            SetColor(YELLOW);
            (*ply_it)->print_info();
            SetColor(WHITE);
        }
        else
            (*ply_it)->print_info();

    }

}
void Dealer::print_public_cards()
{
    if(shared_cards.size() == 0)
        return;
    cout<<" [ ";
    for(uint8_t i=0;i < shared_cards.size();i++)
        print_card(shared_cards[i]);
    cout<<"]";
}
void Dealer::print_help()
{
    cout<<"How to play: "<<endl;
    cout<<"f = fold,"<<endl;
    cout<<"c = check/call"<<endl;
    cout<<"r(space)raise amount)=raise"<<endl;
    cout<<"a = all in"<<endl;

}
