
Welcome to Blackjack! 

I. Introduction/Rules
---------------------
This program simulates a simple game of Las Vegas Casino Blackjack on the command-line. Using a standard 52-card pack, each participant attempts to beat the Dealer("The House") by getting a count as close to 21 as possible, without going over 21. Number cards are worth their face value, Face cards are worth 10 points, and Aces are worth either 1 or 11, up to the player's discretion. Before the deal begins, each player places a bet in front of him/her. Once all players have placed their bets, the dealer gives one card face up to each player in the rotation clockwise, and then one card face up to him/herself. Another round of cards is then death face up to each player, but the dealer this time takes his/her second card face down. In summary, each player except the dealer received two cards face up, but the dealer receives one card face up and one card face down. 

If a player's first two cards count to 21, a blackjack has been achieved. If any player has a blackjack and the dealer does not, the dealer immediately pays the player one and a half times the amount of his bet. If the dealer has a blackjack, he immediately collects the bets of all players who do not have blackjacks. If the dealer and another player both have naturals, the best of the player is a tie, and the player neither wins nor loses any money. 

Once all blackjacks have been checked, the round is ready to be played. The player to the left goes first and must decided whether to pass(not ask for another card) or hit(ask for another card).  If the player hits and the count goes over 21, then the player loses and the dealer collects the bet wagered. The dealer then turns to the next player to his left and serves him in the same manner. 

After the dealer has served every player, his face-down card is turned up. If the total is 17 or more, he must stand. If the total is 16 of under, he just take a card. The dealer must continue taking cards until is total is 17 or more, at which the dealer will then pass.

If a player's first two cards are of the same denomination, he may choose to treat them as two separate hands when his turn comes around. The amount of his original bet goes on each hand. The two hands are treated separately, and the dealer settles with each on its own merits. With a pair of aces, the player is given one card for each ace and may not draw again. 

Another option to the player is doubling his bet. When the player's turn comes, he places a bet equal to the original bet, and the dealer gives him just one card. Note that the dealer does not have the option of splitting or doubling down. 

A bet once paid and collected is never returned. If the player goes bust, he has already lost his wager, even if the dealer goes bust as well. If the dealer goes over 21, he pays each player who has stood the amount of the player's bet. If the dealer stands at 21 or less, he pays the bet of any player having a higher total(not greater than 21) and collects the bet of any player having a lower total. If a tie is achieved, the player neither wins nor loses money. 

II. Build Dependencies
----------------------
Blackjack(v1.0.0) was written in c++11 and compiled on Apple LLVM version 10.0.0 (clang-1000.11.45.5) AND Linux g++ (GCC) 6.2.0. This project was NOT tested/built on any other platforms.

III. Library Dependencies
-------------------------
GLIBCXX_3.4.21 required. 

IV. File List
-------------
README 				This File
Blackjack.h			Blackjack header 
Card.h				Card header
CardDeck.h			CardDeck header
Dealer.h			Dealer header
GamePlayer.h		GamePlayer header
blackjack.cpp	    Driver Program
Makefile			Makefile for driver
sampleRound.txt     Output from a sample game
blackjack           Executable

V. Game Assumptions
---------------------
The program was written using the following assumptions:

1) Starting amount of $1000 for each player
2) Maximum of 7 players allowed per round
3) 6 shuffled 52-card packs
4) Maximum of 1 split per round per player
5) Cards of the same VALUE (i.e 10 and Q) can be split
6) Only integer bets are allowed
7) Double Down and Split only allowed on the original hand
8) Double Down can be performed after a split

VI. Running the Driver Program(blackjack.cpp)
--------------------------------------------
A Makefile for blackjack.cpp has been provided to aid compilation. To create an executable, navigate to the working directory using the command-line, and type "make". To run the Blackjack program, enter "./blackjack" to the command line. Follow the visible instructions to play the game. Possible game commands are located in parenthesis. Note, that using a -h flag after the executable name, that is "./blackjack -h", will present a help page from which a summary of the basic game commands can be viewed. To clean the executable, enter "make clean" into the command line. Note, that if the executable is "cleaned", it will need to be recompiled before launch using "make". Up to 7 players may play at once. Each player will perform a game command using the command-line. 

VII. Valid User Inputs
----------------------
The program is heavily dependent on user inputs from the command-line. According, it it is critical
that valid inputs are provided. The following table summarizes valid inputs for each user query:

       Query			  Valid Input		 Sample Invalid Input
====================   =================	======================		
1) Number of players   Positive Integers	0, 8, -5, "assad" , 5.5
   at the table?	    1 - 7 (inc.)


2) Player Names?	        All						None


3) Place Bet?	       Positive Integers	    0, -5, "assad" , 5.5
		            1 - Money Capacity (inc.)


4) What will you do   "P","H","S","D",V",Q"	     "dfdsf",0,-2
   with Hand __?	  "p","h","s","d","v",q"


5) Play again?		      "Y","N"		  		"a",6,7,0,-2
						  "y","n"

6) Do you really want 	  "Y","N"		     	 a",6,7,0,-2
   to quit and lose your  "y","n"
   wager?

Output from a sample round has been provided in "sampleRound.txt".

VIII.Implementation
-------------------
A standard playing card is represented by a Card class. A Card object keeps track of the following Information:

- Card rank
- Card suit

 A Card Deck is represented by a CardDeck class. A CardDeck object keeps track of the following information:

- Cards in deck(represented by a vector of type Cards)
- Size of deck
- Pointer to next card to deal in the deck

The CardDeck class is capable of the following functions:

- Construct a standard 52-card deck(custom constructor that adds the standard 52 Cards to the vector in ascending rank from Spades, to Hearts, to Clubs, and finally Diamonds)
- Shuffling Cards using an in-shuffle algorithm sillier to https://en.wikipedia.org/wiki/In_shuffle
- Dealing a single card to a user by returning the Card in the vector pointed to by the "next" pointer and incrementing "next" pointer to the next index in the vector


A player that is not a dealer is represented by a GamePlayer class. A GamePlayer object keeps track of the following information:

- Player name
- Amount of Money
- Number of Splits
- Rank of Card Split
- Number of Hands
- Value of each hand
- Cards in each hand
- The bust status of each hand
- The hit status of each hand
- Index of Ace in each hand
- Bet/Wager for each hand

The GamePlayer class is capable of the following functions:

- Return any of the information listed above 
- Print Player Hands to command-line
- Indicate whether the all player hands are empty
- Add a card to a specified hand
- Place a bet on a specified hand
- Add to/Subtract from total amount of money
- Split a given hand
- Hit on a specified hand
- Double Down on a specified hand
- Remove a hand once complete
- Reset all hands to defaults

A dealer is represented by a Dealer class, and has very similar functionality to the GamePlayer Class. 

The Blackjack interface is represented by the Blackjack Class. A Blackjack object keeps track of the following information:

- Number of players
- Players
- Players in a given round
- Card deck
- Dealer
- Round Number

The Blackjack class performs the following functions in the given order below:

- Read in the number of players
- Allow users to place bets and read player bets
- Deal cards to players
- Check for Blackjacks before actual round starts
- Run a Blackjack round through all players
- Play the Dealer's hand once the round is complete
- Print a summary of the round, indicating amount of money left over for each player
- Reset the round and ask user whether they want to continue playing

IX. Player Commands
-------------------
P                   Pass
H                   Hit
D                   Double Down
S                   Split
V                   View Dealer's Hand
Q                   Quit Round

X. References
-------------
https://www.bicyclecards.com/how-to-play/blackjack/
https://www.pagat.com/banking/blackjack.html