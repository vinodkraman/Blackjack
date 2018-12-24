#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <getopt.h>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <string>
#include "GamePlayer.h"
#include "CardDeck.h"
#include "Dealer.h"
#include "Blackjack.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main(int argc, char * argv[]) {

	//Create Command Line Help Flag
	ios_base::sync_with_stdio(false);
    srand (unsigned(time(NULL)));
    struct option longOpts[] = {
        {"help",no_argument,NULL,'h'}
    };

    opterr = false;

    int opt = 0, index = 0;
    while ((opt = getopt_long(argc, argv, "h", longOpts, &index)) != -1){

        switch (opt) {

            case 'h':
                cout << "A summary of each command is provided below.\n\n";
                cout << "  P, Pass:            Do NOT take a card and end turn.\n";
                cout << "\n";
                cout << "  H, Hit:             Request a card from the Dealer.\n";
                cout << "\n";
                cout << "  D, Double Down:     Double your wager and add an extra card to your hand.\n";
                cout << "                      No additional cards may be requested after this point.\n";
                cout << "\n";
                cout << "  S, Split:           Split hand into two seperate hands. Note, this can\n";
                cout << "                      only be done with a pair. An equal wager will be placed\n";
                cout << "                      on the second hand. If an Ace is split, only one additional\n";
                cout << "                      card is able to be added to either hand. Each hand is treated\n";
                cout << "                      seperately, and a maximum of two splits can be performed in a \n";
                cout << "                      single round.\n";
                cout << "\n";
                cout << "  V, View Dealer's\n";
                cout << "     Hand:            View the Dealer's Hand\n";
                cout << "\n";
                cout << "  Q, Quit Round:      Quit the round prematurely. Your wage will be forfeited.\n";
                cout << "\n";
                cout << "Need more help? Please view the README.\n";
                return(0);
                break;

            default:
                cout << "I didn't recognize one of your flags!\n";
                    exit(1);
        }
    }

    //Create Blackjack Object
    Blackjack blackjack;
    blackjack.runGame();
}