#include "HandEvaluator.h"
#include "Deck.h"
#include "Card.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

void HandEvaluator::ResetEvaluator()
{
	jokers_ = 0;
	aces_ = 0;
	pairs_ = 0;
	biggestcard_ = NULL;
	smallestcard_ = NULL;
	straightjokervalue_ = 0;
	sumofcards_ = 0;
	biggestsamecard_ = 0;
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
		int amount = i.second, cardvalue = i.first;
		//DEBUG: std::cout << cardvalue << " : " << amount << std::endl;
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
	for (int i : samecardvalues_)
	{
		if (i > biggestsamecard_)
		{
			biggestsamecard_ = i;
			//std::cout << "Biggest samecard is now: " << biggestsamecard_ << "\n";
		}
	}
	//std::cout << "Samecardvalues size: " << samecardvalues_.size() << "\n";
	if (jokers_ > 0 && samecardvalues_.size() == 0)
	{
		biggestsamecard_ = biggestcard_;
	}
}

bool HandEvaluator::IsFlush(std::vector<Card>& pokerhand)
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

bool HandEvaluator::IsStraight(std::vector<int>& handvalues)
{
	int neededforstraight = 4, straightgaps = 0, secondbiggest;
	bool islowstraightace = false;
	std::vector<int> missingcards, handwithoutjokers;

	for (int i : handvalues)
	{
		if (i > 1)
		{
			handwithoutjokers.emplace_back(i);
			//std::cout << "value put into handwithoutjokers: " << i << "\n";
		}
	}
	if (aces_ > 0)
	{
		int valuedifflowace = smallestcard_ - 1;
		int valuediffhighace = biggestcard_ - handwithoutjokers[handwithoutjokers.size() - 2];
		/*DEBUG: std::cout << "value diff low ace: " << valuedifflowace << "\n";
		std::cout << "value diff high ace: " << valuediffhighace << "\n";*/
		if (valuediffhighace > valuedifflowace)
		{
			auto aceindex = std::find(handwithoutjokers.begin(), handwithoutjokers.end(), 14);
			if (aceindex != handwithoutjokers.end())
			{
				sumofcards_ -= 13;
				handwithoutjokers.erase(aceindex);
				handwithoutjokers.emplace_back(1);
				sort(handwithoutjokers.begin(), handwithoutjokers.end());
				islowstraightace = true;
			}
		}
	}
	secondbiggest = handwithoutjokers[handwithoutjokers.size() - 2];
	/*DEBUG: for (int i : handwithoutjokers)
	{
		std::cout << "handwithoutjokers: " << i << "\n";
	}*/
	for (int i = 0; i < handwithoutjokers.size() - 1; i++)
	{
		int valuedifference = handwithoutjokers[i + 1] - handwithoutjokers[i];
		if (valuedifference == 1)
		{
			neededforstraight--;
			if (neededforstraight == 0)
			{
				//std::cout << "Straight formed normally\n\n";
				return true;
			}
		}
		else if (valuedifference >= 2)
		{
			int gapsize = valuedifference - 1;
			for (int gapvalue = handwithoutjokers[i] + 1; gapvalue < handwithoutjokers[i + 1]; gapvalue++)
			{
				//std::cout << "Value put into missingcards: " << gapvalue << "\n";
				missingcards.push_back(gapvalue);
			}
		}
	}
	if (jokers_ > missingcards.size())
	{
		int index = 1, missingcardindex = 0, leftinmissingcards = missingcards.size();
		for (int i = jokers_; i > 0; i--)
		{
			if (leftinmissingcards != 0)
			{
				straightjokervalue_ += missingcards[missingcardindex];
				missingcardindex++;
				leftinmissingcards--;
				//std::cout << "Straight joker value if leftinmissing!=0: " << straightjokervalue_ << "\n";
			}
			else if (leftinmissingcards == 0)
			{
				if (biggestcard_ + index < 15)
				{
					straightjokervalue_ += (biggestcard_ + index);
					//std::cout << "Straight joker value if != 14: " << straightjokervalue_ << "\n";
					index++;
				}
				else if (islowstraightace)
				{
					straightjokervalue_ += (handwithoutjokers[handwithoutjokers.size() - 1] + index);
					//std::cout << "straight joker value lowace: " << straightjokervalue_ << "\n";
					index++;
				}
				else
				{
					straightjokervalue_ += (smallestcard_ - 1);
					//std::cout << "Straight joker value else != 14: " << straightjokervalue_ << "\n";
				}
			}
		}
		//std::cout << "Straight formed with joker\n\n";
		return true;
	}
	else if (jokers_ == missingcards.size())
	{
		for (int i = 0; i < missingcards.size(); i++)
		{
			straightjokervalue_ += missingcards[i];
			//std::cout << "Straight joker value if i==size: " << straightjokervalue_ << "\n";
		}
		//std::cout << "Straight formed with joker\n\n";
		return true;
	}
	return false;
}

