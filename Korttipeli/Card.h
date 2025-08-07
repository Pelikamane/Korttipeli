#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

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

	void PrintCard() const
	{
		std::cout << suit << value << std::endl;
	}
};

#endif // !CARD_H
