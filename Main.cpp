// Programmer: Evan Richard
// Date: 4/4/24
// File: Main.cpp
// Assignment: HW5
// Purpose: Main file containing Blackjack Game!!!

#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "Blackjack.h"
using namespace std;

int main() {
	/*GAME SETUP*/
	srand(time(0));
	
	cout << "Welcome to CodeJack: The Blackjack Simulator!" << endl;

	cout << "Enter player name: ";

	Hand playerHand;
	cin >> playerHand.playerName; 
	float startingBalance = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 901.0f)) + 100.0f;
	playerHand.playerBalance = startingBalance;

	Hand dealerHand; 
	dealerHand.playerName = "Dealer"; 

	printf("\nStarting balance: $%.2f\n\n*****THE GAMES BEGIN! GOODLUCK!*****\n", playerHand.playerBalance);

	/*GAME BEGINS*/
	bool continueGame = true;
	while (playerHand.playerBalance >= 0.01 && continueGame == true) {
		playerHand.roundsPlayed += 1;

		cout << "\nROUND " << playerHand.roundsPlayed << "\n*******\n" << endl;

		printf("Player balance: $%.2f\nEnter an amount to wager: ", playerHand.playerBalance);
		cin >> playerHand.playerWager;

		while (playerHand.playerWager <= 0 || playerHand.playerWager > playerHand.playerBalance) {
			printf("Please enter a valid amount between 0 and your balance of %.2f: ", playerHand.playerBalance);  
			cin >> playerHand.playerWager; 
		}

		/*INITIAL CARD DRAWING*/
		int luckyChance = generateRandomCard(0, 100);

		playerHand.playerCardValue1 = (luckyChance <= 15) ? generateRandomCard(1,13,generateRandomCard(0,100)) : generateRandomCard(1, 13); 
		playerHand.playerCardValue2 = (luckyChance <= 15) ? generateRandomCard(1,13,generateRandomCard(0,100)) : generateRandomCard(1, 13);  
		playerHand.playerCardSuit1 = generateRandomSuit(playerHand.playerCardValue1);
		playerHand.playerCardSuit2 = generateRandomSuit(playerHand.playerCardValue2);
		playerHand.playerHand = playerHand.playerCardSuit1 + ", " + playerHand.playerCardSuit2;

		dealerHand.playerCardValue1 = generateRandomCard(1, 13); 
		dealerHand.playerCardValue2 = generateRandomCard(1, 13); 
		dealerHand.playerCardSuit1 = generateRandomSuit(dealerHand.playerCardValue1); 
		dealerHand.playerCardSuit2 = generateRandomSuit(dealerHand.playerCardValue2); 
		dealerHand.playerHand = dealerHand.playerCardSuit1 + ", " + dealerHand.playerCardSuit2; 

		playerHand.playerActualTotal += (((playerHand.playerCardValue1 == 1) ? 11 : playerHand.playerCardValue1) + ((playerHand.playerCardValue2 == 1) ? 11 : playerHand.playerCardValue2));
		playerHand.playerAces += (((playerHand.playerCardValue1 == 1) ? 1 : 0) + ((playerHand.playerCardValue2 == 1) ? 1 : 0));
		playerHand.playerTotal += (((playerHand.playerCardValue1 > 10) ? 10 : playerHand.playerCardValue1) + ((playerHand.playerCardValue2 > 10) ? 10 : playerHand.playerCardValue2) + (10*playerHand.playerAces));
	
		dealerHand.playerActualTotal += (((dealerHand.playerCardValue1 == 1) ? 11 : dealerHand.playerCardValue1) + ((dealerHand.playerCardValue2 == 1) ? 11 : dealerHand.playerCardValue2)); 
		dealerHand.playerAces += (((dealerHand.playerCardValue1 == 1) ? 1 : 0) + ((dealerHand.playerCardValue2 == 1) ? 1 : 0)); 
		dealerHand.playerTotal += (((dealerHand.playerCardValue1 > 10) ? 10 : dealerHand.playerCardValue1) + ((dealerHand.playerCardValue2 > 10) ? 10 : dealerHand.playerCardValue2) + (10 * dealerHand.playerAces)); 
		
		playerHand.printHand();   
		cout << "Dealer's Hand: " << dealerHand.playerCardSuit1 << ", [HIDDEN CARD]" << endl; 

		/*CHECK FOR INITIAL PLAYER BLACKJACK*/
		if (isBlackjack(playerHand.playerTotal) == true) { 

			cout << "\n----- Dealer Reveals ----- \n"; 
			dealerHand.printHand();

			if (isBlackjack(dealerHand.playerTotal) == true) {
				playerHand.totalBlackjackTies += 1;
				updatePlayerBalance(playerHand, true, false, true, false);

				playerHand.reset(); 
				dealerHand.reset(); 

				if (playerHand.playerBalance >= 0.01) {
					char playAgain;
					cout << "\nPlay another round? (Y/N): "; 
					cin >> playAgain; 

					continueGame = (playAgain == 'Y') ? true : false; 
				} 
				continue;

			}
			else {
				playerHand.totalBlackjackWins += 1;
				updatePlayerBalance(playerHand, true, false, false, false);

				playerHand.reset(); 
				dealerHand.reset(); 

				if (playerHand.playerBalance >= 0.01) {
					char playAgain; 
					cout << "\nPlay another round? (Y/N): ";
					cin >> playAgain; 

					continueGame = (playAgain == 'Y') ? true : false; 
				}
				continue;

			}
		}

		int ranLuck = generateRandomCard(0, 100); 
		playerHand.playerAdvice = adviseOptimalOptionOnLuck(ranLuck, playerHand, dealerHand); 

		/*PLAYER'S TURN*/
		bool continueRound = true;
		playerHand.playerAction = "";
		while (continueRound == true) {

			playerHand.playerTurn(continueRound);   

		}

		//cout << "\nFirst Player Choice: " << playerHand.playerAction << "\nAdvice: " << playerHand.playerAdvice << "\n" << endl;

		/*DEALER'S TURN*/
		continueRound = true;
		
		cout << "\nDEALERS TURN\n" << endl;
		cout << "----- Dealer Reveals ----- \n";
		dealerHand.printHand(); 

		while (continueRound == true) { 

			if (playerHand.playerTotal > 21) {
				playerHand.totalRegularLosses += 1;
				updatePlayerBalance(playerHand, false, false, true, false);
				continueRound = false;
				break;
			}

			int card3 = 0;
			string cardSuit3;

			if (dealerHand.playerTotal < 17) {
				cout << "Dealer Draws ... " << endl; 
				card3 = generateRandomCard(1, 13);
				cardSuit3 = generateRandomSuit(card3);
				dealerHand.playerActualTotal += (card3 == 1) ? 11 : card3; 
				dealerHand.playerAces += (card3 == 1) ? 1 : 0;
				dealerHand.playerTotal += (card3 == 1) ? card3 + 10 : ((card3 > 10) ? 10 : card3);
				dealerHand.playerHand += ", " + cardSuit3;
			}
			else {
				cout << "Dealer Stands ... " << endl;

				if (isBlackjack(dealerHand.playerTotal) == true) { 
					if (isBlackjack(playerHand.playerTotal) == true) { 
						playerHand.totalBlackjackTies += 1; 
						updatePlayerBalance(playerHand, true, false, true, false); 
					}
					else {
						playerHand.totalBlackjackLosses += 1; 
						updatePlayerBalance(playerHand, false, false, true, false); 
					}
					continueRound = false; 
				} 
				else if (playerHand.playerTotal > dealerHand.playerTotal) {
					playerHand.totalRegularWins += 1;
					updatePlayerBalance(playerHand, false, true, false, false);
					continueRound = false; 
					break;
				}
				else if (dealerHand.playerTotal > playerHand.playerTotal) {
					playerHand.totalRegularLosses += 1;
					updatePlayerBalance(playerHand, false, false, false, true);
					continueRound = false;
					break;
				}
				else {
					playerHand.totalRegularTies += 1;
					cout << "\nIts a Tie...\n" << endl; 
					continueRound = false; 
					break;
				}
			}

			dealerHand.printHand();
			if (continueRound == false) {
				break;
			}

			if (isBlackjack(dealerHand.playerTotal) == true) {
				if (isBlackjack(playerHand.playerTotal) == true) {
					playerHand.totalBlackjackTies += 1;
					updatePlayerBalance(playerHand, true, false, true, false);
				}
				else {
					playerHand.totalBlackjackLosses += 1;
					updatePlayerBalance(playerHand, false, false, true, false);
				}
				continueRound = false;
			}
			else {
				while (dealerHand.playerTotal > 21) {
					if (dealerHand.playerAces > 0) {
						dealerHand.playerTotal -= 10;
						dealerHand.playerAces -= 1;
					}
					else {
						playerHand.totalRegularWins += 1;
						updatePlayerBalance(playerHand, false, false, false, false);
						continueRound = false;
						break;
					}
				}
			}
		}

		playerHand.reset();
		dealerHand.reset();

		if (playerHand.playerBalance >= 0.01) {
			char playAgain; 
			cout << "\nPlay another round? (Y/N): ";
			cin >> playAgain; 

			continueGame = (playAgain == 'Y') ? true : false; 
		}
	}
	
	playerHand.playerBalance = (playerHand.playerBalance < 0) ? 0 : playerHand.playerBalance;
	playerHand.netGainAndLoss = playerHand.playerBalance - startingBalance; 

	displayGameSummary(playerHand);

	return 0;
}