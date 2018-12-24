#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Card.h"
#include "CardDeck.h"
#include <iostream>


class GamePlayer {
public:

	//Constructor
	GamePlayer(const std::string &name_in,int money_in)
	:name(name_in), money(money_in),aceIndex(std::vector<int>(5,-1)),handWager(std::vector<int>(5,0)), 
	handHitStatus(std::vector<bool>(5,false)),bustStatus(std::vector<bool>(5,false)){}

	// Getter Functions
 	const std:: string& getName() const {
 		return name;
 	}

 	int getMoney() const {
 		return money;
 	}

 	int getWager(unsigned int hand) const {
 		return handWager[hand];
 	}

 	int getNumHands() const {
 		return numHands;
 	}

 	int getHandValue(int hand) const {
 		auto it = handValues.find(hand);
 		return it->second;
 	}

 	std::string getSplitRank() const {
 		return splitRank;
 	}

 	// iterate through all hands and print cards
 	void showHands() const {
 		for (int i = 0 ; i < numHands; i++) {
 			auto it = hands.find(i+1);

 			if (it != hands.end()) {
 				std::cout << "\t" <<  "Hand " + std::to_string(i+1) <<": " << getHandValue(i+1) << std::endl;

 				for (unsigned int i = 0; i < it->second.size();i++) {
 					std::cout << "\t" << it->second[i] << std::endl;
 				}
 			}
 		}
 	}

 	//indicate whether user has ANY hands
 	bool handsEmpty() const {
 		if (hands.empty()) {
 			return true;
 		}

 		return false;
 	}

 	// add card to specified hand
 	Card addCard(const Card& c, int hand) {
 		//check whether card to add is an ace
 		if (c.get_value() == Card::ACE.second) {

 			//if adding the card with a value of 1 gives value > 21
 			// add an ace value of 1
 			if (maxPoints - (handValues[hand] + Card::ACE.second) < 0) {
 				handValues[hand] += Card::ACE.second;
 				hands[hand].push_back(c);

 			//if adding the card with a value of 11 gives a value < 0
 			// add an ace with value 1
 			} else if (maxPoints - (handValues[hand] + Card::ACE11.second)< 0) {
 				handValues[hand] += Card::ACE.second;
 				hands[hand].push_back(c);

 			// add an ace with value 11
 			} else {
 				handValues[hand] += Card::ACE11.second;
 				hands[hand].push_back(Card(Card::ACE11,c.get_suit()));
 			}

 			// keep track of what index in the specified hand the FIRST ace
 			// was added
 			if (aceIndex[unsigned(hand)] == -1) {
 				aceIndex[unsigned(hand)] = int(hands[hand].size()-1);
 			}

 		} else {
 			//check whether an ace is in the current hand
 			// if not add in the card to the specified hand
 			if (aceIndex[unsigned(hand)] == -1) {
 				handValues[hand] += c.get_value();
 			} else {

 				// get the ace in the specified hand
 				Card ace = hands[hand][unsigned(aceIndex[unsigned(hand)])];

 				//find the value of the hand without the ace and with
 				// the new card
 				int valNoAce = handValues[hand] - ace.get_value();
 				valNoAce += c.get_value();

 				// if adding an ace with a 11 gives a hand value <= 21
 				// add the ace with value 11
 				if (maxPoints - (valNoAce + Card::ACE11.second) >= 0) {

 					handValues[hand] = valNoAce + Card::ACE11.second;
 					hands[hand][unsigned(aceIndex[unsigned(hand)])] = Card(Card::ACE11,ace.get_suit());

 				} else {

 					//add the ace with value of 1
 					handValues[hand] = valNoAce + Card::ACE.second;
 					hands[hand][unsigned(aceIndex[unsigned(hand)])] = Card(Card::ACE,ace.get_suit());
 				}
 			}

 			hands[hand].push_back(c);
 		}

		return c;
 	}

 	// place a bet
 	void placeBet(int val,unsigned int hand) {
		handWager[hand] = val;
 	}

 	// add to money
 	void addMoney(int val) {
 		money += val;
 	}

