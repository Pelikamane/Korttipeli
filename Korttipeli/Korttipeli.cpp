#include <string>
#include <limits> //for numeric_limits 
#include <numeric>
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
//Straghts are still broken!!
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

static class GameManager
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

class Card
{
public:
	std::string suit;
	std::string value;

	Card(std::string cardsuit, std::string cardvalue) : suit(cardsuit), value(cardvalue) {}

	bool operator==(const Card& other) const //This overload is to enable find() on vectors of Card
	{
		return value == other.value && suit == other.suit;
	}

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
	void PrintHand(std::vector<Card>&);
	void AddJokers(int);
	void SortHand(std::vector<Card>&);
	int ValueToIntBlackjack(std::string, int);
	int ValueToIntPoker(std::vector<Card>&);
	Card& operator[](size_t index) //Operator overload to make it possible to access deck[x] (IMPORTANT)
	{
		return deck[index];
	}
};

class HandEvaluator
{
public:
	HandEvaluator(Deck& deck) : deck_(deck) {}
	void ResetEvaluator();
	bool IsFlush(std::vector<Card>&);
	bool IsStraight(std::vector<int>&);
	bool IsRoyalStraight(std::vector<int>&);
	void SameCards(std::vector<int>&);
	int EvaluateHand(std::vector<Card>&);

private:
	Deck& deck_;
	int jokers_ = 0, aces_ = 0, pairs_ = 0, biggestcard_ = NULL, smallestcard_ = NULL;
	std::vector<int> samecardvalues_;
	bool straight_ = false, royalstraight_ = false, flush_ = false, threeofakind_ = false, fourofakind_ = false,
		threeofakindjoker_ = false, fourofakindjoker_ = false, fiveofakindjoker_ = false;

};

Deck::Deck()
{
	std::vector<std::string> suits = { u8"\033[35m\u2660\033[0m", u8"\033[31m\u2665\033[0m", u8"\033[91m\u2666\033[0m", u8"\033[95m\u2663\033[0m" }; //Order of janky bitches: spade, heart, diamond, club
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

void Deck::PrintHand(std::vector<Card> &hand)
{
	for (Card card : hand)
	{
		card.PrintCard();
	}
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
		deck.emplace_back(u8"\033[1m🃏\033[0m", "JOKER");
	}
}

void Deck::SortHand(std::vector<Card> &pokerhand)
{
	static const std::unordered_map<std::string, int> valuemap = {
		{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6},
		{"7", 7}, {"8", 8}, {"9", 9}, {"10", 10},
		{"J", 11}, {"Q", 12}, {"K", 13},
		{"A", 14}, {"JOKER", 0}
	};
	for (int i = 0; i < pokerhand.size() - 1; i++)
	{
		for (int y = 0; y < pokerhand.size() - i - 1; y++)
		{
			int foundvalue = valuemap.at(pokerhand[y].value);
			int foundvalue2 = valuemap.at(pokerhand[y + 1].value);
			if (foundvalue > foundvalue2)
			{
				std::swap(pokerhand[y], pokerhand[y + 1]);
			}
		}
	}
}