bool HandEvaluator::IsRoyalStraight(std::vector<int>& handvalues)
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

int HandEvaluator::EvaluateHand(std::vector<Card>& pokerhand)
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
	smallestcard_ = 15; //15 to make it not count jokers
	for (int i : handvalues)
	{
		sumofcards_ += i;
		if (i > biggestcard_)
		{
			biggestcard_ = i;
		}
		if (i < smallestcard_ && i != 0)
		{
			smallestcard_ = i;
		}
	}
	//DEBUG: std::cout << "Biggest card: " << biggestcard_ << "\nSmallest card : " << smallestcard_ << "\n";
	sort(handvalues.begin(), handvalues.end());
	SameCards(handvalues);
	flush_ = IsFlush(pokerhand);
	if (pairs_ == 0 && threeofakind_ == false && fourofakind_ == false && fourofakindjoker_ == false && fiveofakindjoker_ == false)
	{
		straight_ = IsStraight(handvalues);
		if (straight_ == true)
		{
			royalstraight_ = IsRoyalStraight(handvalues);
		}
	}
	if (fiveofakindjoker_)
	{
		return 13;
	}
	else if ((fourofakindjoker_) || (threeofakindjoker_ && pairs_ == 1 && flush_))
	{
		return 12;
	}
	else if (royalstraight_ && flush_)
	{
		return 11;
	}
	else if ((fourofakind_ && jokers_ == 1) || (threeofakind_ && jokers_ == 2) || (threeofakindjoker_ && pairs_ == 1))
	{
		return 10;
	}
	else if (flush_ && straight_)
	{
		return 8;
	}
	else if ((fourofakind_ && jokers_ == 0) || (threeofakind_ && jokers_ == 1) || (pairs_ == 1 && jokers_ == 2) || (threeofakindjoker_ && pairs_ == 0))
	{
		return 7;
	}
	else if ((threeofakind_ && pairs_ == 1) || (pairs_ == 2 && jokers_ == 1))
	{
		return 6;
	}
	else if (flush_ && !straight_)
	{
		return 5;
	}
	else if (straight_)
	{
		return 4;
	}
	else if ((pairs_ == 0 && jokers_ == 2) || (pairs_ == 1 && jokers_ == 1) || (threeofakind_ && pairs_ == 0))
	{
		return 3;
	}
	else if (pairs_ == 2 && jokers_ == 0)
	{
		return 2;
	}
	else if ((pairs_ == 1) || (pairs_ == 0 && jokers_ == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

std::string HandEvaluator::TypeToString(int handtype, bool scoringornot)
{
	if (scoringornot == false)
	{
		switch (handtype)
		{
		case 0:
			return "Highcard..\n";
			break;

		case 1:
			return "Pair\n";
			break;

		case 2:
			return "Two pair\n";
			break;

		case 3:
			return "Three of a kind\n";
			break;

		case 4:
			return "Straight\n";
			break;

		case 5:
			return "Flush\n";
			break;

		case 6:
			return "Full house!\n";
			break;

		case 7:
			return "Four of a kind!\n";
			break;

		case 8:
			return "Straight flush!!\n";
			break;
		
		case 10:
			return "FIVE OF A KIND!!!\n";
			break;

		case 11:
			return "ROYAL FLUSH!!!\n";
			break;

		case 12:
			return "FLUSH FIVE!!!\n";
			break;

		case 13:
			return "JOKER FIVE!!!!\n";
			break;

		default:
			return "Default case, something has gone terribly wrong..\n";
			break;
		}
	}
	else
	{
		switch (handtype)
		{
		case 0:
			return "Highcard";
			break;

		case 1:
			return "Pair";
			break;

		case 2:
			return "Two pair";
			break;

		case 3:
			return "Three of a kind";
			break;

		case 4:
			return "Straight";
			break;

		case 5:
			return "Flush";
			break;

		case 6:
			return "Full house";
			break;

		case 7:
			return "Four of a kind";
			break;

		case 8:
			return "Straight flush";
			break;

		case 10:
			return "Five of a kind";
			break;

		case 11:
			return "Royal flush";
			break;

		case 12:
			return "Flush five";
			break;

		case 13:
			return "Joker five";
			break;

		default:
			return "Default case, something has gone terribly wrong..\n";
			break;
		}
	}
}

int HandEvaluator::TieBreaker(int handtype, std::vector<Card>& playercards, std::vector<Card>& dealercards)
{
	std::string samehand = TypeToString(handtype, true);
	std::cout << "\n\033[4mTIE BREAKER\033[0m\n";
	switch (handtype)
	{
	case 0:
	{
		int playerhighcard = 0, dealerhighcard = 0;
		EvaluateHand(playercards);
		playerhighcard = biggestcard_;
		EvaluateHand(dealercards);
		dealerhighcard = biggestcard_;
		if (playerhighcard > dealerhighcard)
		{
			std::cout << "\033[32m\033[4mYour\033[0m " << samehand << " " << playerhighcard << " beats dealer's " <<
				samehand << " " << dealerhighcard << "\n";
			return 1;
		}
		else if (playerhighcard < dealerhighcard)
		{
			std::cout << "\033[31m\033[4mDealer's\033[0m " << samehand << " " << dealerhighcard << " beats your " <<
				samehand << " " << playerhighcard << "\n";
			return 0;
		}
		else
		{
			std::cout << "\033[33mDRAW!\033[0m\nBoth have the same " << samehand << " " << playerhighcard << "\n";
		}
		break;
	}
	case 12:
	case 10:
	case 7:
	case 3:
	case 1:
	{
		int playersamecard = 0, dealersamecard = 0;
		EvaluateHand(playercards);
		playersamecard = biggestsamecard_;
		EvaluateHand(dealercards);
		dealersamecard = biggestsamecard_;
		if (playersamecard > dealersamecard)
		{
			std::cout << "\033[32m\033[4mYour\033[0m " << samehand << " " << playersamecard << " beats dealer's " <<
				samehand << " " << dealersamecard << "\n";
			return 1;
		}
		else if (playersamecard < dealersamecard)
		{
			std::cout << "\033[31m\033[4mDealer's\033[0m " << samehand << " " << dealersamecard << " beats your " <<
				samehand << " " << playersamecard << "\n";
			return 0;
		}
		else
		{
			std::cout << "\033[33mDRAW!\033[0m\nBoth have the same " << samehand << " " << playersamecard << "\n";
		}
		break;
	}
	case 6:
	case 2:
	{
		int playerpairsum = 0, dealerpairsum = 0;
		EvaluateHand(playercards);
		for (int i : samecardvalues_)
		{
			playerpairsum += i;
		}
		EvaluateHand(dealercards);
		for (int i : samecardvalues_)
		{
			dealerpairsum += i;
		}
		if (playerpairsum > dealerpairsum)
		{
			std::cout << "\033[32m\033[4mYour sum\033[0m " << playerpairsum << " beats dealer's sum " << dealerpairsum << "\n";
			return 1;
		}
		else if (playerpairsum < dealerpairsum)
		{
			std::cout << "\033[31m\033[4mDealer's sum\033[0m " << dealerpairsum << " beats your sum " << playerpairsum << "\n";
			return 0;
		}
		else
		{
			std::cout << "\033[33mDRAW!\033[0m\nBoth have the same sum\n";
		}
		break;
	}
	case 5:
	{
		int playerflushsum = 0, dealerflushsum = 0, jokeramount = 0;
		EvaluateHand(playercards);
		jokeramount = jokers_;
		for (int i = 0; i < jokers_; i++)
		{
			playerflushsum += 14;
		}
		playerflushsum += sumofcards_;
		EvaluateHand(dealercards);
		jokeramount = jokers_;
		for (int i = 0; i < jokers_; i++)
		{
			dealerflushsum += 14;
		}
		dealerflushsum += sumofcards_;
		if (playerflushsum > dealerflushsum)
		{
			std::cout << "\033[32m\033[4mYour sum\033[0m " << playerflushsum << " beats dealer's sum " << dealerflushsum << "\n";
			return 1;
		}
		else if (playerflushsum < dealerflushsum)
		{
			std::cout << "\033[31m\033[4mDealer's sum\033[0m " << dealerflushsum << " beats your sum " << playerflushsum << "\n";
			return 0;
		}
		else
		{
			std::cout << "\033[33mDRAW!\033[0m\nBoth have the same sum\n";
		}
		break;
	}
	case 8:
	case 4:
	{
		int playerstraightsum = 0, dealerstraightsum = 0;
		EvaluateHand(playercards);
		playerstraightsum = sumofcards_ + straightjokervalue_;
		EvaluateHand(dealercards);
		dealerstraightsum = sumofcards_ + straightjokervalue_;
		if (playerstraightsum > dealerstraightsum)
		{
			std::cout << "\033[32m\033[4mYour sum\033[0m " << playerstraightsum << " beats dealer's sum " << dealerstraightsum << "\n";
			return 1;
		}
		else if (playerstraightsum < dealerstraightsum)
		{
			std::cout << "\033[31m\033[4mDealer's sum\033[0m " << dealerstraightsum << " beats your sum " << playerstraightsum << "\n";
			return 0;
		}
		else
		{
			std::cout << "\033[33mDRAW!\033[0m\nBoth have the same sum\n";
		}
		break;
	}
	case 11:
	case 13:
	{
		std::cout << "Both have " << samehand << ", tiebreak not possible\n";
		break;
	}
	default:
		std::cout << "Error in tiebreaker, unknown handtype\n";
	}
}
