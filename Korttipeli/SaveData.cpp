#include "SaveData.h"

#include <iostream>
#include <fstream>

bool SaveManager::SavePokerData(const SaveDataPoker& savedata)
{
	std::fstream writedata(pokersavelocation_, std::ios::out);

	if (!writedata.is_open())
	{
		std::cout << "Error opening SavePokerData, creating save file\n";
		return false;
	}
	else
	{
		writedata << savedata.totalgames_ << "\n" << savedata.wins_ << "\n" << savedata.highcard_ << "\n" << 
			savedata.pair_ << "\n" << savedata.twopair_ << "\n" << savedata.three_ << "\n" << savedata.straight_ << "\n" << 
			savedata.flush_ << "\n" << savedata.fullhouse_ << "\n" << savedata.four_ << "\n" << savedata.straightflush_ << "\n" << 
			savedata.royalflush_ << "\n" << savedata.five_ << "\n" << savedata.flushfive_ << "\n" << savedata.jokerfive_;
		writedata.close();
		return true;
	}
}

bool SaveManager::LoadPokerData(SaveDataPoker& savedata)
{
	std::fstream readdata(pokersavelocation_, std::ios::in);
	if (!readdata.is_open())
	{
		std::cout << "\nSavefile doesn't exist, creating one\n";
		std::fstream createsave(pokersavelocation_, std::ios::out);
		createsave.close();
	}
	else if (readdata.is_open())
	{
		std::cout << "\nSavefile opened!\n";
		readdata.seekg(0, std::ios::end);
		if (readdata.tellg() == 0)
		{
			std::cout << "Save file is empty.\n";
			readdata.close();
			return false;
		}
		readdata.seekg(0, std::ios::beg);
		std::cout << "File is not empty.\n";
		readdata >> savedata.totalgames_ >> savedata.wins_ >> savedata.highcard_ >> savedata.pair_ >> savedata.twopair_ >>
			savedata.three_ >> savedata.straight_ >> savedata.flush_ >> savedata.fullhouse_ >>
			savedata.four_ >> savedata.straightflush_ >> savedata.royalflush_ >> savedata.five_ >>
			savedata.flushfive_ >> savedata.jokerfive_;
		readdata.close();
		std::cout << "Data is fetched\n";
		return true;
	}
}

bool SaveManager::SaveBlackjackData(const SaveDataBlackjack& savedata)
{
	std::fstream writedata(blackjacksavelocation_, std::ios::out);
	if (!writedata.is_open())
	{
		std::cout << "Error: blackjack save doesn't exist\n";
		return false;
	}
	else
	{
		writedata << savedata.totalgames_ << "\n" << savedata.wins_ << "\n" << savedata.draws_ << "\n" << savedata.blackjacks_;
		writedata.close();
	}
	return true;
}

bool SaveManager::LoadBlackjackData(SaveDataBlackjack& savedata)
{
	std::fstream readdata(blackjacksavelocation_, std::ios::in);
	if (!readdata.is_open())
	{
		std::cout << "Blackjack save doesn't exist, creating one\n";
		std::fstream createsave(blackjacksavelocation_, std::ios::out);
		createsave.close();
	}
	else if (readdata.is_open())
	{
		std::cout << "\nBlackjack save opened";
		readdata.seekg(0, std::ios::end);
		if (readdata.tellg() == 0)
		{
			std::cout << "\nBlackjack save is empty.\n";
			readdata.close();
			return false;
		}
		std::cout << "\nblackjack file is not empty.\n";
		readdata >> savedata.totalgames_ >> savedata.wins_ >> savedata.draws_ >>savedata.blackjacks_;
		readdata.close();
		std::cout << "Blackjack data is fetched\n";
		return true;
	}
}

void SaveManager::PokerDataToStruct(SaveDataPoker& pokersave, const int& handtype)
{
	switch (handtype)
	{
	case 0:
		pokersave.highcard_++;
		break;
	case 1:
		pokersave.pair_++;
		break;
	case 2:
		pokersave.twopair_++;
		break;
	case 3:
		pokersave.three_++;
		break;
	case 4:
		pokersave.straight_++;
		break;
	case 5:
		pokersave.flush_++;
		break;
	case 6:
		pokersave.fullhouse_++;
		break;
	case 7:
		pokersave.four_++;
		break;
	case 8:
		pokersave.straightflush_++;
		break;
	case 10:
		pokersave.five_++;
		break;
	case 11:
		pokersave.royalflush_++;
		break;
	case 12:
		pokersave.flushfive_++;
		break;
	case 13:
		pokersave.jokerfive_++;
		break;
	default:
		std::cout << "Error adding handtype to savefile\n";
	}
}

void SaveManager::ClearPokerSaveData() const
{
	std::ofstream(pokersavelocation_, std::ios::trunc);
	std::cout << "Poker data has been cleared!\n";
}