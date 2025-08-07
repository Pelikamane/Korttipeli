#ifndef HANDEVALUATOR_H
#define HANDEVALUATOR_H

#include <string>
#include <vector>

class Card;
class Deck;

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
	std::string TypeToString(int, bool);
	int TieBreaker(int, std::vector<Card>&, std::vector<Card>&);

private:
	Deck& deck_;
	int jokers_ = 0, aces_ = 0, pairs_ = 0, biggestcard_ = NULL, smallestcard_ = NULL,
		straightjokervalue_ = 0, sumofcards_ = 0, biggestsamecard_ = 0;
	std::vector<int> samecardvalues_; //For two pair and fullhouse tie breaker 
	bool straight_ = false, royalstraight_ = false, flush_ = false, threeofakind_ = false, fourofakind_ = false,
		threeofakindjoker_ = false, fourofakindjoker_ = false, fiveofakindjoker_ = false;

};

#endif