#include "GameManager.h"
#include "Player.h"
#include "Dealer.h"
#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include "SaveData.h"

#include <iostream>
#include <limits>
#include <iomanip>

void GameManager::Run()
{
	float programversion = 1.01;
	int choice = 0;
	Player player;
	Dealer dealer;
	SaveManager savemanager;
	SaveDataPoker pokersave;
	SaveDataBlackjack blackjacksave;
	

	while (choice != 7)
	{
		bool ispokerfilevalid = savemanager.LoadPokerData(pokersave);
		bool isblackjackfilevalid = savemanager.LoadBlackjackData(blackjacksave);
		std::cout << "\n\033[4mCARD GAMES in C++\033[0m\tversion " << programversion << "\nby Otso\n\n";
		std::cout << "1. Blackjack\n2. Poker hands\n3. Statistics\n4. Game rules\n5. Dev notes\n6. Save manager\n7. Quit\n\n";
		std::cout << "What do you want to do: ";
		std::cin >> choice;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Wrong input\n\n";
			continue; //back to the top of the loop
		}
		else
		{
			switch (choice)
			{
			case 1:
			{
				BlackJack(player, dealer, blackjacksave, savemanager);
				break;
			}

			case 2:
			{
				Poker(player, dealer, pokersave, savemanager);
				break;
			}
			case 3:
			{
				ShowStats(savemanager, pokersave, blackjacksave);
				break;
			}	
			case 4:
			{
				ShowRules();
				break;
			}
			case 5:
			{
				DevNotes();
				break;
			}
			case 6:
			{
				SaveManagement(savemanager);
				break;
			}
			case 7:
				std::cout << "Goodbye!\n";
				break;

			default:
			{
				std::cout << "Default case, wrong number input!\n\n";
				break;
			}
			}
		}
	}
}

void GameManager::ShowRules()
{
	bool showrules = true;
	int rulesinput;
	while (showrules == true)
	{
		std::cout << "\n\n\033[4mBlackjack rules:\033[0m\n\nA battle to reach the closest score to 21. Rounds end when you or the dealer go over 21\n\n\n";
		std::cout << "\033[4mPoker hands rules:\033[0m\n\nYour goal is to create the best poker hand. "
			"You can decide how many rounds to play, amount of jokers and card swaps\n\n";

		while (true)
		{
			std::cout << "Input any number to go back: ";
			std::cin >> rulesinput;
			if (!std::cin.good())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Wrong input\n\n";
				continue;
			}
			else
			{
				if (rulesinput == 69)
				{
					std::cout << "Nice.\n";
				}
				else if (rulesinput == 420)
				{
					std::cout << "Blaze that shit!\n";
				}
				else if (rulesinput == 42069 || rulesinput == 69420)
				{
					std::cout << "Ultimate combination!!\n";
				}
				showrules = false;
				break;
			}
		}
	}
}

void GameManager::ShowStats(SaveManager& savemanager, SaveDataPoker& pokersave, SaveDataBlackjack& blackjsave)
{
	float winpercentpoker;
	float winpercentblackjack;
	if (pokersave.wins_ == 0 && pokersave.totalgames_ == 0)
	{
		winpercentpoker = 0;
	}
	else
	{
		winpercentpoker = (static_cast<float>(pokersave.wins_) / pokersave.totalgames_) * 100;
	}
	if (blackjsave.wins_ == 0 && blackjsave.totalgames_ == 0)
	{
		winpercentblackjack = 0;
	}
	else
	{
		winpercentblackjack = (static_cast<float>(blackjsave.wins_) / blackjsave.totalgames_) * 100;
	}
	std::cout << "\033[4mPoker stats:\033[0m\n";
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Win percentage --> " << winpercentpoker << "%\n";
	std::cout << "Total games --> " << pokersave.totalgames_ << "\nWins --> " << pokersave.wins_ << "\n\nHigh cards --> " << pokersave.highcard_ <<
		"\nPairs --> " << pokersave.pair_ << "\nTwo pairs --> " << pokersave.twopair_ << "\nThree of a kinds --> " << pokersave.three_ << 
		"\nStraights --> " << pokersave.straight_ << "\nFlushes --> " << pokersave.flush_ << "\nFull houses --> " << pokersave.fullhouse_ << 
		"\nFour of a kinds --> " << pokersave.four_ << "\nStraight flushes --> " << pokersave.straightflush_ << 
		"\nRoyal flushes --> " << pokersave.royalflush_ << "\nFive of a kinds --> " << pokersave.five_ << 
		"\nFlush fives --> " << pokersave.flushfive_ << "\nJoker fives --> " << pokersave.jokerfive_ << "\n";

	std::cout << "\n\033[4mBlackjack stats:\033[0m\n";
	std::cout << "Win percentage --> " << winpercentblackjack << "%\n";
	std::cout << "Total games --> " << blackjsave.totalgames_ << "\nWins --> " << blackjsave.wins_ << "\nDraws --> " << blackjsave.draws_ <<
		"\nBlackjacks --> " << blackjsave.blackjacks_ << "\n";
}

