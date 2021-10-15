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

using namespace std;
const string signs[4] = {"\u2665", "\u2660", "\u2663", "\u2666"}; //heart, spade, club, diamond respectively
const string sett[13] = {"A", "2", "3", "4","5", "6", "7", "8", "9", "10", "J", "Q", "K"};
vector<string> dealer_score;
vector<string> player_score; 
int dealer_drawing();
int player_drawing();
int hit(int score);
bool blackjack(vector<string> check);
int dealer_final_display(int score);

int main()
{
    int asset = 100, amount_bet;
    cout << "Welcome to the world of Blackjack: Created by Ethan Nguyen." << endl;
    sleep(1);
    srand(time(0)); // run random num every time program is executed
    cout << "You are given $" << asset << ". Use wisely!" << endl;
    sleep(1); 
    while (asset > 0)
    {
        cout << "You own $" << asset << " How much would you like to bet? ";
        cin >> amount_bet;
        while (true) // protect against invalid input, such as characters
        {
            if (cin.fail())
            {
                cout << "Invalid Input. Please try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cin >> amount_bet;
            } 
            else
            {
                break;
            }
        }
        while (amount_bet > asset)
        {
            cout << endl << "The amount you entered exceeds your currect asset. Please choose a smaller amount: ";
            cin >> amount_bet;
        }
        sleep(1);
        cout << "OK! Let's play." << endl;
        cout << "Dealer: ";
        sleep(1);
        int DealerScore = dealer_drawing(); 
        sleep(1);
        cout << endl << "You: ";
        int PlayerScore = player_drawing();
        cout << endl;
        if (blackjack(player_score) == true && blackjack(dealer_score) == true)
        { // both parties have blackjack
            for (int i = 0; i < dealer_score.size(); i++)
            {
                cout << dealer_score[i];
            }
            cout << endl << "You both have blackjack. TIE." << endl;
            sleep(1);
        }
        else if (blackjack(player_score) == true && blackjack(dealer_score) == false)
        { // player win blackjack
            cout << "Congrats! You win!" << endl;
            sleep(1);
            asset = asset + (amount_bet * 1.5) + amount_bet;
        }
        else if (blackjack(player_score) == false && blackjack(dealer_score) == true)
        { // player lost blackjack
            for (int i = 0; i < dealer_score.size(); i++)
            {
                cout << dealer_score[i];
            }
            cout << "Blackjack..." << " You lost" << endl;
            asset -= amount_bet;
        }
        else // the actual playing starts if no blackjack from both parties recognized
        {
            char input;
            while (true)
            {
                cout << "Please press H to hit, S to stay, D to Double Down, and Q to quit the game: ";
                cin >> input;
                if (toupper(input) == 'H') // hit
                {
                    PlayerScore = hit(PlayerScore);
                    if (PlayerScore > 21)
                    {
                        asset -= amount_bet;
                        cout << "Dealer final result: ";
                        for (int j = 0; j < dealer_score.size(); j++)
                        {
                            cout << dealer_score[j];
                        }
                        cout << endl << "Sorry... You lost... Please try again" << endl;
                        break;
                    }
                }
                else if (toupper(input) == 'S') //stay
                {
                    break;
                } 
                else if (toupper(input) == 'D' && player_score.size() == 4) // double down (only on the first hit)
                {
                    amount_bet *= 2;
                    PlayerScore = hit(PlayerScore);
                    if (PlayerScore > 21)
                    {
                        asset -= amount_bet;
                        cout << "Dealer final result: ";
                        for (int j = 0; j < dealer_score.size(); j++)
                        {
                            cout << dealer_score[j];
                        }
                        cout << endl << "Sorry... You lost... Please try again" << endl;
                        break;
                    }
                    break;
                }
                else if (toupper(input) == 'Q') // quit
                {
                    cout << "Good bye. Come back at anytime. Although, remember that you've lost everything." << endl;
                    return 1;
                }
            }
            if (PlayerScore < 22) // player still in game
            {
                cout << "Dealer final result: ";
                DealerScore = dealer_final_display(DealerScore);
                sleep(2);
                if (DealerScore > 21) // dealer break first
                {
                    asset += amount_bet;
                    sleep(1);
                    cout << "YOU WON!! CONGRATS!" << endl;
                }
                else if (DealerScore > PlayerScore) // both in game: dealer won, player lost
                {
                    asset -= amount_bet;
                    sleep(1);
                    cout << "You lost... Please try again" << endl;
                }
                else if (DealerScore < PlayerScore) // both in game: dealer lost, player won
                {
                    asset += amount_bet;
                    sleep(1);
                    cout << "YOU WON!! CONGRATS!" << endl;
                }
                else //both in game: tie
                {
                    sleep(1);
                    cout << "TIE." << endl;
                }
            }
        }   
        player_score.clear(); // clear vector before restart the game
        dealer_score.clear();
    }
    if (asset <= 0)
    {
        cout << "Sorry! Come back soon." << endl;
    }
    return 0;
}