int Deck::ValueToIntBlackjack(std::string value, int score)
{
	static std::unordered_map<std::string, int> valuemap = {
		{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6},
		{"7", 7}, {"8", 8}, {"9", 9}, {"10", 10},
		{"J", 10}, {"Q", 10}, {"K", 10},
		{"A", 11}
	};
	auto returnable = valuemap.find(value);
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

int Deck::ValueToIntPoker(std::vector<Card> &pokerhand)
{
	std::vector<int> handvalues;
	std::unordered_map<int, int> valuecounts;
	static std::unordered_map<std::string, int> valuemap = {
		{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6},
		{"7", 7}, {"8", 8}, {"9", 9}, {"10", 10},
		{"J", 11}, {"Q", 12}, {"K", 13},
		{"A", 14}, {"JOKER", 0}
	};
	for (int i = 0; i < pokerhand.size(); i++)
	{
		auto foundvalue = valuemap.find(pokerhand[i].value);
		handvalues.emplace_back(foundvalue->second);
	}
	for (int value : handvalues)
	{
		valuecounts[value]++;
	}
	for (auto& i : valuecounts)
	{
		//DEBUG: std::cout << i.first << " : " << i.second << std::endl;
		int removable = NULL, amount = i.second, cardvalue = i.first;

		if (amount == 5 && cardvalue == 0)
		{

		}
		else if (amount == 4 && cardvalue != 0)
		{

		}
		else if (amount == 4 && cardvalue == 0)
		{

		}
		else if (amount == 3 && cardvalue != 0)
		{

		}
		else if (amount == 3 && cardvalue == 0)
		{

		}
		else if (amount == 2 && cardvalue != 0)
		{

		}
	}
	return (std::accumulate(handvalues.begin(), handvalues.end(), 0));
}

void HandEvaluator::ResetEvaluator()
{
	jokers_ = 0;
	aces_ = 0;
	pairs_ = 0;
	biggestcard_ = NULL;
	smallestcard_ = NULL;
	samecardvalues_.clear();

	straight_ = false;
	royalstraight_ = false;
	flush_ = false;
	threeofakind_ = false;
	fourofakind_ = false;
	threeofakindjoker_ = false;
	fourofakindjoker_ = false;
	fiveofakindjoker_ = false;
}

void HandEvaluator::SameCards(std::vector<int>& handvalues)
{
	std::unordered_map<int, int> valuecounts;
	for (int value : handvalues)
	{
		valuecounts[value]++;
	}
	for (auto& i : valuecounts)
	{
		//DEBUG: std::cout << i.first << " : " << i.second << std::endl;
		int amount = i.second, cardvalue = i.first;

		if (amount == 5 && cardvalue == 0)
		{
			fiveofakindjoker_ = true;
		}
		else if (amount == 4 && cardvalue != 0)
		{
			samecardvalues_.emplace_back(cardvalue);
			fourofakind_ = true;
		}
		else if (amount == 4 && cardvalue == 0)
		{
			fourofakindjoker_ = true;
		}
		else if (amount == 3 && cardvalue != 0)
		{
			samecardvalues_.emplace_back(cardvalue);
			threeofakind_ = true;
		}
		else if (amount == 3 && cardvalue == 0)
		{
			threeofakindjoker_ = true;
		}
		else if (amount == 2 && cardvalue != 0)
		{
			samecardvalues_.emplace_back(cardvalue);
			pairs_++;
		}
	}
	/*DEBUG: if (samecardvalues_.empty())
	{
		std::cout << "Samecards is empty.\n";
	}
	else
	{
		for (int i : samecardvalues_)
		{
			std::cout << "Same card: " << i << "\n";
		}
	}*/
	
}

bool HandEvaluator::IsFlush(std::vector<Card> &pokerhand)
{
	int samesuit = 0;
	std::string firstsuit;
	for (int i = 0; i < pokerhand.size(); i++)
	{
		if (pokerhand[i].value != "JOKER")
		{
			firstsuit = pokerhand[i].suit;
			break;
		}
	}
	for (int i = 0; i < pokerhand.size(); i++)
	{
		if ((firstsuit == pokerhand[i].suit) || (pokerhand[i].value == "JOKER"))
		{
			samesuit++;
			if (samesuit == 5)
			{
				//DEBUG: std::cout << "Flush made with " << firstsuit << "\n";
				return true;
			}
		}
	}
	//DEBUG: std::cout << "\nSama maa: " << samesuit << "\nValittu maa: " << firstsuit << "\n";
	return false;
}

bool HandEvaluator::IsStraight(std::vector<int> &handvalues)
{
	int neededforstraight = 4, straightgaps = 0, largegaps = 0;
	std::vector<int> missingcards;

	for (int i = 0; i < handvalues.size() - 1; i++)
	{
		int valuedifference = handvalues[i + 1] - handvalues[i];
		if (valuedifference == 1)
		{
			neededforstraight--;
			if (neededforstraight == 0)
			{
				std::cout << "Straight formed normally\n\n";
				return true;
			}
		}
		else if (valuedifference >= 2 && handvalues[i] > 1)
		{
			int gapsize = valuedifference - 1;
			for (int gapvalue = handvalues[i] + 1; gapvalue < handvalues[i + 1]; gapvalue++)
			{
				std::cout << "Value put into missingcards: " << gapvalue << "\n";
				missingcards.push_back(gapvalue);
			}
			largegaps++;
		}
	}
	if (straightgaps > 0 && jokers_ >= missingcards.size()) // Pitää korjata
	{
		for (int i = 0; i < straightgaps; i++)
		{
			std::cout << "Using joker as " << missingcards[i] << "\n";
			handvalues.push_back(missingcards[i]);
		}
		std::cout << "Straight formed with joker\n\n";
		return true;
	}
	std::cout << "Suoraan tarvitaan: " << neededforstraight << "\nGaps: " << straightgaps << "\nLarge gaps: " << largegaps << "\n";
	return false;
}

bool HandEvaluator::IsRoyalStraight(std::vector<int> &handvalues)
{
	std::vector<int> royalvalues = { 10, 11, 12, 13, 14 };
	for (int i = 0; i < handvalues.size(); i++)
	{
		auto royalindex = find(royalvalues.begin(), royalvalues.end(), handvalues[i]);
		if (royalindex != royalvalues.end())
		{
			//DEBUG: std::cout << "This element of royals removed: " << handvalues[i] << "\n";
			royalvalues.erase(royalindex);
			//DEBUG: std::cout << "The size of royalvalues = " << royalvalues.size() << "\n";
		}
	}
	if (royalvalues.size() - jokers_ == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int HandEvaluator::EvaluateHand(std::vector<Card> &pokerhand)
{
	int handtype = NULL, jokervalue = NULL;
	std::vector<int> handvalues;
	static std::unordered_map<std::string, int> valuemap = {
		{"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6},
		{"7", 7}, {"8", 8}, {"9", 9}, {"10", 10},
		{"J", 11}, {"Q", 12}, {"K", 13},
		{"A", 14}, {"JOKER", 0}
	};
	ResetEvaluator();
	for (int i = 0; i < pokerhand.size(); i++)
	{
		auto foundvalue = valuemap.find(pokerhand[i].value);
		handvalues.emplace_back(foundvalue->second);
		if (foundvalue->second == 0)
		{
			jokers_++;
		}
		else if (foundvalue->second == 14)
		{
			aces_++;
		}
	}
	biggestcard_ = handvalues[0];
	smallestcard_ = handvalues[0];
	for (int i : handvalues)
	{
		if (i > biggestcard_)
		{
			biggestcard_ = i;
		}
		if (i < smallestcard_)
		{
			smallestcard_ = i;
		}
	}
	//DEBUG: std::cout << "Biggest card: " << biggestcard_ << "\nSmallest card : " << smallestcard_ << "\n";
	if (aces_ >= 1)
	{
		handvalues.push_back(1);
	}
	sort(handvalues.begin(), handvalues.end());
	SameCards(handvalues);
	flush_ = IsFlush(pokerhand);
	if (pairs_ == 0 && threeofakind_ == false && fourofakind_ == false && fiveofakindjoker_ == false) 
	{
		straight_ = IsStraight(handvalues);
		if (straight_ == true)
		{
			royalstraight_ = IsRoyalStraight(handvalues);
		}
	}
	if (fiveofakindjoker_)
	{
		std::cout << "JOKER FIVE!!!!\n";
		return 13;
	}
	else if ((fourofakindjoker_) || (threeofakindjoker_ && pairs_ == 1 && flush_))
	{
		std::cout << "FLUSH FIVE!!!!\n";
		return 12;
	}
	else if (royalstraight_ && flush_)
	{
		std::cout << "ROYAL FLUSH!!!\n";
		return 11;
	}
	else if ((fourofakind_ && jokers_ == 1) || (threeofakindjoker_ && pairs_ == 1))
	{
		std::cout << "FIVE OF A KIND!!!\n";
		return 10;
	}
	else if (flush_ && straight_)
	{
		std::cout << "Straight flush!!\n";
		return 8;
	}
	else if ((fourofakind_ && jokers_ == 0) || (threeofakind_ && jokers_ == 1) || (pairs_ == 1 && jokers_ == 2) || (threeofakindjoker_ && pairs_ == 0))
	{
		std::cout << "Four of a kind!!\n";
		return 7;
	}
	else if ((threeofakind_ && pairs_ == 1) || (pairs_ == 2 && jokers_ == 1))
	{
		std::cout << "Full house!\n";
		return 6;
	}
	else if (flush_ && !straight_)
	{
		std::cout << "Flush!\n";
		return 5;
	}
	else if (straight_)
	{
		std::cout << "Straight\n";
		return 4;
	}
	else if ((pairs_ == 0 && jokers_ == 2) || (pairs_ == 1 && jokers_ == 1))
	{
		std::cout << "Three of a kind\n";
		return 3;
	}
	else if (pairs_ == 2 && jokers_ == 0)
	{
		std::cout << "Two pair\n";
		return 2;
	}
	else if ((pairs_ == 1) || (pairs_ == 0 && jokers_ == 1))
	{
		std::cout << "Pair\n";
		return 1;
	}
	else
	{
		std::cout << "Highcard..\n";
		return 0;
	}
}

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
	std::vector<Card> dealercards, playercards;
	std::vector<int> dealervalueandtype, playervalueandtype;
	bool swapordrawloop = true;
	unsigned int currentcard = 0, jokeramount = 0, swapsleft = NULL, swapordraw = NULL, playerswap = NULL, dealerswap = NULL;
	Deck pokerdeck;
	HandEvaluator evaluator(pokerdeck);

	std::cout << "\nWelcome to Poker hands!\n\n";
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
	while (true)
	{
		std::cout << "\nNumber of card swaps allowed: ";
		std::cin >> swapsleft;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Wrong input\n";
			continue; //back to the top of the loop
		}
		else
		{
			if (swapsleft == 0)
			{
				std::cout << "\nNo swaps will be possible.\n";
				break;
			}
			else
			{
				std::cout << swapsleft << " swaps are possible.\n";
				break;
			}
		}
	}
	pokerdeck.ShuffleDeck();
	std::cout << "\nDealer's cards:\n";
	for (int i = 0; i < 5; i++)
	{
		std::cout << "X\n";
		dealercards.emplace_back(pokerdeck[currentcard].suit, pokerdeck[currentcard].value);
		currentcard++;
	}
	pokerdeck.SortHand(dealercards);
	std::cout << "\nPlayer's cards:\n";
	for (int i = 0; i < 5; i++)
	{
		playercards.emplace_back(pokerdeck[currentcard].suit, pokerdeck[currentcard].value);
		currentcard++;
	}
	pokerdeck.SortHand(playercards);
	pokerdeck.PrintHand(playercards);
	evaluator.EvaluateHand(playercards);
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
			evaluator.EvaluateHand(playercards);
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
			playercards[playerswap - 1].PrintCard();
			swapsleft--;
			std::cout << "Swapping card..\nSwaps left: " << swapsleft << "\n";
			playercards.erase(find(playercards.begin(), playercards.end(), playercards[playerswap - 1])); //PROBLEMATIC without overload
			playercards.emplace_back(pokerdeck[currentcard].suit, pokerdeck[currentcard].value);
			currentcard++;
			pokerdeck.SortHand(playercards);
			pokerdeck.PrintHand(playercards);
			evaluator.EvaluateHand(playercards);
			break;

		case 3:
			swapordrawloop = false;
			break;
		}
	}
	std::cout << "\n\033[4mROUND RESULTS\033[0m\n";
	std::cout << "\nRevealing dealer's hand:\n";
	pokerdeck.SortHand(dealercards);
	pokerdeck.PrintHand(dealercards);
	evaluator.EvaluateHand(dealercards);
	std::cout << "\nYour hand:\n";
	pokerdeck.SortHand(playercards);
	pokerdeck.PrintHand(playercards);
	evaluator.EvaluateHand(playercards);
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
