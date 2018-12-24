#ifndef CARDDECK_H
#define CARDDECK_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "Card.h"

class CardDeck {
public:
	//Initializes deck using 6 standard 52-card packs
	//Cards are added in order of increasing rank for each suit
	CardDeck()
	:next(0){ 

		cards.resize(52*6);
		unsigned int counter = 0;

		for (unsigned int j = 0; j < 6; j++) {
			for (unsigned int i = 0; i < NUM_SUITS; i++) {
				for (unsigned int j = 0; j < NUM_RANKS;j++) {
					cards[counter] = Card(RANKS[j],SUITS[i]);
		    		++counter;
				}
			}
		}
	}

	//Returns next card in deck and incremenets next index
	Card deal_one() {
		Card c = cards[next];
		next = next + 1;
		return c;
	}

	// Resets next index to first card in the deck
	void reset() {
		next = 0; 
	}

	//helper function for shuffle
	void switch_element(std::vector<Card>& cards, unsigned int startingIndex, unsigned int numSwitches) {
		for(unsigned int i = 0; i < numSwitches; i++) {
		  Card leftCard = cards[(startingIndex - i) - 1];
		  cards[(startingIndex - i) - 1] = cards[startingIndex - i];
		  cards[startingIndex - i] = leftCard; 
		}
	}

	//shuffles deck using an in-shuffle. See
	// https://en.wikipedia.org/wiki/In_shuffle.
	void shuffle(unsigned int seed) {

		for(unsigned int j = 0; j < seed; j++) {
		   for (unsigned int i = deckSize/2; i < deckSize;i++) {
		      switch_element(cards,i,deckSize - i);
		  }
		}

		reset();
	}

	//returns true if no more cards left in deck
	bool empty() const {
		return (next > deckSize-1);
	}

private:

	static const int deckSize = 52*6;
	std::vector<Card> cards;
	unsigned int next; // index of next card to be dealt
};

#endif