int dealer_drawing() // initial drawing for the dealer, return total score
{
    int total = 0;
    for (int i = 0; i < 2; i++)
    {
        // card drawing for dealer
        int dealer_sign_generator = rand() % 4;
        int dealer_sett_generator = rand() % 13;
        int dealer_card = 0;
        if (dealer_sett_generator == 0)
        {
            dealer_card = 1;
        }
        else if (dealer_sett_generator > 9)
        {
            dealer_card = 10;
        }
        else
        {
            dealer_card = stoi(sett[dealer_sett_generator]);
        }

        //for display purposes
        if (i == 0)
        {
            cout << sett[dealer_sett_generator] << signs[dealer_sign_generator] << endl;
        }
        dealer_score.push_back(sett[dealer_sett_generator]);
        dealer_score.push_back(signs[dealer_sign_generator]);
        total += dealer_card;
    }
    return total;
}

int player_drawing() //initial drawing for the player
{
    int total = 0;
    for (int i = 0; i < 2; i++)
    {
        // card drawing for player
        int player_sign_generator = rand() % 4;
        int player_sett_generator = rand() % 13;
        int player_card = 0;
        if (player_sett_generator == 0)
        {
            player_card = 1;
        }
        else if (player_sett_generator > 9)
        {
            player_card = 10;
        }
        else
        {
            player_card = stoi(sett[player_sett_generator]);
        }

        //for display purposes
        cout << sett[player_sett_generator] << signs[player_sign_generator];
        if (i == 1)
        {
            cout << endl;
        }
        player_score.push_back(sett[player_sett_generator]);
        player_score.push_back(signs[player_sign_generator]);
        total += player_card;
    }
    return total;
}

int hit(int score) 
{
    //drawing a card for the player
    int player_sign_generator = rand() % 4;
    int player_sett_generator = rand() % 13;
    int player_card;
    if (player_sett_generator == 0)
    {
        player_card = 1;
    }
    else if (player_sett_generator > 9)
    {
        player_card = 10;
    }
    else
    {
        player_card = stoi(sett[player_sett_generator]);
    }
    player_score.push_back(sett[player_sett_generator]);
    player_score.push_back(signs[player_sign_generator]);
    for (int j = 0; j < player_score.size(); j++)
    {
        cout << player_score[j];
    }
    cout << endl;
    return score + player_card;
}

bool blackjack(vector<string> check)
{
    if (check[0] == "A" && ((check[2] == "10"||check[2] == "J"||check[2] == "Q"||check[2] == "K")))
    {
        return true;
    }
    else if (check[2] == "A" && ((check[0] == "10"||check[0] == "J"||check[0] == "Q"||check[0] == "K")))
    {
        return true;
    }
    return false;
}
int dealer_final_display(int score) 
{
    
    //dealer display one last time before decide to hit or not
    for (int j = 0; j < dealer_score.size(); j++)
    {
        cout << dealer_score[j];
    }
    sleep(2);
    //dealer hits until total >= 17
    while (score < 17)
    {
        int dealer_sign_generator = rand() % 4;
        int dealer_sett_generator = rand() % 13;
        int dealer_card = 0;
        if (dealer_sett_generator == 0)
        {
            dealer_card = 1;
        }
        else if (dealer_sett_generator > 9)
        {
            dealer_card = 10;
        }
        else
        {
            dealer_card = stoi(sett[dealer_sett_generator]);
        }
        cout << sett[dealer_sett_generator] << signs[dealer_sign_generator];
        score += dealer_card;
        sleep(1);
    }
    cout << endl;
    return score;
}
