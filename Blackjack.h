#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <getopt.h>
#include <algorithm>
#include <string>
#include <unordered_set>
#include "GamePlayer.h"
#include "CardDeck.h"
#include "Dealer.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <time.h>
#include <climits>
#include <cstdint>

//enumerate move types
enum class MOVES : char {
    Pass = 'P',
    Hit = 'H',
    DoubleDown = 'D',
    Split = 'S',
    ViewDealer = 'V',
    Quit = 'Q'
};

//custom comparator to check whether money is <= 0
class pred {
public:
    bool operator()(const GamePlayer& p) const{
        if (p.getMoney() <= 0) {
            return true;
        }
        return false;
    }
};

// Read input from os stream
int readPlayerCount() {
    int input;
    bool valid = false;

    do {
        std::cin >> input;
        if (input == 0 || input > 7) {
            valid = false;
            std::cout << "Oops! You have entered an incorrect player count" << "\n";
            std::cout << "Please Enter the Number of Players at the Table: ";
        } else {
            valid = true;
        }

        if (std::cin.fail()) {

            //flush out cin
            std::cin.clear();
            std::cin.ignore(INT_MAX,'\n');
        }

    } while(!valid);

    return input;
}


class Blackjack {
public:
    //Constructor
    Blackjack(){

        // read in number of players and reserve "players" vector
        std::cout << "\n" << "Welcome to Blackjack!" << "\n";
        std::cout << "\n" << "Please Enter the Number of Players at the Table: ";
        numPlayers = readPlayerCount();

        players.reserve(numPlayers);

        std::cout << "Please enter your names below: " << "\n";

        //read in each player name, create GamePlayer object, and place in players vector 
        std::string playerName;
        for (unsigned int i = 0; i < numPlayers; i++) {
            std::cout << "Player " + std::to_string(i + 1) << ": ";
            std::cin >> playerName;
            players.emplace_back(playerName,1000);
        }

        //create card deck
        deck = CardDeck();
        std::cout << "\n";
    };

    //sets up round by adding all GamePlayers to the "roundPlayers" hashset
    void setupRound() {
        for (unsigned int i = 0; i < players.size();i++) {
            roundPlayers.insert(i);
        }
    }

    // reach in each GamePlayer's input bet
    int readBet(int playerIndex) {
        int input;
        bool valid = false;
        do {
            std::cin >> input;

            if (input <= 0 || input > players[playerIndex].getMoney()) {
                valid = false;
                std::cout << "Oops! You have placed an invalid bet!" << "\n";
                std::cout << players[playerIndex].getName() << ": $";
            } else {
                valid = true;
            }

            if (std::cin.fail()) {

                //flush out cin
                std::cin.clear();
                std::cin.ignore(INT_MAX,'\n');
            }

        } while(!valid);

        return input;
    }

    //reads each GamePlayer's bet and updates the corresponding money value
    void placeBets() {
        std::cout << "*********************** Round " << roundCounter << " ***********************" << "\n";
        std::cout << "Please place your bets below: " << "\n";

        for (unsigned int i = 0; i < numPlayers; i++) {
            std::cout << players[i].getName() << ": $";

            int bet = readBet(i);
            players[i].placeBet(bet,1);

            std::cin.clear();
            std::cin.ignore(INT_MAX,'\n');
        }

        std::cout << "All bets are final. Cards are now being dealt." << "\n";
    }


    // deal cards to each GamePlayers and the dealer
    void dealCards(){
        // allows for different seed values
        std::srand (time(NULL));

        //generate a random seed and shuffle the deck
        unsigned int seed = (rand()%10)+3;

        // shuffle cards every 5 rounds
        if (roundCounter % 5 == 0 || roundCounter == 1) {
            deck.shuffle(seed);
        }
        
        //deal first round cards to each GamePlayer and the dealer
        for (unsigned int i = 0; i < numPlayers; i++) {
            players[i].addCard(deck.deal_one(),1);
        }
        dealer.addCard(deck.deal_one());

        //deal second round cards to each GamePlayer and the dealer
        for (unsigned int i = 0; i < numPlayers; i++) {
            players[i].addCard(deck.deal_one(),1);
        }
        dealer.addCard(deck.deal_one());
    }

