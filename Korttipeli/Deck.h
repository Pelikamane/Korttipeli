#ifndef DECK_H
#define DECK_H

#include <string>
#include <vector>

class Card;

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
	void PutCardFromHand(std::vector<Card>&, int);
	void EraseOldCard(int);
	int ValueToIntBlackjack(std::string, int);
	Card& operator[](size_t index) //Operator overload to make it possible to access deck[x] (IMPORTANT)
	{
		return deck[index];
	}
	size_t Size()
	{
		return deck.size();
	}
};


#endif