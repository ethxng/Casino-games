#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <stack>
#include <unordered_set>
#include <set>
#include <unistd.h>
#include <ctime>
#include <numeric>

using namespace std;
//all below works for ante, pair, and six card bonus
bool royal_flush(vector<string> cards);
bool straight_flush(vector<string> cards);
bool three_of_a_kind(vector<string> cards);
bool four_of_a_kind(vector<string> cards);
bool straight(vector<string> cards);
bool flush(vector<string> cards);
bool full_house(vector<string> cards);
bool single_pair(vector<string> cards);
bool qualify(vector<string> cards);
int pair_pay(int asset, int amount, vector<string> cards);
int ante_pay(int asset, int amount, vector<string> cards);
int six_card_bonus(int asset, int amount, vector<string> cards);
int tie_check(vector<string> player, vector<string> dealer);
int head_to_head(vector<string> cards);
void random_generator();
void show_dealer_card();
const string suit[4] = {"\u2665", "\u2660", "\u2663", "\u2666"}; //heart, spade, club, diamond respectively
const string ranking[13] = {"2", "3", "4","5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
vector<string> player_drawing, dealer_drawing;
//REMEMBER TO CLEAR VECTOR BEFORE RESTART THE GAME
//REMEMBER: FOR SIX CARD BONUS CHECK, MERGE VECTOR BEFORE CHECKING
int main()
{
    int asset = 100, ante_bet, pair_bet, six_card_bonus_bet, play_bet; 
    cout << "Welcome to the world of Three Card Poker: Created by Ethan Nguyen." << endl;
    sleep(2);
    cout << "You are given $" << asset << ". Use wisely!" << endl;
    while (asset > 0)
    {
        cout << "You own $" << asset << ". How much would you like to ante? ";
        cin >> ante_bet; 
        play_bet = ante_bet;
        cout << "How much would you like to bet on pair bonus? (your pair bonus has be less than your ante!) ";
        cin >> pair_bet;
        while (pair_bet >= ante_bet) //safeguard
        {
            cout << "Your pair bet has to be less than your ante! Please enter again: ";
            cin >> pair_bet;
        }
        cout << "How much would you like to bet on six card bonus? (min: $5 max: $50) ";
        cin >> six_card_bonus_bet;
        while (six_card_bonus_bet < 5 || six_card_bonus_bet > 50) //safeguard
        {
            cout << "Rememeber! You have to bet at least $5 or at most $50! Please enter again: ";
            cin >> six_card_bonus_bet;
        }
        random_generator(); // draw cards and show player's card
        cout << "After viewing your hand, would you like to fold? Enter Y for yes or N for no: ";
        char choice;
        cin >> choice;
        if (toupper(choice) == 'Y') // if player folds
        {
            sleep(5);
            asset -= (play_bet + ante_bet + pair_bet);
            if (six_card_bonus_bet > 0)
            {
                show_dealer_card();
                sleep(3);
                player_drawing.insert(player_drawing.end(), dealer_drawing.begin(), dealer_drawing.end());
                asset = six_card_bonus(asset, six_card_bonus_bet, player_drawing);
            }
        }
        else if(toupper(choice) == 'N') // if player doesn't fold
        {
            sleep(3);
            show_dealer_card();
            sleep(3);
            asset = pair_pay(asset, pair_bet, player_drawing);
            asset = ante_pay(asset, ante_bet, player_drawing);
            if (qualify(dealer_drawing)) // if dealer qualify
            {
                int player_check = head_to_head(player_drawing);
                int dealer_check = head_to_head(dealer_drawing);
                if (player_check == dealer_check) //same type(straight, flush, etc)
                {
                    int tie = tie_check(player_drawing, dealer_drawing);
                    if (tie == 2) // player wins
                    {
                        asset += (ante_bet + play_bet);
                    }
                    else if (tie == 1)
                    {
                        asset -= (ante_bet + play_bet);
                    }
                    // the last condition is a push, which means there is no need to write

                    //if player bet six card bonus
                    if (six_card_bonus_bet > 0)
                    {
                        sleep(3);
                        player_drawing.insert(player_drawing.end(), dealer_drawing.begin(), dealer_drawing.end());
                        asset = six_card_bonus(asset, six_card_bonus_bet, player_drawing);
                    }
                }
                else if (player_check < dealer_check) // player lost
                {
                    sleep(3);
                    asset -= (ante_bet + play_bet);
                    if (six_card_bonus_bet > 0)
                    {
                        sleep(3);
                        player_drawing.insert(player_drawing.end(), dealer_drawing.begin(), dealer_drawing.end());
                        asset = six_card_bonus(asset, six_card_bonus_bet, player_drawing);
                    }
                }
                else if (player_check > dealer_check) // player wins
                {
                    sleep(3);
                    asset += (ante_bet + play_bet);
                    if (six_card_bonus_bet > 0)
                    {
                        sleep(3);
                        player_drawing.insert(player_drawing.end(), dealer_drawing.begin(), dealer_drawing.end());
                        asset = six_card_bonus(asset, six_card_bonus_bet, player_drawing);
                    }
                }
            }
            else if (qualify(dealer_drawing) == false) // if dealer not qualify
            {
                sleep(3);
                cout << "Dealer doesn't qualify." << endl;
                asset += (ante_bet + play_bet);
                if (six_card_bonus_bet > 0)
                {
                    sleep(3);
                    player_drawing.insert(player_drawing.end(), dealer_drawing.begin(), dealer_drawing.end());
                    asset = six_card_bonus(asset, six_card_bonus_bet, player_drawing);
                }
            }
            
        }
        else if (toupper(choice) == 'Q')
        {
            cout << "Goodbye. Hope to see you again! Although, remember that all of your progress is lost." << endl;
            return 0;
        }
        player_drawing.clear(); // clear vec before starting game
        dealer_drawing.clear();
    }
    return 0;
}

bool royal_flush(vector<string> cards) 
{
    vector<string> testing;
    for (int a = 0; a < cards.size(); a+=2)
    {
        testing.push_back(cards[a]);
    }
    sort(testing.begin(), testing.end());
    if (cards.size() == 6) // for player only
    {
        if (testing[0] == "A" && testing[2] == "K" && testing[4] == "Q") 
        {
            for (int i = 1; i < cards.size() - 2; i+=2)
            {
                if (cards[i] != cards[i + 2])
                {
                    return false;
                }
            }
            return true;
        }
    }

    // for both player and dealer comparision at six card bonus *NOTE: RARE*
    vector<int> signs(4,0);
    int count = 0;
    for (int i = 0; i < cards.size() - 1; i+=2)
    {
        if (cards[i] == "10" || cards[i] == "J" || cards[i] == "Q" || cards[i] == "K" || cards[i] == "A")
        {
            count++; // if count > 4 good
            if (cards[i + 1] == suit[0])
            {
                signs[0]++;
            }
            else if (cards[i+1] == suit[1])
            {
                signs[1]++;
            }
            else if (cards[i+1] == suit[2])
            {
                signs[2]++;
            }
            else
            {
                signs[3]++;
            }
        }
    }
    if (count > 4 && *max_element(signs.begin(), signs.end()) > 4)
    {
        return true;
    }
    return false;
}
bool straight_flush(vector<string> cards)
{
    vector<int> test;
    for (int j = 0; j < cards.size(); j+=2)
    {
        test.push_back(distance(ranking, find(ranking, ranking + 13, cards[j])));
    }
    sort(test.begin(), test.end()); 
    if (cards.size() == 6) // for player only
    {
        for (int i = 0; i < test.size() - 1; i++) // check for straight
        {
            if (test[i] - test[i + 1] != -1)
            {
                return false;
            }
        }
        for (int a = 1; a < cards.size() - 2; a+=2) // check for same suit
        {
            if (cards[a] != cards[a + 2])
            {
                return false;
            }
        }
        return true;
    }
    else // for player and dealer at six card bonus FIX
    {
        multimap<string, string> compare;
        vector<string> element;
        for (int i = 0; i < cards.size() - 1; i+=2) // remove duplicate in multimap
        {
            compare.insert(pair<string, string> (cards[i], cards[i+1]));
        }

        for (multimap<string, string>::iterator it = compare.begin(); it != compare.end(); it++)
        {
            if (it->first == "A")
            {
                continue;
            }
            else
            {
                element.push_back(it->first);
                element.push_back(it->second);
            }
        }

        //CORNER CASE
        int counter = 0;
        vector<int> signs(4,0);
        for (int a = 0; a < element.size(); a+=2) // corner case where K and Q are present
        {
            if (element[a] == "9" || element[a] == "10" || element[a]== "J" || element[a] == "Q" || element[a] == "K")
            {
                counter++;
                if (cards[a + 1] == suit[0])
                {
                    signs[0]++;
                }
                else if (cards[a+1] == suit[1])
                {
                    signs[1]++;
                }
                else if (cards[a+1] == suit[2])
                {
                    signs[2]++;
                }
                else
                {
                    signs[3]++;
                }
            }
        }
        if (counter > 4 && *max_element(signs.begin(), signs.end()) > 4)
        {
            return true;
        }

        for (int i = 0; i < element.size() - 8; i+=2) // will return false if NEITHER straight NOR flush
        {
            int count = 0, increment = -1;
            for (int j = i + 2; j < element.size(); j+=2)
            {
                int num1 = distance(ranking, find(ranking, ranking + 13, element[i])); 
                int num2 = distance(ranking, find(ranking, ranking + 13, element[j]));
                if (num1 - num2 == increment && element[i+1] == element[j+1])
                {
                    increment--;
                    count++;
                }
            }
            if (count > 3)
            {
                return true;
            }
        }
    return false;
    }
}

bool three_of_a_kind(vector<string> cards)
{
    if (cards.size() == 6)// for player only
    {
        vector<int> test; 
        for (int j = 0; j < cards.size(); j+=2)
        {
            test.push_back(distance(ranking, find(ranking, ranking + 13, cards[j])));
        }
        sort(test.begin(), test.end());
        for (int i = 0; i < test.size() - 2; i++)
        {
            if (test[i] == test[i + 1] && test[i] == test[i+2])
            {
                return true;
            }
        }
    }

    //for player and dealer at six card bonus
    vector<int> player, dealer, check; 
    for (int i = 0; i < cards.size()/2; i+=2)
    {
        int player_num = distance(ranking, find(ranking, ranking+12, cards[i]));
        int dealer_num = distance(ranking, find(ranking, ranking + 13, cards[i+6]));
        player.push_back(player_num);
        dealer.push_back(dealer_num);
    }
    check = player;
    vector<int>::iterator it = unique(check.begin(), check.end());
    check.resize(distance(check.begin(), it));
    for (int i = 0; i < check.size(); i++)
    {
        int player_count = count(player.begin(), player.end(), check[i]);
        int dealer_count = count(dealer.begin(), dealer.end(), check[i]);
        if (player_count > 0 && dealer_count > 0 && player_count + dealer_count == 3)
        {
            return true;
        }
    }
    return false;
}

bool four_of_a_kind(vector<string> cards) // only at six card bonus for both player and dealer
{
    vector<int> player, dealer, check; 
    for (int i = 0; i < cards.size()/2; i+=2) // divide by 2 to take player's card only
    {
        int player_num = distance(ranking, find(ranking, ranking+12, cards[i]));
        int dealer_num = distance(ranking, find(ranking, ranking + 13, cards[i+6]));
        player.push_back(player_num);
        dealer.push_back(dealer_num);
    }
    check = player;
    vector<int>::iterator it = unique(check.begin(), check.end());
    check.resize(distance(check.begin(), it));
    for (int i = 0; i < check.size(); i++)
    {
        int player_count = count(player.begin(), player.end(), check[i]);
        int dealer_count = count(dealer.begin(), dealer.end(), check[i]);
        if (player_count > 0 && dealer_count > 0 && player_count + dealer_count == 4)
        {
            return true;
        }
    }
    return false;
}

bool straight(vector<string> cards)
{
    if (cards.size() == 6) // for player only at pair and ante
    {
        vector<int> test;
        for (int j = 0; j < cards.size(); j+=2)
        {
            test.push_back(distance(ranking, find(ranking, ranking + 13, cards[j])));
        }
        sort(test.begin(), test.end());
        if (cards.size() == 6) // for player only
        {
            for (int i = 0; i < test.size() - 1; i++)
            {
                if (test[i] - test[i + 1] != -1)
                {
                    return false;
                }
            }
            return true;
        }
    }
    else // for player and dealer at six card bonus
    {
        string arr[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "A", "J", "K", "Q"};
        vector<string> element;
        for (int i = 0; i < cards.size() - 1; i+=2)
        {
            element.push_back(cards[i]);
        }
        sort(element.begin(), element.end());
        for (int i = 0; i < element.size() - 4; i++) // will return false if not straight
        {
            int count = 0, increment = -1;
            for (int j = i + 1; j < element.size(); j++)
            {
                int num1 = distance(arr, find(arr, arr + 13, element[i])); 
                int num2 = distance(arr, find(arr, arr + 13, element[j]));
                if (num1 - num2 == increment)
                {
                    increment--;
                    count++;
                }
            }
            if (count > 3)
            {
                return true;
            }
        }
    }
    return false;
}

bool flush(vector<string> cards)
{
    if (cards.size() == 6) // for player only at ante and pair
    {
        if (cards[1] == cards[3] && cards[3] == cards[5])
        {
            return true;
        }
        return false;
    }
    else // for both player and dealer at six card bonus
    {
        for (int i = 3; i < cards.size(); i+=2)
        {
            if (cards[1] != cards[i])
            {
                return false;
            }
        }
    }
    return true;
}

bool single_pair(vector<string> cards) // for player only
{
    //if num1 == num2 || num2 == num3 || num1 == num3, use ranking to convert
    int num1 = distance(ranking, find(ranking, ranking + 13, cards[0]));
    int num2 = distance(ranking, find(ranking, ranking + 13, cards[2]));
    int num3 = distance(ranking, find(ranking, ranking + 13, cards[4]));
    if (num1 == num2 || num2 == num3 || num1 == num3)
    {
        return true;
    }
    return false;
}

bool full_house(vector<string> cards) // only for six card bonus for both dealer and player
{ //less than four of a kind
    vector<int> player, dealer, check, combine; 
    for (int i = 0; i < cards.size()/2; i+=2)
    {
        int player_num = distance(ranking, find(ranking, ranking+13, cards[i]));
        int dealer_num = distance(ranking, find(ranking, ranking + 13, cards[i+6]));
        player.push_back(player_num);
        dealer.push_back(dealer_num);
    }
    combine.reserve(dealer.size() + player.size());
    combine.insert(combine.end(), player.begin(), player.end());
    combine.insert(combine.end(), dealer.begin(), dealer.end());
    check = player; // create a unique veck and use that to check it against dealer's and player's
    vector<int>::iterator it = unique(check.begin(), check.end());
    check.resize(distance(check.begin(), it));
    for (int i = 0; i < check.size(); i++)
    {
        int player_count = count(player.begin(), player.end(), check[i]);
        int dealer_count = count(dealer.begin(), dealer.end(), check[i]);
        if (player_count + dealer_count == 3)
        {
            combine.erase(remove(combine.begin(), combine.end(), check[i]), combine.end());
            if (combine[0] == combine[1] || combine[1] == combine[2] || combine[0] == combine[2]) //check fo pair
            {
                return true;
            }
        }
    }
    return false;
}

int pair_pay(int asset, int amount, vector<string> cards) // n is amount bet, cards is the payment
{ // for player only
    if (royal_flush(cards))
    {
        return asset + (amount * 100);
    }
    else if (straight_flush(cards))
    {
        return asset+ (amount * 40);
    }
    else if (three_of_a_kind(cards))
    {
        return asset + (amount * 30);
    }
    else if (straight(cards))
    {
        return asset + (amount * 6);
    }
    else if (flush(cards))
    {
        return asset + (amount * 3);
    }
    else if (single_pair(cards))
    {
        return asset + amount;
    }
    return asset - amount; // take money away if not match any above
}

int ante_pay(int asset, int amount, vector<string> cards)
{ // for player only
    if (straight_flush(cards))
    {
        return asset + (amount * 6);
    }
    else if (three_of_a_kind(cards))
    {
        return asset + (amount * 5);
    }
    else if (straight(cards))
    {
        return asset + (amount * 2);
    }
    else if (flush(cards) == true && single_pair(cards) == true)
    {
        return asset + amount;
    }
    return asset; // not take money away bc it still depends later on when comparing to dealer
}

int six_card_bonus(int asset, int amount, vector<string> cards)
{ // pay six card bonus
    if (royal_flush(cards))
    {
        return asset + (amount * 1000);
    }
    else if (four_of_a_kind(cards))
    {
        return asset + (amount * 50);
    }
    else if (straight_flush(cards))
    {
        return asset + (amount * 50);
    }
    else if (full_house(cards))
    {
        return asset + (amount * 20);
    }
    else if (flush(cards))
    {
        return asset + (amount * 15);
    }
    else if (straight(cards))
    {
        return asset + (amount * 10);
    }
    else if (three_of_a_kind(cards))
    {
        return asset + (amount * 10);
    }
    return asset - amount;
}

// function where it takes in tie result and compare high card to hgih card if dealer qualify
int tie_check(vector<string> player, vector<string> dealer) // only compare suit, ranking doesn't matter here
{// return 2 if player win, 1 if dealer win, 0 if really tie
    vector<int> player_cards, dealer_cards;
    player_cards.push_back(distance(ranking, find(ranking, ranking + 13, player[0])));
    player_cards.push_back(distance(ranking, find(ranking, ranking + 13, player[2])));
    player_cards.push_back(distance(ranking, find(ranking, ranking + 13, player[4])));
    dealer_cards.push_back(distance(ranking, find(ranking, ranking + 13, dealer[0])));
    dealer_cards.push_back(distance(ranking, find(ranking, ranking + 13, dealer[2])));
    dealer_cards.push_back(distance(ranking, find(ranking, ranking + 13, dealer[4])));
    if (*max_element(player_cards.begin(), player_cards.end()) > *max_element(dealer_cards.begin(), dealer_cards.end()))
    { // player win
        return 2;
    }
    else if (*max_element(player_cards.begin(), player_cards.end()) < *max_element(dealer_cards.begin(), dealer_cards.end()))
    {//dealer win
        return 1;
    }
    return 0; // both tie
}

//check to see if dealer qualify before proceed with the game
bool qualify(vector<string> cards) // for dealer
{
    if (royal_flush(cards) || straight_flush(cards) || three_of_a_kind(cards) || straight(cards) || flush(cards) || single_pair(cards))
    {
        return true;
    }
    else // qualify for high pair
    {
        for (int i = 0; i < cards.size(); i+=2)
        {
            int check = distance(ranking, find(ranking, ranking + 13, cards[i]));
            if (check > 9) // score for ranking[] in order to qualify
            {
                return true; // qualify for high pair
            }
        }
    }
    return false; // not qualify end game (pay if bet six card bonus)
}

int head_to_head(vector<string> cards)
{// compare separately, this is actual playing part, here qualify already, no need to check
    if (royal_flush(cards))
    {
        return 6;
    }
    else if (straight_flush(cards))
    {
        return 5;
    }
    else if (three_of_a_kind(cards))
    {
        return 4;
    }
    else if (straight(cards))
    {
        return 3;
    }
    else if (flush(cards))
    {
        return 2;
    }
    else if (single_pair(cards))
    {
        return 1;
    }
    return 0; // has high card
}   

void random_generator()
{
    vector<string> store;
    int i = 0;
    while (i < 3) 
    {
        string player_score = ranking[rand() % 13];
        string player_suit = suit[rand() % 4];
        string dealer_score = ranking[rand() % 13];
        string dealer_suit = suit[rand() % 4]; // below condition help prevent the impossible where there are 2 same cards of the same suit
        if (count(store.begin(), store.end(), player_score) < 4 && count(store.begin(), store.end(), dealer_score) < 4)
        {
            store.push_back(player_score);
            store.push_back(dealer_score);
            player_drawing.push_back(player_score);
            player_drawing.push_back(player_suit);
            dealer_drawing.push_back(dealer_score);
            dealer_drawing.push_back(dealer_suit);
            i++;
        }
    }
    for (int i = 0; i < player_drawing.size(); i++) // print out for player ONLY, no dealer showing
    {
        cout << player_drawing[i];
    }
    cout << endl;
}

void show_dealer_card()
{
    cout << "Dealer's hands: ";
    for (int j = 0; j < dealer_drawing.size(); j++)
    {
        cout << dealer_drawing[j];
    }
    cout << endl;
}