    //checks for a blackjack across all the GamePlayers and the dealer
    bool checkBlackJack() {
        //Check if the dealer has a blackjack
        if (dealer.getHandValue() == maxPoints) {
            std::cout << "The dealer has a Blackjack!" << "\n";

            //For each GamePlayer, check whether he/she has a blackjack. 
            for (unsigned int i = 0; i < numPlayers;i++) {
                // If the GamePlayer has a blackjack, then a tie occurs and the GamePlauer
                // does not lose any many
                if (players[i].getHandValue(1) == maxPoints) {
                    std::cout << "Luckily, " << players[i].getName() << " also has a Blackjack!" << "\n";
                    players[i].resetHand();

                //if the GamePlayer has anything else other than the blackjack, he/she loses their wages
                } else {
                    std::cout << players[i].getName() << " loses $" << players[i].getWager(1) << "."<<"\n";
                    players[i].addMoney(-1*players[i].getWager(1));
                    roundPlayers.erase(i);
                    players[i].resetHand();
                }
            }

            std::cout << "\n";
            roundPlayers.clear();
            return false;

        } else {

            // in the case where the dealer does not have a blackjack,
            // check whether any of the GamePlayers do
            for (unsigned int i = 0; i < numPlayers;i++) {
                //if a GamePlayer has a blackjack, then he/she wins 1.5x the wager amount
                if (players[i].getHandValue(1) == maxPoints) {
                    std::cout << "\n" << "BLACKJACK!! " << players[i].getName() << " wins $" << int(players[i].getWager(1) * (1.5)) << " with: " << "\n";
                    players[i].showHands();
                    players[i].addMoney(int((1.5 * players[i].getWager(1))));
                    roundPlayers.erase(i);
                    players[i].resetHand();
                } 
            }

            std::cout << "\n";

            if (roundPlayers.size() == 0) {
                return false;
            }

            return true;
        }
    }

    // run a single round through each GamePlayer
    void runRound() {
        // for each GamePlayer...
        for (unsigned int i = 0; i < numPlayers;i++) {
            // only proceed if the player is still in the round
            if(roundPlayers.find(i) != roundPlayers.end()) {

                std::cout << "It's your turn, " << players[i].getName() << ": " << "\n";
                players[i].showHands();

                std::cout << "\n";

                std::string move = "";
                bool playHand = true;
                bool playPlayer = true;

                // for each hand of a given GamePlayer(max 2 from a split), allow the player
                // to pass, hit, split, double down, view dealer care, and quit
                for (int j = 0; j < players[i].getNumHands(); j++) {
                    //while the GamePlayer can still play
                    while(playHand && playPlayer) {

                        std::cout << "\t" << "What will you do with Hand " << std::to_string(j+1) << "?(P:H:S:D:V:Q): ";
                        std::cin >> move; 
                        move[0] = std::toupper(move[0]);

                        //flush out cin
                        std::cin.clear();
                        std::cin.ignore(INT_MAX,'\n');

                        switch (static_cast<MOVES>(move[0])) {

                            case MOVES::Pass:
                            {
                                // prevent the GamePlayer from playing on this hand any further
                                playHand = false;
                                break;
                            }

                            case MOVES::Hit:
                            {
                                players[i].hit(deck.deal_one(),j+1,playHand);
                                break;
                            }

                            case MOVES::ViewDealer:
                            {
                                std::cout << "\t" << "Here's the dealer's visible card: " << "\n";
                                dealer.showFaceUp();
                                break;
                            }

                            case MOVES::DoubleDown:
                            {
                                if (!players[i].doubleDown(deck.deal_one(),j+1)) {
                                    break;
                                }

                                playHand = false;

                                // only end the player's turn if at the last hand
                                if (j >= players[i].getNumHands() - 1) {
                                    playPlayer = false;
                                }
                                
                                break;
                            }

                            case MOVES::Split:
                            {
                                players[i].split(j+1,deck);

                                // if Aces have been split, prevent the GamePlayer from playing on this hand
                                if (players[i].getSplitRank() == Card::ACE.first) {
                                    playHand = false;
                                    playPlayer = false;
                                    break;
                                }

                                break;
                            }

                            case MOVES::Quit:
                            {
                                std::cout << "\t" << "Do you really want to quit and lose your wager?(Y:N): ";
                                std::string input;
                                std::cin >> input;
                                input[0] = std::toupper(input[0]);

                                // Read user input, and remove GamePlayer from round if needed. Update GamePlayer's 
                                // money by subtracting the wage if he/she decides to leave
                                if (input == "Y") {
                                    std::cout << "\t" << players[i].getName() << " has left the round." << "\n";
                                    roundPlayers.erase(i);
                                    players[i].addMoney(-1 * players[i].getWager(j+1));
                                    playHand = false;
                                    playPlayer = false;
                                } 

                                break;
                            }

                            default:
                            {
                                std::cout << "\t" << "Oops! You have entered an invalid choice!" << "\n";
                                break;
                            }
                        }
                    }

                    playHand = true;
                    std::cout << "\n";
                }

            }
        }
    }

