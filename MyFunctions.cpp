#include <iostream>
#include <stdlib.h>
#include <string>
#include "Blackjack.h"
using namespace std;

int generateRandomCard(const int min, const int max) {
	int randomCardValue = (rand() % ((max+1)-min)) + min;
	return randomCardValue;
}

int generateRandomCard(const int min, const int max, const int probability) {
	int ranNum = generateRandomCard(0,100);
	if (ranNum <= probability) {
		return generateRandomCard(10,13);  
	}
	else {
		return generateRandomCard(min,max); 
	}
}

string generateRandomSuit(const int cardValue) {
	string suit;
	string newString;

	int randomNum = (rand() % 4) + 1;

	switch (randomNum) {
	case 1:
		suit = "Spades";
		break;
	case 2:
		suit = "Hearts";
		break;
	case 3:
		suit = "Diamonds";
		break;
	case 4:
		suit = "Clubs";
		break;
	default:
		break;
	}

	if (cardValue > 1 && cardValue < 11) {
		newString = to_string(cardValue) + " of " + suit;
		return newString;
	}
	else {
		switch (cardValue) {
		case 1:
			newString = ("Ace of " + suit);
			break;
		case 11:
			newString = ("Jack of " + suit);
			break;
		case 12:
			newString = ("Queen of " + suit);
			break;
		case 13:
			newString = ("King of " + suit);
			break;
		}

		return newString;
	}
}  

bool isBlackjack(const int pointsTotal) {
	if (pointsTotal == 21) {
		return true; 
	} 
	return false;
} 

void displayOutcome(float playerBalance, const float balanceUpdate, const string whatWon, const bool playerWon, const bool Tie) {
	playerBalance = (playerBalance < 0) ? 0 : playerBalance;

	if (playerWon == true) {
		cout << "\nCongratulations! You " << whatWon << endl;
	}
	else if (Tie == true) {
		cout << "\nYou both Tied!" << endl;
	}
	else {
		cout << "\nDealer has won the round!" << endl;
	}

	printf("Balance Update: +$%.2f\n", balanceUpdate);
	printf("Your new balance is $%.2f.\n", playerBalance);
}

void updatePlayerBalance(Hand& playerHand, const bool playerBlackjack, const bool playerCardWin, const bool computerBlackjack, const bool computerCardWin) {
	bool followedAdvice = (playerHand.playerAction == playerHand.playerAdvice) ? true : false;

	//Blackjack Win Check
	if (playerBlackjack == true) {
		if (computerBlackjack == true) {
			displayOutcome(playerHand.playerBalance, 0, "hit Blackjack!", false, true);
			return;
		}
		else {
			if (followedAdvice == true) {
				playerHand.playerBalance += playerHand.playerWager + (0.5 * playerHand.playerWager);
				displayOutcome(playerHand.playerBalance, (playerHand.playerWager + (0.5 * playerHand.playerWager)), "hit Blackjack!", true, false);
			}
			else {
				playerHand.playerBalance += playerHand.playerWager + (0.5 * playerHand.playerWager) + (0.8 * playerHand.playerWager);
				displayOutcome(playerHand.playerBalance, (playerHand.playerWager + (0.5 * playerHand.playerWager) + (0.8 * playerHand.playerWager)), "hit Blackjack!", true, false);
			}
			return;
		}
	}
	else if (computerBlackjack == true) {
		if (followedAdvice == true) {
			playerHand.playerBalance -= (0.8 * playerHand.playerWager);
			displayOutcome(playerHand.playerBalance, -(0.8 * playerHand.playerWager), "hit Blackjack!", false, false);
		}
		else {
			playerHand.playerBalance -= (1.2 * playerHand.playerWager);
			displayOutcome(playerHand.playerBalance, -(1.2 * playerHand.playerWager), "hit Blackjack!", false, false);
		}
		return;
	}

	//Hand Win Check
	if (playerCardWin == true) {
		if (computerCardWin == true) {
			displayOutcome(playerHand.playerBalance, 0, "won the Hand!", false, true);
			return;
		}
		else {
			if (followedAdvice == true) {
				playerHand.playerBalance += playerHand.playerWager;
				displayOutcome(playerHand.playerBalance, playerHand.playerWager, "won the Hand!", true, false);
			}
			else {
				playerHand.playerBalance += (1.8 * playerHand.playerWager);
				displayOutcome(playerHand.playerBalance, (1.8 * playerHand.playerWager), "won the Hand!", true, false);
			}
			return;
		}
	}
	else if (computerCardWin == true) {
		if (followedAdvice == true) {
			playerHand.playerBalance -= (0.8 * playerHand.playerWager);
			displayOutcome(playerHand.playerBalance, -(0.8 * playerHand.playerWager), "won the Hand!", false, false);
		}
		else {
			playerHand.playerBalance -= (1.2 * playerHand.playerWager);
			displayOutcome(playerHand.playerBalance, -(1.2 * playerHand.playerWager), "won the Hand!", false, false);
		}
		return;
	}

	//Computer Bust Check
	if (computerBlackjack == false && computerCardWin == false) {
		if (followedAdvice == true) {
			playerHand.playerBalance += playerHand.playerWager;
			displayOutcome(playerHand.playerBalance, playerHand.playerWager, "won the Round!", true, false); 
		}
		else {
			playerHand.playerBalance += (1.8 * playerHand.playerWager);
			displayOutcome(playerHand.playerBalance, (1.8 * playerHand.playerWager), "won the Round!", true, false);
		}
		return;
	}
}

void displayGameSummary(const Hand playerHand) {
	printf("\n\n=== GAME SUMMARY ===\nTotal Rounds Played: %i\nTotal Wins (Regular): %i\nTotal Wins (Blackjack): %i\nTotal Losses (Regular): %i\nTotal Losses (Blackjack): %i\nTotal Ties (Regular): %i\nTotal Ties (Blackjack): %i\nFinal Balance: $%.2f\nNet Gain/Loss: $%.2f\n",playerHand.roundsPlayed,playerHand.totalRegularWins,playerHand.totalBlackjackWins,playerHand.totalRegularLosses,playerHand.totalBlackjackLosses,playerHand.totalRegularTies,playerHand.totalBlackjackTies,playerHand.playerBalance,playerHand.netGainAndLoss);
}