void GameManager::DevNotes()
{
	bool shownotes = true;
	int choice;
	while (shownotes)
	{
		std::cout << "\n\033[4m7.8.2025\033[0m First build version of this program (v1.01) Has some issues with blackjack scoring (dealer hits at 21) "
			"and some small spacing problems. Will be fixed in future versions along with more card games.\n\n";
		while (true)
		{
			std::cout << "Input any number to go back to menu: ";
			std::cin >> choice;
			if (std::cin.good())
			{
				shownotes = false;
				break;
			}
			else
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Wrong input\n";
				continue;
			}
		}
	}
}

void GameManager::SaveManagement(SaveManager& savemanager)
{
	int choice;
	std::cout << "\n\033[4mYou can delete savefiles here\033[0m\n\n";
	std::cout << "(1) Delete Blackjack save\n(2) Delete Poker save\n(3) Delete both\n(4) Main menu\n";
	while (true)
	{
		std::cout << "What would you like to do: ";
		std::cin >> choice;
		if (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Wrong input\n";
			continue;
		}
		else
		{
			if (choice < 1 || choice > 4)
			{
				std::cout << "Invalid number\n";
				continue;
			}
			else
			{
				switch (choice)
				{
				case 1:
				{
					while (true)
					{
						char deletedata;
						std::cout << "Are you sure that you want to delete the blackjack data? (y/n): ";
						std::cin >> deletedata;
						if (!std::cin.good())
						{
							std::cout << "Wrong input\n";
							continue;
						}
						else if ((deletedata != 'y' && deletedata != 'Y') && (deletedata != 'n' && deletedata != 'N'))
						{
							std::cout << "Wrong letter\n";
							continue;
						}
						else if (deletedata == 'y' || deletedata == 'Y')
						{
							savemanager.ClearBlackjackSaveData();
							break;
						}
						else
						{
							std::cout << "Blackjack data is \033[3mback in black\033[0m\n";
							break;
						}
					}
					break;
				}
				case 2:
				{
					while (true)
					{
						char deletedata;
						std::cout << "Are you sure that you want to delete the poker data? (y/n): ";
						std::cin >> deletedata;
						if (!std::cin.good())
						{
							std::cout << "Wrong input\n";
							continue;
						}
						else if ((deletedata != 'y' && deletedata != 'Y') && (deletedata != 'n' && deletedata != 'N'))
						{
							std::cout << "Wrong letter\n";
							continue;
						}
						else if (deletedata == 'y' || deletedata == 'Y')
						{
							savemanager.ClearPokerSaveData();
							break;
						}
						else
						{
							std::cout << "Poker data has been \033[3msaved\033[0m :D\n";
							break;
						}
					}
					break;
				}	
				case 3:
				{
					while (true)
					{
						char deletedata;
						std::cout << "Are you sure that you want to delete both saves? (y/n): ";
						std::cin >> deletedata;
						if (!std::cin.good())
						{
							std::cout << "Wrong input\n";
							continue;
						}
						else if ((deletedata != 'y' && deletedata != 'Y') && (deletedata != 'n' && deletedata != 'N'))
						{
							std::cout << "Wrong letter\n";
							continue;
						}
						else if (deletedata == 'y' || deletedata == 'Y')
						{
							savemanager.ClearPokerSaveData();
							savemanager.ClearBlackjackSaveData();
							break;
						}
						else
						{
							std::cout << "All save files have been saved\n";
							break;
						}
					}
					break;
				}
				case 4:
					std::cout << "Going back to menu..\n";
					break;
				}	
			}
		}
		std::cout << "Returning back to menu..\n";
		break;
	}
}

