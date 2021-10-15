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
const string suit[4] = {"\u2665", "\u2660", "\u2663", "\u2666"}; //heart, spade, club, diamond respectively
const string ranking[13] = {"2", "3", "4","5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
vector<string> player, banker, store;
void player_draw();
void banker_draw();
int add(vector<string> cards);
int panda(int asset, int amount, vector<string> cards); // for player
int dragon(int asset, int amount, vector<string> cards); // for banker
int tie(int asset, int amount, vector<string> player_cards, vector<string> banker_cards);
int player_comparision(int asset, int amount, vector<string> player, vector<string> banker);
int banker_comparision(int asset, int amount, vector<string> player, vector<string> banker);
void random_generator();
int main()
{
    cout << "Welcome to the world of EZ Bacarat: Created by Ethan Nguyen" << endl;
    int player_asset = 100, banker_asset = 100, bet;
    char input;
    cout << "Which side would you like to bet? Please press P for player, B for banker, Q for quit: ";
    cin >> input;
    srand(time(0));
    if (toupper(input) == 'P')
    {
        sleep(3);
        cout << "Welcome to the Player's side! You are given $" << player_asset << ". Use wisely!" << endl;
        while (player_asset > 0)
        {
            sleep(1);
            cout << "You own $" << player_asset << ". How much would you like to bet? ";
            cin >> bet;
            while (bet > player_asset)
            {
                cout << "You've bet pass your limit! Please try again: ";
                cin >> bet;
            }
            cout << "Ok! Let's play." << endl;
            sleep(2);
            random_generator();
            sleep(2);
            int player_scores = add(player), banker_scores = add(banker);
            if (player_scores > 7 && banker_scores > 7)
            {
                (player_scores > banker_scores) ? player_asset += bet 
                : (player_scores < banker_scores) ? player_asset -= bet 
                : player_asset;
            }
            else if (player_scores > 7 && banker_scores < 8)
            {
                player_asset += bet;
            }
            else if (player_scores < 8 && banker_scores > 7)
            {
                player_asset -= bet;
            }
            else if (player_scores > 5 && player_scores < 8 && banker_scores > 5 && banker_scores < 8)
            {
                (player_scores > banker_scores) ? player_asset += bet
                : (player_scores < banker_scores) ? player_asset -= bet
                : player_asset;
            }
            else if (player_scores > 5 && player_scores < 8 && banker_scores < 7)
            {
                banker_draw();
                int player_scores = add(player), banker_scores = add(banker);
                (player_scores > banker_scores) ? player_asset += bet
                : (player_scores < banker_scores) ? player_asset -= bet
                : player_asset;
            }
            else if (player_scores < 6 && banker_scores == 7)
            {
                player_draw();
                int player_scores = add(player), banker_scores = add(banker);
                (player_scores > banker_scores) ? player_asset += bet
                : (player_scores < banker_scores) ? player_asset -= bet
                : player_asset;
            }
            else if (player_scores < 6 && banker_scores < 7)
            {
                player_asset = player_comparision(player_asset, bet, player, banker);
            }

            player_asset = panda(player_asset, bet, player);
            player_asset = tie(player_asset, bet, player, banker);

            // clear vec before restarting game
            player.clear();
            banker.clear();
            store.clear();
        }
    }
    else if (toupper(input) == 'B')
    {
        sleep(3);
        cout << "Welcome to the Banker's side! You are given $" << banker_asset << ". Use wisely!" << endl;
        while (banker_asset > 0)
        {
            sleep(1);
            cout << "You own $" << banker_asset << ". How much would you like to bet? ";
            cin >> bet;
            while (bet > banker_asset)
            {
                cout << "You've bet pass your limit! Please try again: ";
                cin >> bet;
            }
            cout << "Ok! Let's play." << endl;
            sleep(2);
            random_generator();
            sleep(2);
            int player_scores = add(player), banker_scores = add(banker);
            if (player_scores > 7 && banker_scores > 7)
            {
                (player_scores > banker_scores) ? banker_asset -= bet 
                : (player_scores < banker_scores) ? banker_asset += bet 
                : banker_asset;
            }
            else if (player_scores > 7 && banker_scores < 8) // player wins
            {
                banker_asset -= bet;
            }
            else if (player_scores < 8 && banker_scores > 7) // banker wins
            {
                banker_asset += bet;
            }
            else if (player_scores > 5 && player_scores < 8 && banker_scores > 5 && banker_scores < 8)
            {
                (player_scores > banker_scores) ? banker_asset -= bet 
                : (player_scores < banker_scores) ? banker_asset += bet 
                : banker_asset;
            }
            else if (player_scores > 5 && player_scores < 8 && banker_scores < 7)
            {
                banker_draw();
                int player_scores = add(player), banker_scores = add(banker);
                (player_scores > banker_scores) ? banker_asset -= bet 
                : (player_scores < banker_scores) ? banker_asset += bet 
                : banker_asset;
            }
            else if (player_scores < 6 && banker_scores == 7)
            {
                player_draw();
                int player_scores = add(player), banker_scores = add(banker);
                (player_scores > banker_scores) ? banker_asset -= bet 
                : (player_scores < banker_scores) ? banker_asset += bet 
                : banker_asset;
            }
            else if (player_scores < 6 && banker_scores < 7)
            {
                banker_asset = banker_comparision(banker_asset, bet, player, banker); // fix
            }

            //bonus
            banker_asset = dragon(banker_asset, bet, banker);
            banker_asset = tie(banker_asset, bet, player, banker);

            // clear vec before restarting game
            player.clear();
            banker.clear();
            store.clear();
        }
    }
    else
    {
        cout << "Goodbye. Hope to see you again soon. Remember that all of your money is lost." << endl;
        return 1;
    }

    if (banker_asset < 1 || player_asset < 1)
    {
        cout << "You loss... Come back again soon! Ciao." << endl;
    }
    return 0;
}


void player_draw() // can only draw one card
{
    while (true)
    {
        string player_rank = ranking[rand() % 13];
        string player_suit = suit[rand() % 4];
        if (count(store.begin(), store.end(), player_rank) < 4)
        {
            player.push_back(player_rank);
            player.push_back(player_suit);
            store.push_back(player_rank);
            store.push_back(player_suit);
            cout << "Player's hand after drawing: ";
            for (int i = 0; i < player.size(); i++)
            {
                cout << player[i];
            }
            cout << endl;
            return;
        }
    }
}

void banker_draw() // can only draw one card
{
    while (true)
    {
        string banker_rank = ranking[rand() % 13];
        string banker_suit = suit[rand() % 4];
        if (count(store.begin(), store.end(), banker_rank) < 4)
        {
            banker.push_back(banker_rank);
            banker.push_back(banker_suit);
            store.push_back(banker_rank);
            store.push_back(banker_suit);
            cout << "Banker's hand after drawing: ";
            for (int i = 0; i < banker.size(); i++)
            {
                cout << banker[i];
            }
            cout << endl;
            return;
        }
    }
}

int add(vector<string> cards)
{
    int ans = 0;
    for (int i = 0; i < cards.size(); i+=2)
    {
        if (cards[i] == "A")
        {
            ans = (ans + 1) % 10;
        }
        else if (cards[i] == "J" || cards[i] == "Q" || cards[i] == "K" || cards[i] == "10")
        {
            continue;
        }
        else
        {
            ans = (ans + stoi(cards[i])) % 10;
        }
    }
    return ans;
}

int panda(int asset, int amount, vector<string> cards)
{
    if (cards.size() == 6)
    {
        if (add(cards) == 8)
        {
            cout << "You've won a Panda bonus! $" << 25 * amount << endl; 
            return asset + (25 * amount);
        }
    }
    return asset;
}

int dragon(int asset, int amount, vector<string> cards)
{
    if (cards.size() == 6)
    {
        if (add(cards) == 7)
        {
            cout << "You've won a Dragon bonus! $" << 40 * amount << endl;
            return asset + (40 * amount);
        }
    }
    return asset;
}

int tie(int asset, int amount, vector<string> player_cards, vector<string> banker_cards)
{
    if (add(player_cards) == add(banker_cards))
    {
        cout << "You've won a tie bonus! $" << 8 * amount << endl;
        return asset + (8 * amount);
    }
    return asset;
}

int player_comparision(int asset, int amount, vector<string> player, vector<string> banker)
{
    // return asset at the end
    if (add(banker) == 0 || add(banker) == 1 || add(banker) == 2)
    {
        banker_draw();
        int player_score = add(player), banker_score = add(banker);
        (player_score > banker_score) ? asset += amount 
        : (player_score < banker_score) ? asset -= amount
        : asset;
    }
    else if (add(banker) == 3)
    {
        player_draw();
        if (player[4] == "8")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
        else //not 8
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
    }
    else if (add(banker) == 4)
    {
        player_draw();
        if (player[4] == "A" || player[4] == "9" || player[4] == "8" || player[4] == "J" || player[4] == "Q" || player[4] == "K" || player[4] == "10")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
        else
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
    }
    else if (add(banker) == 5)
    {
        player_draw();
        if (player[4] == "A" || player[4] == "9" || player[4] == "8" || player[4] == "J" || player[4] == "Q" || player[4] == "K" || player[4] == "10" || player[4] == "2" ||player[4] == "3")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
        else
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
    }
    else if (add(banker) == 6)
    {
        player_draw();
        if (player[4] == "A" || player[4] == "9" || player[4] == "8" || player[4] == "J" || player[4] == "Q" || player[4] == "K" || player[4] == "10" || player[4] == "2" || player[4] == "3" || player[4] == "4" || player[4] == "5")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
        else
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset += amount 
            : (player_score < banker_score) ? asset -= amount
            : asset;
        }
    }
    return asset;
}


