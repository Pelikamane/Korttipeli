#include <string>
#include <limits> //for numeric_limits 
#include <vector>
#include <iostream>
#include <ios> //for <streamsize>
#define NOMINMAX //needed for windows.h not to interrupt <limits>
#include <Windows.h> //Probably not needed
#include <chrono> //Seeds rng
#include <random> //enables std::default_random_engine
#include <algorithm> //enables std::shuffle
#include <unordered_map> //Comparison for card point scoring


//To please the compiler, Player and Dealer must be declared above GameManager
class Player
{
public:
	int playerblackjackwins = 0;
	int totalblackjacks = 0;
private:

};

class Dealer
{
public:
	int dealerblackjackwins = 0;

private:

};

class GameManager
{
public:
	GameManager()
	{

	}
	void Run();
	void ShowRules();
	void BlackJack(Player&, Dealer&);
	void Poker(Player&, Dealer&);
private:

};

void GameManager::Run()
{
	int choice = 0;
	Player player;
	Dealer dealer;

	while (choice != 4)
	{
		std::cout << "\nCARD GAMES in C++" << std::endl << "by Otso\n\n";
		std::cout << "1. Blackjack\n2. Poker hands\n3. Game rules\n4. Quit\n\n";
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
				BlackJack(player, dealer);
				break;
			}

			case 2:
			{
				Poker(player, dealer);
				break;
			}

			case 3:
			{
				ShowRules();
				break;
			}

			case 4:
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

class Card
{
public:
	std::string suit;
	std::string value;

	Card(std::string cardsuit, std::string cardvalue) : suit(cardsuit), value(cardvalue) {} //Fancy and efficient constructor (Why are the curly brackets needed)

	void PrintCard()
	{
		std::cout << suit << value << std::endl;
	}
};

class Deck
{
private:
	std::vector<Card> deck; //Vector of the class Card: Each member is an object with a suit and a value

public:
	Deck();
	void ShuffleDeck();
	void PrintDeck();
	void AddJokers(int);
	int ValueToInt(std::string value, int);
	Card& operator[](size_t index) //Operator overload to make it possible to access deck[x] (IMPORTANT)
	{
		return deck[index];
	}
};

Deck::Deck()
{
	std::vector<std::string> suits = { u8"\u2660", u8"\u2665", u8"\u2666", u8"\u2663" }; //Order of janky bitches: spade, heart, diamond, club
	std::vector<std::string> values = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

	for (std::string& suit : suits)
	{
		for (std::string& value : values)
		{
			deck.emplace_back(suit, value);  //Creates an unshuffled deck of cards
		}
	}
}

void Deck::ShuffleDeck() //This is actual voodoo
{
	//Seed from the current system clock time (unsigned = only positive int = bigger range because the negatives are added to positives so int*2)
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

	//Random number generator made with a funny name (mt = Mersenne Twister) (19937 = exponent after the numbers start to repeat)
	std::mt19937 rng(seed);

	std::shuffle(deck.begin(), deck.end(), rng);
	std::cout << "Current seed: " << seed << std::endl;
}

void Deck::PrintDeck()
{
	for (Card card : deck)
	{
		card.PrintCard();
	}
}

void Deck::AddJokers(int jokeramount)
{
	for (int i = 0; i < jokeramount; i++)
	{
		deck.emplace_back("JOKER", "0");
	}
}

int Deck::ValueToInt(std::string value, int score)
{
	static std::unordered_map<std::string, int> valuemap = {
		{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6},
		{"7", 7}, {"8", 8}, {"9", 9}, {"10", 10},
		{"J", 10}, {"Q", 10}, {"K", 10},
		{"A", 11}
	};
	auto returnable = valuemap.find(value); //Requires auto probably for type conversion
	if (value == "A" && (score + 11) > 21)
	{
		return 1;
	}
	else
	{
		return (returnable != valuemap.end() ? returnable->second : 100); //(condition) ? value_true : value_false
	}
	//if the returnable is found before the search has gone beyond index -> assign the value to the int next to the string -> else assign default value 100
}

void GameManager::ShowRules()
{
	bool showrules = true;
	int rulesinput;
	while (showrules == true)
	{
		std::cout << "\n\n\033[4mBlackjack rules:\033[0m\n\nA battle to reach the closest score to 21. Rounds end when you or the dealer go over 21\n\n\n";
		std::cout << "\033[4mPoker hands rules:\033[0m\n\nYour goal is to create the best poker hand. You can exchange 3 cards with the dealer.\n"
			"If the dealer doesn't want to exhange cards, you will draw them from the deck.\n\n\n";

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

void GameManager::BlackJack(Player& player, Dealer& dealer)
{
	bool gameloop = true;
	Deck deck;

	std::cout << "\nWelcome to Blackjack, time to get robbed.\n";
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
			dealerscore += deck.ValueToInt(deck[currentcard].value, dealerscore);
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
			playerscore += deck.ValueToInt(deck[currentcard].value, playerscore);
			playercards.emplace_back(currentcard);
			currentcard++;
		}
		std::cout << "Your hand value: " << playerscore << "\n";
		if (playerscore == 21)
		{
			gameoverdealer = true; //Skip hit or stand
			player.totalblackjacks++;
			std::cout << "Blackjack!\n\n";

			while (dealerscore < 21)
			{
				std::cout << "Dealer draws: ";
				deck[currentcard].PrintCard();
				dealerscore += deck.ValueToInt(deck[currentcard].value, dealerscore);
				dealercards.emplace_back(currentcard);
				currentcard++;
				std::cout << "Dealer's current hand value: " << dealerscore << "\n\n";
			}
			if (dealerscore == 21)
			{
				std::cout << "Blackjack for both!\nIt's a draw!!";
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
				playerscore += deck.ValueToInt(deck[currentcard].value, playerscore);
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
						player.totalblackjacks++;
						std::cout << "Blackjack!\n";
					}
					std::cout << "Your current hand value: " << playerscore << "\n";
					if (dealerscore != 21 && dealerscore < playerscore)
					{
						std::cout << "\nDealer draws a card: ";
						deck[currentcard].PrintCard();
						dealerscore += deck.ValueToInt(deck[currentcard].value, dealerscore);
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
					dealerscore += deck.ValueToInt(deck[currentcard].value, dealerscore);
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
						player.playerblackjackwins++;
						gameoverdealer = true;
						break;
					}
				}
				while (dealerscore < playerscore)
				{
					std::cout << "\nDealer draws a card: ";
					deck[currentcard].PrintCard();
					dealerscore += deck.ValueToInt(deck[currentcard].value, dealerscore);
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
						player.playerblackjackwins++;
						gameoverdealer = true;
						break;
					}
					if (dealerscore == playerscore)
					{
						std::cout << "It's a draw!\n\n";
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
			gameloop = false;
			break;

		default:
			std::cout << "\nDefault case, something has gone terribly wrong!\n";
			break;
		}
	}
}

void GameManager::Poker(Player& player, Dealer& dealer)
{
	int currentcard = 0, jokeramount = 0;
	Deck pokerdeck;

	std::cout << "\nWelcome to Poker hands!\n\n";
	while (true)
	{
		std::cout << "The number of jokers added to the deck: ";
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
			if (jokeramount == 0)
			{
				std::cout << "\nNo jokers will be added to the deck.\n";
				break;
			}
			else
			{
				pokerdeck.AddJokers(jokeramount);
				std::cout << jokeramount << " jokers added to the deck.\n";
				break;
			}
		}
	}

	pokerdeck.PrintDeck();
}

int main()
{
	bool programloop = true;
	SetConsoleOutputCP(CP_UTF8); //Probably needed

	while (programloop)
	{
		GameManager gm;
		gm.Run();
		return 0;
	}
}
