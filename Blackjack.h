#include <iostream>
#include <stdlib.h>
using namespace std;

#ifndef BLACKJACK_H
#define BLACKJACK_H

/*FUNCTION PROTOTYPES --- used by structs*/ 

//Gen: generates a random number between and including the min and max.
//Pre: None
//Post: Returns a number between min and max inclusive.
int generateRandomCard(const int min, const int max);

//Gen: Generates a random number between min and max BUT has a chance to generate a random number above and equal to 10.
//Pre: Must only be called if the user is lucky (aka a 15% chance to be called).
//Post: returns either a number between min and max or 10 to 13 all inclusive.
int generateRandomCard(const int min, const int max, const int probability);

//Gen: Gives a card value a random suit (EX: 2 of Spades, 10 of Diamonds, Ace of Hearts)
//Pre: cardValue must be between 1 and 13 inclusive.
//Post: returns a string containing the card value and suit.
string generateRandomSuit(const int cardValue);

/*STRUCTS*/
struct Hand {
	//Variables
	string playerName;

	float playerWager = 0;
	float playerBalance = 0;

	int playerAces = 0;
	int playerTotal = 0;
	int playerActualTotal = 0;

	//Cards
	int playerCardValue1 = 0;
	int playerCardValue2 = 0;

	string playerCardSuit1;
	string playerCardSuit2;

	string playerHand;
	string playerAdvice;
	string playerAction = "";

	//EndGame Variables
	int roundsPlayed = 0;
	int totalRegularWins = 0;
	int totalBlackjackWins = 0;
	int totalRegularLosses = 0;
	int totalBlackjackLosses = 0;
	int totalRegularTies = 0;
	int totalBlackjackTies = 0;
	float netGainAndLoss = 0;

	//functions
private:

	void Hit() {
		int luckyChance = generateRandomCard(0, 100); 
		int newCard = (luckyChance <= 15) ? generateRandomCard(1, 13, generateRandomCard(0, 100)) : generateRandomCard(1, 13);
		string newSuit = generateRandomSuit(newCard);

		playerActualTotal += (newCard == 1) ? 11 : newCard;
		playerAces += (newCard == 1) ? 1 : 0;
		playerTotal += ((newCard == 1) ? (newCard + 10) : ((newCard > 10) ? 10 : newCard));

		playerHand += ", " + newSuit;
	}

	void doubleDown() {
		int newCard = generateRandomCard(1, 13);
		string newSuit = generateRandomSuit(newCard);

		playerActualTotal += (newCard == 1) ? 11 : newCard;
		playerAces += (newCard == 1) ? 1 : 0;
		playerTotal += ((newCard == 1) ? (newCard + 10) : ((newCard > 10) ? 10 : newCard));

		playerHand += ", " + newSuit;

		playerWager *= 2;
	}

public:

	void printHand() {
		cout << playerName << "'s Hand: " << playerHand << endl;
	}

	void playerTurn(bool& endRound) {
		int playerChoice;

		cout << "\n=== CHOOSE AN OPTION ===\n(1) to Hit\n(2) to Stand\n(3) to Double Down\n\tCURRENT TOTAL: " << to_string(playerTotal) << endl;
		cin >> playerChoice;

		while ((playerChoice < 1 || playerChoice > 3) || (playerChoice == 3 && playerWager * 2 > playerBalance)) {
			cout << "Please enter a valid choice!" << endl;
			cout << "\n=== CHOOSE AN OPTION ===\n(1) to Hit\n(2) to Stand\n(3) to Double Down\n\tCURRENT TOTAL: " << to_string(playerTotal) << endl;
			cin >> playerChoice;
		}

		switch (playerChoice) {
		case 1:
			playerAction = (playerAction == "") ? "Hit" : playerAction;
			Hit();
			break;
		case 2:
			playerAction = (playerAction == "") ? "Stand" : playerAction;
			endRound = false;
			break;
		case 3:
			if (playerAction == "Hit") {
				cout << "\nCannot Double Down After Hitting!\n" << endl;
			}
			else {
				playerAction = (playerAction == "") ? "Double Down" : playerAction;
				doubleDown(); 
				endRound = false; 
			}
			break;
		default:
			break;
		}

		if (playerTotal == 21) {
			endRound = false;
		}
		else {
			while (playerTotal > 21) {
				if (playerAces > 0) {
					playerTotal -= 10;
					playerAces -= 1;
				}
				else {
					endRound = false;
					break;
				}
			}
		}

		printHand();

	}