void random_generator()
{
    int i = 0;
    while (i < 2)
    {
        string player_rank = ranking[rand() % 13];
        string player_suit = suit[rand() % 4];
        string banker_rank = ranking[rand() % 13];
        string banker_suit = suit[rand() % 4];
        if (count(player.begin(), player.end(), player_rank) < 4 && count(banker.begin(), banker.end(), banker_rank) < 4)
        {
            player.push_back(player_rank);
            player.push_back(player_suit);
            banker.push_back(banker_rank);
            banker.push_back(banker_suit);
            i++;
        }
    }
    cout << "Player: ";
    for (int i = 0; i < player.size(); i++)
    {
        cout << player[i];
    }
    cout << endl << "Banker: ";
    for (int j = 0; j < banker.size(); j++)
    {
        cout << banker[j];
    }
    cout << endl;
    return;
}

int banker_comparision(int asset, int amount, vector<string> player, vector<string> banker)
{
    // return asset at the end
    if (add(banker) == 0 || add(banker) == 1 || add(banker) == 2)
    {
        banker_draw();
        int player_score = add(player), banker_score = add(banker);
        (player_score > banker_score) ? asset -= amount 
        : (player_score < banker_score) ? asset += amount
        : asset;
    }
    else if (add(banker) == 3)
    {
        player_draw();
        if (player[4] == "8")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
        else //not 8
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
    }
    else if (add(banker) == 4)
    {
        player_draw();
        if (player[4] == "A" || player[4] == "9" || player[4] == "8" || player[4] == "J" || player[4] == "Q" || player[4] == "K" || player[4] == "10")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
        else
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
    }
    else if (add(banker) == 5)
    {
        player_draw();
        if (player[4] == "A" || player[4] == "9" || player[4] == "8" || player[4] == "J" || player[4] == "Q" || player[4] == "K" || player[4] == "10" || player[4] == "2" ||player[4] == "3")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
        else
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
    }
    else if (add(banker) == 6)
    {
        player_draw();
        if (player[4] == "A" || player[4] == "9" || player[4] == "8" || player[4] == "J" || player[4] == "Q" || player[4] == "K" || player[4] == "10" || player[4] == "2" || player[4] == "3" || player[4] == "4" || player[4] == "5")
        {
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
        else
        {
            banker_draw();
            int player_score = add(player), banker_score = add(banker);
            (player_score > banker_score) ? asset -= amount 
            : (player_score < banker_score) ? asset += amount
            : asset;
        }
    }
    return asset;
}