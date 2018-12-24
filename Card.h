#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Card {
public:

	//suits
	static constexpr const char* const SUIT_SPADES = "Spades";
	static constexpr const char* const SUIT_HEARTS = "Hearts";
	static constexpr const char* const SUIT_CLUBS = "Clubs";
	static constexpr const char* const SUIT_DIAMONDS = "Diamonds";

	//ranks
	static constexpr auto TWO = std::make_pair("Two",2);
	static constexpr auto  THREE = std::make_pair("Three",3);
	static constexpr auto  FOUR = std::make_pair("Four",4);
	static constexpr auto  FIVE = std::make_pair("Five",5);
	static constexpr auto  SIX = std::make_pair("Six",6);
	static constexpr auto  SEVEN = std::make_pair("Seven",7);
	static constexpr auto  EIGHT = std::make_pair("Eight",8);
	static constexpr auto  NINE = std::make_pair("Nine",9);
	static constexpr auto  TEN = std::make_pair("Ten",10);
	static constexpr auto  JACK = std::make_pair("Jack",10);
	static constexpr auto  QUEEN = std::make_pair("Queen",10);
	static constexpr auto  KING = std::make_pair("King",10);
	static constexpr auto  ACE = std::make_pair("Ace",1);
	static constexpr auto  ACE11 = std::make_pair("Ace",11);

	//Initializes Card to Two of Spades
	Card()
	:rank(TWO), suit(SUIT_SPADES){};

	//Initialize Card to specified rank, suit and value
	Card(const std::pair<std::string,int> &rank_in, const std::string &suit_in)
	:rank(rank_in), suit(suit_in) {};

	//Returns the rank
  	std::string get_rank() const { 
  		return rank.first;
  	}

  	//Returns the suit
  	std::string get_suit() const {
  		return suit;
  	}

  	//Returns the value
  	int get_value() const {
  		return rank.second;
  	}


private:

	std::pair<std::string,int> rank;
	std::string suit;
};

	//Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
	os << card.get_rank() << " of " << card.get_suit(); 
	return os;
}

// Suits in consideration
constexpr const char* const SUITS[] = {
  Card::SUIT_SPADES,
  Card::SUIT_HEARTS,
  Card::SUIT_CLUBS,
  Card::SUIT_DIAMONDS
};
const int NUM_SUITS = 4;

// Cards in consideration
const std::vector<std::pair<std::string,int>> RANKS = {
  Card::TWO,
  Card::THREE,
  Card::FOUR,
  Card::FIVE,
  Card::SIX,
  Card::SEVEN,
  Card::EIGHT,
  Card::NINE,
  Card::TEN,
  Card::JACK,
  Card::QUEEN,
  Card::KING,
  Card::ACE
};
const int NUM_RANKS = 13;

#endif