	void reset() {
		playerWager = 0;
		playerAces = 0;
		playerTotal = 0;
		playerActualTotal = 0; 
		playerAdvice = ""; 
	}

};

/*FUNCTION PROTOTYPES --- not used by structs*/

//Gen: Will return whether or not the someone has hit blackjack.
//Pre: None
//Post: Will return true if blackjack (21) or false if anything but that.
bool isBlackjack(const int pointsTotal);

//Gen: Displays onto the screen the outcome of the round.
//Pre: None.
//Post: Displays onto screen the player's new balance, their balance change, and who won/how.
void displayOutcome(const float playerBalance, const float balanceUpdate, const string whatWon, const bool playerWon, const bool Tie); 

//Gen: Updates the balance of the player according to how they won and whether they followed the advice given.
//Pre: None.
//Post: Will modify the players Balance according to the rules given.
void updatePlayerBalance(Hand& playerHand, const bool playerBlackjack, const bool playerCardWin, const bool computerBlackjack, const bool computerCardWin);

//Gen: Displays the summary of the game.
//Pre: None.
//Post: Will Display what games the player won/lost and how, how many round they played, their balance, how much they lost/won, as well as a goodbye message.
void displayGameSummary(const Hand playerHand); 

/*FUNCTION TEMPLATES*/

//Gen: Gives differing advice to the player according to if they are lucky or not.
//Pre: None.
//Post: Will return what advice was given in a string (EX: "Hit" "Stand" "Double Down")
template <typename T>
string adviseOptimalOptionOnLuck(const T probability_threshold, const Hand playerHand, const Hand dealerHand) {
	int adviceLuck = generateRandomCard(0, 100);

	if (adviceLuck >= probability_threshold) {
		if (playerHand.playerTotal <= 11) {
			if ((dealerHand.playerCardValue2 >= 7 && playerHand.playerCardValue1 == 1) || (dealerHand.playerCardValue2 >= 7 && playerHand.playerCardValue2 == 1)) {
				cout << "Game Advice: Hit" << endl;
				return "Hit";
			}
			else if (playerHand.playerTotal >= 9 || playerHand.playerCardValue1 == 1 || playerHand.playerCardValue2 == 1) {
				cout << "Game Advice: Double Down" << endl; 
				return "Double Down";
			}
			cout << "Game Advice: Hit" << endl; 
			return "Hit";
		}
		else if (playerHand.playerTotal >= 12 && playerHand.playerTotal <= 16) {
			if (dealerHand.playerCardValue2 >= 7 && playerHand.playerCardValue1 != 1 && playerHand.playerCardValue2 != 1) {
				cout << "Game Advice: Hit" << endl;
				return "Hit";
			}
			else {
				cout << "Game Advice: Stand" << endl; 
				return "Stand";
			}
		}
		else if (playerHand.playerTotal >= 17) {
			cout << "Game Advice: Stand" << endl; 
			return "Stand";
		}
	}
	else {
		int ranNum = generateRandomCard(1, 3);
		switch (ranNum) {
		case 1:
			cout << "Game Advice: Hit" << endl; 
			return "Hit";
			break;
		case 2:
			cout << "Game Advice: Stand" << endl;
			return "Stand";
			break;
		case 3:
			cout << "Game Advice: Double Down" << endl; 
			return "Double Down";
			break;
		default:
			break;
		}
	}
}

#endif