void GameManager::BlackJack(Player& player, Dealer& dealer, SaveDataBlackjack& savedata, SaveManager& savemanager)
{
	bool gameloop = true;
	Deck deck;

	std::cout << "\nNEEDS SOME FIXING\nWelcome to Blackjack, time to get robbed.\n";
	while (gameloop)
	{
		bool gameoverplayer = false, gameoverdealer = false;
		int dealerscore = 0, playerscore = 0, currentcard = 0;
		int hitorstand, playornot;
		std::vector<int> playercards;
		std::vector<int> dealercards;

		deck.ShuffleDeck();
		std::cout << "\nDealer's hand:\n";
		for (int i = 0; i < 2; i++)
		{
			deck[currentcard].PrintCard();
			dealerscore += deck.ValueToIntBlackjack(deck[currentcard].value, dealerscore);
			dealercards.emplace_back(currentcard);
			currentcard++;
		}
		if (dealerscore == 21) //if the dealer gets a blackjack straight away
		{
			std::cout << "Blackjack!\n";
		}
		std::cout << "Dealer's hand value: " << dealerscore << "\n\n";

		std::cout << "Your hand:\n";
		for (int i = 0; i < 2; i++)
		{
			deck[currentcard].PrintCard();
			playerscore += deck.ValueToIntBlackjack(deck[currentcard].value, playerscore);
			playercards.emplace_back(currentcard);
			currentcard++;
		}
		std::cout << "Your hand value: " << playerscore << "\n";
		if (playerscore == 21)
		{
			gameoverdealer = true; //Skip hit or stand
			player.totalblackjacks++;
			savedata.blackjacks_++;
			std::cout << "Blackjack!\n\n";

			while (dealerscore < 21)
			{
				std::cout << "Dealer draws: ";
				deck[currentcard].PrintCard();
				dealerscore += deck.ValueToIntBlackjack(deck[currentcard].value, dealerscore);
				dealercards.emplace_back(currentcard);
				currentcard++;
				std::cout << "Dealer's current hand value: " << dealerscore << "\n\n";
			}
			if (dealerscore == 21)
			{
				std::cout << "Blackjack for both!\nIt's a draw!!";
				savedata.draws_++;
				player.playerblackjackwins++;
				dealer.dealerblackjackwins++;
			}
			if (dealerscore > 21)
			{
				std::cout << "Dealer busted, You win!\n";
				std::cout << "Your final hand:\n";
				for (int cardindex : playercards)
				{
					deck[cardindex].PrintCard();
				}
				savedata.wins_++;
				player.playerblackjackwins++;
				gameoverdealer = true;
			}
		}

		while (gameoverplayer == false && gameoverdealer == false)
		{
			while (true)
			{
				std::cout << "\nHit (1) or stand (2): ";
				std::cin >> hitorstand;
				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Not a number\n";
				}
				else
				{
					if (hitorstand != 1 && hitorstand != 2)
					{
						std::cout << "Wrong number\n";
						continue;
					}
					else
					{
						break;
					}
				}
			}
			switch (hitorstand)
			{
			case 1:
				std::cout << "\nYou draw a card: ";
				deck[currentcard].PrintCard();
				playerscore += deck.ValueToIntBlackjack(deck[currentcard].value, playerscore);
				playercards.emplace_back(currentcard);
				currentcard++;
				if (playerscore > 21)
				{
					std::cout << "Your current hand value: " << playerscore << "\n\n";
					std::cout << "You busted, dealer wins!\n";
					std::cout << "Dealer's final hand:\n";
					for (int cardindex : dealercards)
					{
						deck[cardindex].PrintCard();
					}
					dealer.dealerblackjackwins++;
					gameoverplayer = true;
				}
				if (playerscore <= 21)
				{
					if (playerscore == 21) //Displays the blackjack! message, could be more elegantly done
					{
						savedata.blackjacks_++;
						player.totalblackjacks++;
						std::cout << "Blackjack!\n";
					}
					std::cout << "Your current hand value: " << playerscore << "\n";
					if (dealerscore != 21 && dealerscore < playerscore)
					{
						std::cout << "\nDealer draws a card: ";
						deck[currentcard].PrintCard();
						dealerscore += deck.ValueToIntBlackjack(deck[currentcard].value, dealerscore);
						dealercards.emplace_back(currentcard);
						currentcard++;
						if (dealerscore == 21)
						{
							std::cout << "Blackjack!\n";
						}
						std::cout << "Dealer's current hand value: " << dealerscore << "\n";
						if (dealerscore > 21)
						{
							std::cout << "\nDealer busted, You win!\n";
							std::cout << "Your final hand:\n";
							for (int cardindex : playercards)
							{
								deck[cardindex].PrintCard();
							}
							savedata.wins_++;
							player.playerblackjackwins++;
							gameoverdealer = true;
						}
					}
				}
				break;

			case 2:
				if (playerscore < dealerscore)
				{
					std::cout << "\nYou busted, dealer wins!\n";
					std::cout << "Dealer's final hand:\n";
					for (int cardindex : dealercards)
					{
						deck[cardindex].PrintCard();
					}
					dealer.dealerblackjackwins++;
					gameoverplayer = true;

				}
				if (playerscore == dealerscore)
				{
					std::cout << "\nDealer draws a card: ";
					deck[currentcard].PrintCard();
					dealerscore += deck.ValueToIntBlackjack(deck[currentcard].value, dealerscore);
					dealercards.emplace_back(currentcard);
					currentcard++;
					std::cout << "Dealer's current hand value: " << dealerscore << "\n";
					if (dealerscore == 21)
					{
						std::cout << "Blackjack!\n";
					}
					if (dealerscore > 21)
					{
						std::cout << "\nDealer busted, You win!\n";
						std::cout << "Your final hand:\n";
						for (int cardindex : playercards)
						{
							deck[cardindex].PrintCard();
						}
						savedata.wins_++;
						player.playerblackjackwins++;
						gameoverdealer = true;
						break;
					}
				}
				while (dealerscore < playerscore)
				{
					std::cout << "\nDealer draws a card: ";
					deck[currentcard].PrintCard();
					dealerscore += deck.ValueToIntBlackjack(deck[currentcard].value, dealerscore);
					dealercards.emplace_back(currentcard);
					currentcard++;
					std::cout << "Dealer's current hand value: " << dealerscore << "\n";
					if (dealerscore > 21)
					{
						std::cout << "\nDealer busted, You win!\n";
						std::cout << "Your final hand:\n";
						for (int cardindex : playercards)
						{
							deck[cardindex].PrintCard();
						}
						savedata.wins_++;
						player.playerblackjackwins++;
						gameoverdealer = true;
						break;
					}
					if (dealerscore == 21)
					{
						std::cout << "Blackjack!\n";
					}
					if (dealerscore == playerscore)
					{
						std::cout << "It's a draw!\n\n";
						savedata.draws_++;
						gameoverdealer = true;
						gameoverplayer = true;
						break;
					}
				}
				break;

			default:
				std::cout << "Default case, something has gone terribly wrong!\n";
			}
		}
		std::cout << "\nCurrent game score: Dealer " << dealer.dealerblackjackwins << " Player " << player.playerblackjackwins << "\n\n";
		savedata.totalgames_++;
		while (true)
		{
			std::cout << "Play again (1) or quit (2): ";
			std::cin >> playornot;
			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Not a number\n";
			}
			else
			{
				if (playornot != 1 && playornot != 2)
				{
					std::cout << "Wrong number\n";
					continue;
				}
				else
				{
					break;
				}
			}
		}
		switch (playornot)
		{
		case 1:
			std::cout << "Continuing Blackjack..\n";
			break;

		case 2:
			std::cout << "Going back to the menu..\n";
			savemanager.SaveBlackjackData(savedata);
			gameloop = false;
			break;

		default:
			std::cout << "\nDefault case, something has gone terribly wrong!\n";
			break;
		}
	}
}