 	// split current hand
 	void split(int hand, CardDeck& deck) {
 		//check edge cases 
 		if (handHitStatus[hand]) {
 			std::cout << "\t" << "Oops! You cannot split once you hit!" << std::endl << std::endl;
 			return;
 		}

 		if (hands[hand][0].get_value() != hands[hand][1].get_value()) {
 			if (hands[hand][0].get_value() != Card::ACE11.second || hands[hand][0].get_value() != Card::ACE.second) {
 				std::cout << "\t" << "Oops! You have attempted an illegal split!" << std::endl << std::endl;
 			}
 			return;
 		}

 		if (money - handWager[hand] < 0) {
 			std::cout << "\t" <<"Oops! You do not have enough money to split!" << std::endl << std::endl;
 			return;
 		}

 		if (numSplit > 0) {
 			std::cout << "\t" << "Oops! You cannot split more than once!" << std::endl << std::endl;
 			return;
 		}

 		if (money - 2*handWager[hand] < 0) {
 			std::cout << "\t" << "Oops! Your wager is too high to split!" << std::endl << std::endl;
 			return;
 		}

 		//grab last card from selected hand
 		Card c = hands[hand].back();
 		std::cout << "\t" << "You have split " << c.get_rank() << "'s and placed an equal wager on the second hand" << std::endl;

 		//remove last card from hand and update the value of the hand
		hands[hand].pop_back();
		handValues[hand] -= c.get_value();

		numHands++;

		//create a new hand in the "hands" has table
		hands.insert({numHands,std::vector<Card>()});

		//insert card c into the newly created hand and update the hand value
		hands[numHands].push_back(c);
		handValues.insert({numHands,c.get_value()});

		//update the wager for the newly created hand
		handWager[numHands] += handWager[hand];

		//increment the number of splits and assign splitRank to c's rank
		numSplit++;
		splitRank = c.get_rank();

		//add the next card from the deck to the original hand
		c = addCard(deck.deal_one(),hand);
		std::cout << "\t" << c << " was added to Hand " << std::to_string(hand) << " for a total value of " << getHandValue(hand) << std::endl;

		//add the next card from the deck to the newly created hand
		c = addCard(deck.deal_one(),numHands);
		std::cout << "\t" << c << " was added to Hand " << std::to_string(numHands) << " for a total value of " << getHandValue(numHands) << std::endl << std::endl;

		//check whether hand value of original and newly created  hand is > 21
      	if (getHandValue(hand) > maxPoints) {
          std::cout << "\t" << "Bust! Hand " << std::to_string(hand) << " has a value above 21! You lose $" << getWager(hand) << "." << std::endl;
          bustStatus[hand] = true;
      	}

      	if (getHandValue(numHands) > maxPoints) {
          std::cout << "\t" << "Bust! Hand " << std::to_string(numHands) << " has a value above 21! You lose $" << getWager(numHands) << "." << std::endl;
          bustStatus[hand] = true;
      	}

 	}

 	// take a hit
 	void hit(const Card& c, int hand, bool& playHand) {
 		//add next card in deck to specified hand
 		addCard(c,hand);
        std::cout << "\t" << "You have added " << c << " to Hand " << std::to_string(hand) << " for a total value of " << getHandValue(hand) << std::endl << std::endl;

        // update hit status
        handHitStatus[hand] = true;

        // check if new hand value is greater than 21, and update bust status if needed
        if (getHandValue(hand) > maxPoints) {
            std::cout << "\t" << "Bust! Your Hand " << std::to_string(hand) << " value is above 21! You lose $" << getWager(hand) << std::endl;
            bustStatus[hand] = true;
            playHand = false;
        }
 	}

 	// double down on current hand
 	bool doubleDown(const Card& c, int hand) {
		// check whether wage is low enough to double down
		if (2*getWager(hand) > getMoney()) {
	        std::cout << "\t" << "Oops! Your wager is too high to double down." << std::endl << std::endl;
	        return false;
	    }

	    // check whether player has hit on this hand
	    if (handHitStatus[hand]) {
 			std::cout << "\t" << "Oops! You cannot double down once you hit!" << std::endl << std::endl;
 			return false;
 		}

	    std::cout << "\t" << "You doubled down and increased your wager to $" << 2*getWager(hand) << std::endl;

	    // add next card in deck to hand and update hand's wager
	    addCard(c,hand);
		handWager[unsigned(hand)] += handWager[unsigned(hand)];
	    std::cout << "\t" << "You take a " << c << " for a total value of " << getHandValue(hand) << std::endl;

	    // check if new hand value is greater than 21, and update bust status if needed
	    if (getHandValue(hand) > maxPoints) {
	        std::cout << "\t" << "Bust! Your hand value is above 21! You lose $" << getWager(hand) << std::endl;
	        bustStatus[hand] = true;
	    }

	    return true;
 	}

 	//removes a single hand from "hands" hash table
 	void removeHand(int hand) {
 		handWager[unsigned(hand)] = 0;
 		handValues.erase(hand);
 		aceIndex[unsigned(hand)] = -1;
 		handHitStatus[hand] = false;
 		bustStatus[hand] = false;
 		hands.erase(hand);
 	}

 	//check whether a given hands exists in "hands" hash table
 	bool handExists(int hand) {
 		if (hands.find(hand) == hands.end()) {
 			return false;
 		} else {
 			return true;
 		}
 	}

 	//checks whether hand is bust
 	bool isBust(int hand) {
 		return bustStatus[hand];
 	}

 	//resets ALL hands in "hands" hash table
 	void resetHand() {
 		std::fill(handWager.begin(),handWager.end(),0);
 		std::fill(aceIndex.begin(),aceIndex.end(),-1);
 		std::fill(handHitStatus.begin(),handHitStatus.end(),false);
 		std::fill(bustStatus.begin(),bustStatus.end(),false);
 		numHands = 1;
 		numSplit = 0;
 		splitRank = "";
 		handValues.clear();
  		hands.clear();
 	}

private:
	std::string name;
 	int money; 
 	int numSplit = 0;
 	std::string splitRank = "";
 	int numHands = 1;
 	std::unordered_map<int,int> handValues; 
 	std::unordered_map<int,std::vector<Card>> hands; 
 	std::vector<int>aceIndex;
 	std::vector<int>handWager;
 	std::vector<bool> handHitStatus;
 	std::vector<bool> bustStatus;
 	static const int maxPoints = 21;
};

#endif