    card_t card;
    card.suit = 3;
    card.val = 9;
    dealer.shared_cards.push_back(card);
    card.suit = 2;
    card.val = 7;
    dealer.shared_cards.push_back(card);
    card.suit = 0;
    card.val = 14;
    dealer.shared_cards.push_back(card);
    card.suit = 3;
    card.val = 8;
    dealer.shared_cards.push_back(card);
    card.suit = 2;
    card.val = 11;
    dealer.shared_cards.push_back(card);
    dealer.print_public_cards();
    vector<card_t> cv;
    card.suit = 0;
    card.val = 6;
    cv.push_back(card);
    card.suit = 2;
    card.val = 6;
    cv.push_back(card);
    rank_t rnk = dealer.judge(cv);

    cout<<dealer.hash_rank(rnk)<<endl;

    cv.clear();

    card.suit = 0;
    card.val = 12;
    cv.push_back(card);
    card.suit = 3;
    card.val = 5;
    cv.push_back(card);
    rnk = dealer.judge(cv);
    cout<<dealer.hash_rank(rnk);