void GameManager::Poker(Player& player, Dealer& dealer, SaveDataPoker& pokersave, SaveManager& savemanager)
{
	std::vector<Card> dealercards, playercards;
	bool selectionloop = true, gameover = false;
	unsigned int maxrounds = 0, roundcounter = 1, jokeramount = 0, maxswaps = 0,
		dealerhandvalue = 0, playerhandvalue = 0, gameoverchoice = 0;
	Deck pokerdeck;
	HandEvaluator evaluator(pokerdeck);

	std::cout << "\n\n\033[4mWelcome to Poker hands!\033[0m\n";
	while (gameover == false)
	{
		bool swapordrawloop = true, scoringstring = false;
		unsigned int currentcard = 0, swapordraw = 0, playerswap = 0, dealerswap = 0, swapsleft = maxswaps;

		while (selectionloop)
		{
			while (true)
			{
				std::cout << "\nHow many rounds do you want to play: ";
				std::cin >> maxrounds;
				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Wrong input\n";
					continue; //back to the top of the loop
				}
				else
				{
					if (maxrounds > 0)
					{
						if (maxrounds == 1)
						{
							std::cout << "Game will be " << maxrounds << " round long.\n\n";
							break;
						}
						else
						{
							std::cout << "Game will be " << maxrounds << " rounds long.\n\n";
							break;
						}

					}
					else
					{
						std::cout << "Back to the menu then.\n\n"; //Fix
						selectionloop = false;
						gameover = true;
						break;
					}
				}
			}
			while (true)
			{
				std::cout << "Number of jokers added: ";
				std::cin >> jokeramount;
				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Wrong input\n\n";
					continue; //back to the top of the loop
				}
				else
				{
					if (jokeramount > 0)
					{
						if (jokeramount == 1)
						{
							pokerdeck.AddJokers(jokeramount);
							std::cout << jokeramount << " joker added to the deck.\n\n";
							break;
						}
						else
						{
							pokerdeck.AddJokers(jokeramount);
							std::cout << jokeramount << " jokers added to the deck.\n\n";
							break;
						}
					}
					else
					{
						std::cout << "No jokers will be added to the deck.\n\n";
						break;
					}
				}
			}
			while (true)
			{
				std::cout << "Number of card swaps allowed: ";
				std::cin >> maxswaps;
				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Wrong input\n";
					continue; //back to the top of the loop
				}
				else
				{
					if (maxswaps > 0)
					{
						if (maxswaps == 1)
						{
							swapsleft = maxswaps;
							std::cout << maxswaps << " swap is possible.\n\n";
							break;
						}
						else
						{
							swapsleft = maxswaps;
							std::cout << maxswaps << " swaps are possible.\n\n";
							break;
						}
					}
					else
					{
						std::cout << "No swaps will be possible.\n\n";
						break;
					}
				}
			}
			selectionloop = false;
		}
		std::cout << "Round " << roundcounter << "/" << maxrounds << " starting now!\n";
		pokerdeck.ShuffleDeck();
		if (maxswaps != 0)
		{
			std::cout << "\nDealer's cards:\n";
			for (int i = 0; i < 5; i++)
			{
				std::cout << "X\n";
				dealercards.emplace_back(std::move(pokerdeck[currentcard]));
				pokerdeck.EraseOldCard(currentcard);
				currentcard++;
			}
			pokerdeck.SortHand(dealercards);
			std::cout << "\nPlayer's cards:\n";
			for (int i = 0; i < 5; i++)
			{
				playercards.emplace_back(std::move(pokerdeck[currentcard]));
				pokerdeck.EraseOldCard(currentcard);
				currentcard++;
			}
			pokerdeck.SortHand(playercards);
			pokerdeck.PrintHand(playercards);
			playerhandvalue = evaluator.EvaluateHand(playercards);
			std::cout << evaluator.TypeToString(playerhandvalue, scoringstring);
		}
		else
		{
			for (int i = 0; i < 5; i++)
			{
				dealercards.emplace_back(std::move(pokerdeck[currentcard]));
				pokerdeck.EraseOldCard(currentcard);
				currentcard++;
			}
			pokerdeck.SortHand(dealercards);
			for (int i = 0; i < 5; i++)
			{
				playercards.emplace_back(std::move(pokerdeck[currentcard]));
				pokerdeck.EraseOldCard(currentcard);
				currentcard++;
			}
			pokerdeck.SortHand(playercards);
		}
		while (swapordrawloop && swapsleft != 0)
		{
			while (true)
			{
				std::cout << "\nSwap cards with dealer (1)\nSwap cards from the deck (2)\nDo nothing (3)\n\nWhat would you like to do: ";
				std::cin >> swapordraw;
				if (!std::cin.good())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Not a number.\n\n";
					continue;
				}
				else
				{
					if (swapordraw != 1 && swapordraw != 2 && swapordraw != 3)
					{
						std::cout << "Wrong number.\n\n";
						continue;
					}
					else
					{
						break;
					}
				}
			}
			switch (swapordraw)
			{
			case 1:
				while (true)
				{
					std::cout << "\nSelect which card to swap with dealer (1-5): ";
					std::cin >> playerswap;
					if (!std::cin.good())
					{
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Not a number.\n\n";
						continue;
					}
					else
					{
						if (playerswap != 1 && playerswap != 2 && playerswap != 3 && playerswap != 4 && playerswap != 5)
						{
							std::cout << "Wrong selection.\n\n";
							continue;
						}
						else
						{
							break;
						}
					}
				}
				std::cout << "You picked: ";
				playercards[playerswap - 1].PrintCard(); //The error handler takes care of the index range
				while (true)
				{
					std::cout << "\nSelect a dealer's card to swap it with: ";
					std::cin >> dealerswap;
					if (!std::cin.good())
					{
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Not a number.\n\n";
						continue;
					}
					else
					{
						if (dealerswap != 1 && dealerswap != 2 && dealerswap != 3 && dealerswap != 4 && dealerswap != 5)
						{
							std::cout << "Wrong selection.\n\n";
							continue;
						}
						else
						{
							break;
						}
					}
				}
				std::swap(playercards[playerswap - 1], dealercards[dealerswap - 1]); //So convenient!
				swapsleft--;
				std::cout << "Swapping cards..\nSwaps left: " << swapsleft << "\n\n";
				std::cout << "Your cards:\n";
				pokerdeck.SortHand(playercards);
				pokerdeck.PrintHand(playercards);
				playerhandvalue = evaluator.EvaluateHand(playercards);
				std::cout << evaluator.TypeToString(playerhandvalue, scoringstring);
				break;

			case 2:
				while (true)
				{
					std::cout << "\nSelect which card to swap from the deck (1-5): ";
					std::cin >> playerswap;
					if (!std::cin.good())
					{
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Not a number.\n\n";
						continue;
					}
					else
					{
						if (playerswap < 0 || playerswap > 5)
						{
							std::cout << "Wrong selection.\n\n";
							continue;
						}
						else
						{
							playerswap--;
							break;
						}
					}
				}
				std::cout << "You picked: ";
				playercards[playerswap].PrintCard();
				swapsleft--;
				std::cout << "Swapping card..\nSwaps left: " << swapsleft << "\n";
				pokerdeck.PutCardFromHand(playercards, playerswap);
				playercards.erase(playercards.begin() + playerswap);
				playercards.emplace_back(std::move(pokerdeck[currentcard]));
				pokerdeck.EraseOldCard(currentcard);
				currentcard++;
				pokerdeck.SortHand(playercards);
				pokerdeck.PrintHand(playercards);
				playerhandvalue = evaluator.EvaluateHand(playercards);
				std::cout << evaluator.TypeToString(playerhandvalue, scoringstring);
				break;

			case 3:
				swapordrawloop = false;
				break;
			}
		}
		std::cout << "\n" << roundcounter << ". \033[4mROUND RESULTS\033[0m\n";
		std::cout << "\nRevealing dealer's hand:\n";
		pokerdeck.SortHand(dealercards);
		pokerdeck.PrintHand(dealercards);
		dealerhandvalue = evaluator.EvaluateHand(dealercards);
		std::cout << evaluator.TypeToString(dealerhandvalue, scoringstring);
		std::cout << "\nYour hand:\n";
		pokerdeck.SortHand(playercards);
		pokerdeck.PrintHand(playercards);
		playerhandvalue = evaluator.EvaluateHand(playercards);
		std::cout << evaluator.TypeToString(playerhandvalue, scoringstring);
		scoringstring = true;
		savemanager.PokerDataToStruct(pokersave, playerhandvalue);
		if (playerhandvalue == dealerhandvalue)
		{
			int playerwin = evaluator.TieBreaker(playerhandvalue, playercards, dealercards);
			if (playerwin == 0)
			{
				dealer.dealerpokerwins_++;
			}
			else if (playerwin == 1)
			{
				player.playerpokerwins_++;
			}
		}
		else if (playerhandvalue > dealerhandvalue)
		{
			std::cout << "\n\033[4m\033[32mYou win\033[0m with " << evaluator.TypeToString(playerhandvalue, scoringstring) <<
				" against dealer's " << evaluator.TypeToString(dealerhandvalue, scoringstring) << "\n";
			player.playerpokerwins_++;
		}
		else
		{
			std::cout << "\n\033[4m\033[31mYou lost\033[0m with " << evaluator.TypeToString(playerhandvalue, scoringstring) <<
				" against dealer's " << evaluator.TypeToString(dealerhandvalue, scoringstring) << "\n";
			dealer.dealerpokerwins_++;
		}
		std::cout << "\nGame score:\n\033[32mYou:\033[0m " << player.playerpokerwins_ << "\n\033[31mDealer:\033[0m " <<
			dealer.dealerpokerwins_ << "\n\n";
		int handsize = playercards.size();
		for (int i = 0; i < handsize; i++)
		{
			pokerdeck.PutCardFromHand(playercards, i);
			pokerdeck.PutCardFromHand(dealercards, i);
		}
		playercards.clear();
		dealercards.clear();
		swapordrawloop = true;
		scoringstring = false;
		roundcounter++;
		if (roundcounter > maxrounds)
		{
			{
				std::cout << "\033[4mGame over\033[0m\n";
				if (player.playerpokerwins_ > dealer.dealerpokerwins_)
				{
					pokersave.wins_++;
					std::cout << "\033[4m\033[32mYOU WON!\033[0m\n";
				}
				else if (player.playerblackjackwins < dealer.dealerpokerwins_)
				{
					std::cout << "\033[4m\033[31mYOU LOST!\033[0m\n";
				}
				else
				{
					std::cout << "\033[33mDRAW!\033[0m\n";
				}
				pokersave.totalgames_++;
				savemanager.SavePokerData(pokersave);
				while (true)
				{
					std::cout << "\n1.Play again\n2.Main menu\nWhat would you like to do: ";
					std::cin >> gameoverchoice;
					if (!std::cin.good())
					{
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						continue;
					}
					else
					{
						if (gameoverchoice != 1 && gameoverchoice != 2)
						{
							std::cout << "Wrong input.\n";
							continue;
						}
						else
						{
							break;
						}
					}
				}
				if (gameoverchoice == 1)
				{
					std::cout << "Starting new game..\n";
					player.playerpokerwins_ = 0;
					dealer.dealerpokerwins_ = 0;
					maxrounds = 0;
					roundcounter = 1;
					jokeramount = 0;
					maxswaps = 0;
					dealerhandvalue = 0;
					playerhandvalue = 0,
						selectionloop = true;
				}
				else
				{
					player.playerpokerwins_ = 0;
					dealer.dealerpokerwins_ = 0;
					std::cout << "Returning back to menu..\n";
					gameover = true;
				}
			}
		}
		else if (roundcounter <= maxrounds)
		{
			while (true)
			{
				int continuechoice;
				std::cout << "Continue to the next round by inputting any number: ";
				std::cin >> continuechoice;
				if (!std::cin.good())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				else
				{
					std::cout << "Moving to next round..\n\n";
					break;
				}
			}
		}
	}
}