    // once all the GamePlayers have played, play the Dealer's hand
    void playDealerHand() {
        // print the dealers hand value and show both cards
        std::cout << "The dealer reveals his/her Face Down Card: " << "\n";
        std::cout << "\t" <<"Hand: " << dealer.getHandValue() << "\n";
        dealer.showFaceUp();
        dealer.showFaceDown();
        std::cout << "\n";

        // if the dealer's hand value is below 17, add a card from the deck to the  
        // dealers hand 
        while(dealer.getHandValue() < 17) {
            Card c = dealer.addCard(deck.deal_one());
            std::cout << "\t" << "The dealer takes " << c.get_rank() << " of " << c.get_suit() << " for a total value of " << dealer.getHandValue() << "\n";
        }

        if (dealer.getHandValue() > maxPoints) { 
            std::cout << "\t" << "Bust! The dealer's hand value is above 21!" << "\n";
        }

         std::cout << "\n";
     }

     // print round summary
     void roundSummary() {
        //print round summary and increment round counter
        std::cout << "Round " << roundCounter << " Over! Here's what happened: " << "\n";
        roundCounter++;

        // for each GamePlayer..
        for (unsigned int i = 0; i < players.size(); i++) {
            // if the given GamePlayer is still in the round...
            if(roundPlayers.find(i) != roundPlayers.end()) {

                // for each hand of a GamePlayer...
                for (int j = 0; j < players[i].getNumHands(); j++) {
                    // if the dealer's hand value is > 21, then the player wins the wager amount
                    if (players[i].isBust(j+1)) {
                        std::cout << "\t" << players[i].getName() << " loses $" << players[i].getWager(j+1) << " from busting Hand " << std::to_string(j+1) << "\n";
                        players[i].addMoney(-1*players[i].getWager(j+1));
                    } else if (dealer.getHandValue() > maxPoints) {
                        std::cout << "\t" << players[i].getName() << " wins $" << players[i].getWager(j+1) << " from Hand " << std::to_string(j+1) << "\n";
                        players[i].addMoney(players[i].getWager(j+1));

                    // if the GamePlayers hand value > dealer's hand value, then the GamePlayer wins wager amount
                    } else if(players[i].getHandValue(j+1) > dealer.getHandValue()) {
                        std::cout << "\t" << players[i].getName() << " wins $" << players[i].getWager(j+1) << " from Hand " << std::to_string(j+1) << "\n";
                        players[i].addMoney(players[i].getWager(j+1));

                    // if the GamePlayers hand valye < dealer's hand value, then GamePlayer loses wager amount
                    } else if(players[i].getHandValue(j+1) < dealer.getHandValue()) {
                        std::cout << "\t" << players[i].getName() << " loses $" << players[i].getWager(j+1) << " from Hand " << std::to_string(j+1) << "\n";
                        players[i].addMoney(-1*players[i].getWager(j+1));

                    // if GamePlayers hand value == dealer's hand value, GamePlayer neither loses nor wins wager amount
                    } else {
                        std::cout << "\t" << players[i].getName() << " ties the dealer and loses nothing from Hand " << std::to_string(j+1) << "\n";
                    }

                    players[i].removeHand(j+1);
                }

                //reset all hands for a given GamePlayer and remove from round
                players[i].resetHand();
                roundPlayers.erase(i);
            }

            std::cout << "\t" << players[i].getName() << " has $" << players[i].getMoney() << " remaining" << "\n" << "\n";
        }

     }

    bool roundReset() {
        // erase all GamePlayers from vector "players" whose money = 0
        auto delete_it = remove_if(players.begin(), players.end(), pred());
        players.erase(delete_it,players.end());

        // update numPlayers and reset the dealer's hand
        numPlayers = unsigned(players.size());
        dealer.resetHand();

        // if all GamePlayers have 0 money, then the game is over
        if (players.size() == 0) {
            std::cout << "*********************** Game Over ***********************" << "\n";
            std::cout << "Thanks for Playing!" << "\n";
            return false;
        }

        // query if the user would like to play again
        std::string val;
        std::cout << "Play again? (Y:N): ";
        std::cin >> val;
        val[0] = std::toupper(val[0]);

        if (val[0] == 'Y') {
            std:: cout << "\n";
            return true;
        } 

        std::cout << "*********************** Game Over ***********************" << "\n";
        std::cout << "Thanks for Playing!" << "\n";

        return false;
    }

    // runs the entire gameif canPlay if set to true
    void runGame() {
        bool canPlay = true;

        while(canPlay) {
            setupRound();
            placeBets();
            dealCards();
            canPlay = checkBlackJack();

            if (canPlay) {
                runRound();
                playDealerHand();
            }

            roundSummary();
            canPlay = roundReset();
        }
    }


private:
    unsigned int numPlayers;
    std::vector<GamePlayer> players;
    std::unordered_set<unsigned int> roundPlayers;
    CardDeck deck; 
    Dealer dealer;
    int roundCounter = 1;
    static const int maxPoints = 21;
};


#endif