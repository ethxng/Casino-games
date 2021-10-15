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
bool player_straight_flush(vector<string> cards);
bool player_four_of_a_kind(vector<string> cards);
bool player_three_of_a_kind(vector<string> cards);
bool player_flush(vector<string> cards);
bool player_straight(vector<string> cards);
bool player_two_pair(vector<string> cards);
int head_to_head(vector<string> cards);
bool player_one_pair(vector<string> cards);
void dealer_remover();
void random_generator();
int ante_pay(int asset, int amount, vector<string> cards);
int ace_up(int asset, int amount, vector<string> cards);
int tie_check(vector<string> player, vector<string> dealer);
const string suit[4] = {"\u2665", "\u2660", "\u2663", "\u2666"}; //heart, spade, club, diamond respectively
const string ranking[13] = {"2", "3", "4","5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
vector<string> dealer, player;

int main()
{
    srand(time(0)); // generate rand num every game
    int asset = 100, ante_bet, play_bet, ace_up_bet;
    cout << "Welcome to the world of Four Card Poker: Created by Ethan Nguyen." << endl;
    sleep(1);
    cout << "RULES: PLAY BET IS ALWAYS THREE TIMES ANTE BET." << endl;
    cout << "You are given $100. Use wisely!" << endl;
    while (asset > 0)
    {
        cout << "You own $" << asset << ". How much would you like to ante? (min: $5) ";
        cin >> ante_bet;
        sleep(1);
        while (ante_bet < 5 || ante_bet + (ante_bet * 3) > asset)
        {
            cout << "You must bet at least $5! Please try again: ";
            cin >> ante_bet;
        }
        cout << "Your play bet will be $" << ante_bet * 3 << endl;
        play_bet = ante_bet;
        sleep(1);
        cout << "How much would you like to bet on Ace Up Bonus? ";
        cin >> ace_up_bet;
        while (ace_up_bet + ante_bet + play_bet > asset || ace_up_bet < 5)
        {
            cout << "You've exceeded the amount you own. Please try again: ";
            cin >> ace_up_bet;
        }
        //sleep
        random_generator();
        int position;
        cout << "After viewing your hand, which position would you like to eliminate? ";
        cin >> position;
        sleep(1);
        player.erase(player.begin() + (2 * position - 2)); // for ranks
        player.erase(player.begin() + (2 * position - 2)); // for suit
        char input;
        cout << "Your hand: ";
        for (int j = 0; j < player.size(); j++)
        {
            cout << player[j];
        }
        cout << endl;
        cout << "After eliminating your card, would you like to fold? Y for yes, N for no, Q for quit: ";
        cin >> input;
        sleep(2);
        if (toupper(input) == 'Y') // player folds
        {
            asset -= (ante_bet + ace_up_bet);
        }
        else if (toupper(input) == 'N') // player doesn't fold
        {
            for (int i = 0; i < dealer.size(); i++)
            {
                cout << dealer[i];
            }
            cout << endl;
            sleep(2);
            dealer_remover();
            int player_check = head_to_head(player);
            int dealer_check = head_to_head(dealer);
            if (player_check == dealer_check)
            {
                //CORNER CASE   
                if (player_check == 3)
                {
                    int player_score, dealer_score;
                    for (int i = 0;i < dealer.size(); i+=2)
                    {
                        if (count(player.begin(), player.end(), player[i]) == 3)
                        {
                            player_score = distance(ranking, find(ranking, ranking + 13, player[i]));
                        }
                        else if(count(dealer.begin(), dealer.end(), dealer[i]) == 3)
                        {
                            dealer_score = distance(ranking, find(ranking, ranking + 13, dealer[i]));
                        }
                    }
                    if (player_score > dealer_score)
                    {
                        asset += (ante_bet * 3) + ante_bet;
                        asset = ante_pay(asset, ante_bet, player);
                        if (ace_up_bet > 0)
                        {
                            asset = ace_up(asset, ace_up_bet, player);
                        }
                    }
                    else if (player_score < dealer_score)
                    {
                        asset -= ((ante_bet * 3) + ante_bet);
                        asset = ante_pay(asset, ante_bet, player);
                        if (ace_up_bet > 0)
                        {
                            asset = ace_up(asset, ace_up_bet, player);
                        }
                    }
                }
                else
                {
                    int tie_score = tie_check(player, dealer);
                    if (tie_score == 2) // dealer wins
                    {
                        asset = ante_pay(asset, ante_bet, player);
                        asset -= ((ante_bet * 3) + ante_bet);
                        if (ace_up_bet > 0)
                        {
                            asset = ace_up(asset, ace_up_bet, player);
                        }
                    }
                    else if (tie_score == 1) // player wins
                    {
                        asset = ante_pay(asset, ante_bet, player);
                        asset += ((ante_bet * 3) + ante_bet);
                        if (ace_up_bet > 0)
                        {
                            asset = ace_up(asset, ace_up_bet, player);
                        }
                    }
                    else // both tie
                    {
                        asset = ante_pay(asset, ante_bet, player);
                        if (ace_up_bet > 0)
                        {
                            asset = ace_up(asset, ace_up_bet, player);
                        }
                    }
                }    
            }
            else if (player_check > dealer_check) // player win
            {
                asset = ante_pay(asset, ante_bet, player);
                asset += ((ante_bet * 3) + ante_bet);
                if (ace_up_bet > 0)
                {
                    asset = ace_up(asset, ace_up_bet, player);
                }
            }
            else if (player_check < dealer_check) // PLAYER LOSES   
            {
                asset = ante_pay(asset, ante_bet, player);
                asset -= ((ante_bet * 3) + ante_bet);
                if (ace_up_bet > 0)
                {
                    asset = ace_up(asset, ace_up_bet, player);
                }
            }
        }
        else if (toupper(input) == 'Q')
        {
            cout << "Goodbye. Hope to see you again. Remeber that you will lose all of your money!" << endl;
            return 1;
        }
        sleep(1);
        player.clear();
        dealer.clear();
    }
    return 0;
}

bool player_straight_flush(vector<string> cards) // for player only (4 cards *8 in vec*)
{
    vector<int> store;
    for (int i = 0; i < cards.size(); i+=2)
    {
        store.push_back(distance(ranking, find(ranking, ranking + 13, cards[i])));
    }
    sort(store.begin(), store.end());
    int increment = -1;
    for (int j = 1; j < store.size(); j++)
    {  
        if (store[0] - store[j] != increment)
        {
            return false;
        }
        else 
        {
            increment--;
        }
    }
    for (int a = 3; a < cards.size(); a+=2)
    {
        if (cards[1] != cards[a])
        {
            return false;
        }
    }
    return true;
}

bool player_four_of_a_kind(vector<string> cards)
{
    for (int i = 2; i < cards.size(); i+=2)
    {
        if (cards[0] != cards[i])
        {
            return false;
        }
    }
    return true;
}

bool player_three_of_a_kind(vector<string> cards) // for player only
{
    int counter = 0;
    vector<string> store;
    for (int i = 0; i < cards.size(); i+=2)
    {
        store.push_back(cards[i]);
    }
    sort(store.begin(), store.end());
    for (int j = 0; j < store.size() - 1; j++)
    {
        if (store[j] == store[j + 1])
        {
            counter++;
        }
    }
    if (counter == 2)
    {
        return true;
    }
    return false;
}

bool player_flush(vector<string> cards)
{
    for (int i = 3; i < cards.size(); i+=2)
    {
        if (cards[1] != cards[i])
        {
            return false;
        }
    }
    return true;
}

bool player_straight(vector<string> cards) // all 4 has to be straight
{
    vector<int> store;
    for (int i = 0; i < cards.size(); i+=2)
    {
        store.push_back(distance(ranking, find(ranking, ranking + 13, cards[i])));
    }
    sort(store.begin(), store.end());
    int increment = -1;
    for (int j = 1; j < store.size(); j++)
    {  
        if (store[0] - store[j] == increment)
        {
            increment--;
        }
        else 
        {
            return false;
        }
    }
    return true;
}

bool player_two_pair(vector<string> cards)
{
    vector<int> store, distinct;
    for (int i = 0; i < cards.size(); i+=2)
    {
        store.push_back(distance(ranking, find(ranking, ranking + 13, cards[i])));
    }
    sort(store.begin(), store.end());
    distinct = store;
    vector<int>::iterator it = unique(distinct.begin(), distinct.end());
    distinct.resize(distance(distinct.begin(), it));
    int counter = 0;
    for (int i = 0; i < distinct.size(); i++)
    {
        if (count(store.begin(), store.end(), distinct[i]) == 2)
        {
            counter++;
        }
    }
    if (counter == 2)
    {
        return true;
    }
    return false;
}

bool player_one_pair(vector<string> cards)
{
    vector<int> store, distinct;
    for (int i = 0; i < cards.size(); i+=2)
    {
        store.push_back(distance(ranking, find(ranking, ranking + 13, cards[i])));
    }
    sort(store.begin(), store.end());
    distinct = store;
    vector<int>::iterator it = unique(distinct.begin(), distinct.end());
    distinct.resize(distance(distinct.begin(), it));
    int counter = 0;
    for (int i = 0; i < distinct.size(); i++)
    {
        if (distinct[i] == 12 && count(store.begin(), store.end(), distinct[i]) == 2)
        {
            counter++;
        }
    }
    if (counter == 1)
    {
        return true;
    }
    return false;   
}

int tie_check(vector<string> player, vector<string> dealer) 
{// comparing high card after taking dealer's card off 
    vector<int> dealer_score, player_score;
    for (int i = 0; i < dealer.size(); i+=2)
    {
        dealer_score.push_back(distance(ranking, find(ranking, ranking + 13, dealer[i])));
        player_score.push_back(distance(ranking, find(ranking, ranking + 13, player[i])));
    }
    sort(dealer_score.begin(), dealer_score.end());
    sort(player_score.begin(), player_score.end());
    for (int j = dealer_score.size() - 1; j > -1; j--)
    {
        if (dealer_score[j] > player_score[j])
        {//dealer wins
            return 2;
        }
        else if (dealer_score[j] < player_score[j])
        { // player wins
            return 1;
        }
    }
    return 0; // both tie for real
}

void random_generator()
{
    int x = 0, y = 0;
    vector<string> store;
    while (x < 5)
    {
        string player_rank = ranking[rand() % 13];
        string player_suit = suit[rand() % 4];
        if (count(store.begin(), store.end(), player_rank) < 4)
        {
            player.push_back(player_rank);
            player.push_back(player_suit);
            x++;
        }
    }
    while (y < 5)
    {
        string dealer_rank = ranking[rand() % 13];
        string dealer_suit = suit[rand() % 4];
        if (count(store.begin(), store.end(), dealer_rank) < 4)
        {
            dealer.push_back(dealer_rank);
            dealer.push_back(dealer_suit);
            y++;
        }
    }
    for (int x = 0; x < player.size(); x++) // display player's cards ONLY
    {
        cout << player[x];
    }
    cout << endl;
    return;
}

void dealer_remover() // call once
{
    vector<int> result;
    for (int i = 0; i < dealer.size(); i+=2)
    {
        vector<string> duplicate;
        duplicate = dealer;
        duplicate.erase(duplicate.begin() + i); // for rank
        duplicate.erase(duplicate.begin() + i); // for suit
        if (player_four_of_a_kind(duplicate))
        {
            result.push_back(6);
        }
        else if (player_straight_flush(duplicate))
        {
            result.push_back(5);
        }
        else if (player_three_of_a_kind(duplicate))
        {
            result.push_back(4);
        }
        else if (player_flush(duplicate))
        {
            result.push_back(3);
        }
        else if(player_straight(duplicate))
        {
            result.push_back(2);
        }
        else if (player_two_pair(duplicate))
        {
            result.push_back(1);
        }
        else if (player_one_pair(duplicate))
        {
            result.push_back(0);
        }
        else
        {
            result.push_back(-1); // high pair
        }
        duplicate.clear();
    }
    int max = *max_element(result.begin(), result.end());
    int index = find(result.begin(), result.end(), max) - result.begin(); // index of max
    if (count(result.begin(), result.end(), max) > 1)
    {
        int remove, compare = 15, position;
        for (int i  = 0; i < result.size(); i++)
        {
            if (result[i] == max)
            {
                remove = distance(ranking, find(ranking, ranking + 13, dealer[i*2]));
                if (remove < compare)
                {
                    compare = remove;
                    position = i;
                }
            }
        }
        dealer.erase(dealer.begin() + (2 * position));
        dealer.erase(dealer.begin() + (2 * position));
        for (int x = 0; x < dealer.size(); x++)
        {
            cout << dealer[x];
        }
        cout << endl;
        return;
    }
    dealer.erase(dealer.begin() + (2 * index));
    dealer.erase(dealer.begin() + (2 * index)); // remove cards that leave the hand dealt the weakest
    cout << "After dealer removes his/her card: ";
    for (int j = 0; j < dealer.size(); j++)
    {
        cout << dealer[j];
    }
    cout << endl;
    return;
}

int ante_pay(int asset, int amount, vector<string> cards) // if win ALWAYS pay play bet 3 TIMES ante bet
{
    if (player_four_of_a_kind(cards))
    {
        return asset + (25 * amount);
    }
    else if (player_straight_flush(cards))
    {
        return asset + (20 * amount);
    }
    else if(player_three_of_a_kind(cards))
    {
        return asset + amount;
    }
    return asset;
}

int ace_up(int asset, int amount, vector<string> cards)
{
    if (player_four_of_a_kind(cards))
    {
        return asset + (amount * 50);
    }
    else if(player_straight_flush(cards))
    {
        return asset + (amount * 40);
    }
    else if (player_three_of_a_kind(cards))
    {
        return asset + (amount * 8);
    }
    else if (player_flush(cards))
    {
        return asset + (amount * 5);
    }
    else if (player_straight(cards))
    {
        return asset + (amount * 4);
    }
    else if (player_two_pair(cards))
    {
        return asset + (amount * 3);
    }
    else if (player_one_pair(cards))
    {
        return asset + amount;
    }
    return asset - amount;
}

int head_to_head(vector<string> cards)
{
    if (player_four_of_a_kind(cards))
    {
        return 6;
    }
    else if(player_straight_flush(cards))
    {
        return 5;
    }
    else if (player_three_of_a_kind(cards))
    {
        return 4;
    }
    else if (player_flush(cards))
    {
        return 3;
    }
    else if (player_straight(cards))
    {
        return 2;
    }
    else if(player_two_pair(cards))
    {
        return 1;
    }
    else if (player_one_pair(cards))
    {
        return 0;
    }
    return -1; // high pair
}