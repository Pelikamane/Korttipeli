#include "Deck.h"
#include "Card.h"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include <unordered_map>


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

void Deck::PrintHand(std::vector<Card>& hand)
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

void Deck::SortHand(std::vector<Card>& pokerhand)
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

void Deck::PutCardFromHand(std::vector<Card>& hand, int handindex)
{
	deck.emplace_back(std::move(hand[handindex]));
}

void Deck::EraseOldCard(int index)
{
	deck.erase(deck.begin() + index);
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
