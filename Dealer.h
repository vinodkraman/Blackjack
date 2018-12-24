#ifndef DEALER
#define DEALER

#include <string>
#include <vector>
#include <unordered_map>
#include "Card.h"

class Dealer {
public:

	//Constructor
	Dealer()
	:handValue(0) {};

	// add card to specified hand
	// see GamePlayer.h for details
	Card addCard(const Card& c) {
		if (c.get_value() == Card::ACE.second) {
			if (maxPoints - (handValue + Card::ACE.second) < 0) {
 				handValue += Card::ACE.second;
 				hand.push_back(c);
 			} else if (maxPoints - (handValue + Card::ACE11.second)< 0) {
 				handValue += Card::ACE.second;
 				hand.push_back(c);
 			} else {
 				handValue += Card::ACE11.second;
 				hand.push_back(Card(Card::ACE11,c.get_suit()));
 			}

 			if (aceIndex == -1) {
 				aceIndex = int(hand.size()-1);
 			}

		} else {

			 if (aceIndex == -1) {
 				handValue += c.get_value();
 			} else {
 				Card ace = hand[unsigned(aceIndex)];
 				int valNoAce = handValue - ace.get_value();
 				valNoAce += c.get_value();

 				if (maxPoints - (valNoAce + Card::ACE11.second) >= 0) {
 					handValue = valNoAce + Card::ACE11.second;
 					hand[unsigned(aceIndex)] = Card(Card::ACE11,ace.get_suit());
 				} else {
 					handValue = valNoAce + Card::ACE.second;
 					hand[unsigned(aceIndex)] = Card(Card::ACE,ace.get_suit());
 				}
 			}
 			
 			hand.push_back(c);
		}

		return c;
 	}

 	// return current hand value
 	int getHandValue() const {
 		return handValue;
 	}

 	// only show face up card
 	void showFaceUp() {
 		std::cout << "\t" << hand[0] << std::endl;
 	}

 	// show face down card
 	 void showFaceDown() {
 		std::cout << "\t" << hand[1] << std::endl;
 	}

 	// show all cards
 	void showCards() {
 		std::cout << "\t" << hand[0] << std::endl;
 		std::cout << "\t" << hand[1] << std::endl;
 	}

 	// reset current hand
 	void resetHand() {
 		handValue = 0;
 		hand.clear();
 	}

private:
	int handValue;
	std::vector<Card> hand;
	int aceIndex = -1;
	static const int maxPoints = 21;
};


#endif