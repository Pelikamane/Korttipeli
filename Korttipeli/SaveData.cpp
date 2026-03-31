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
		//std::cout << "\nSavefile opened!\n";
		readdata.seekg(0, std::ios::end);
		if (readdata.tellg() == 0)
		{
			//std::cout << "Save file is empty.\n";
			readdata.close();
			savedata.totalgames_ = 0, savedata.wins_ = 0, savedata.highcard_ = 0, savedata.pair_ = 0, savedata.twopair_ = 0,
				savedata.three_ = 0, savedata.straight_ = 0, savedata.flush_ = 0, savedata.fullhouse_ = 0, savedata.four_ = 0,
				savedata.straightflush_ = 0, savedata.royalflush_ = 0, savedata.five_ = 0, savedata.flushfive_ = 0, savedata.jokerfive_ = 0;
			return false;
		}
		readdata.seekg(0, std::ios::beg);
		//std::cout << "File is not empty.\n";
		readdata >> savedata.totalgames_ >> savedata.wins_ >> savedata.highcard_ >> savedata.pair_ >> savedata.twopair_ >>
			savedata.three_ >> savedata.straight_ >> savedata.flush_ >> savedata.fullhouse_ >>
			savedata.four_ >> savedata.straightflush_ >> savedata.royalflush_ >> savedata.five_ >>
			savedata.flushfive_ >> savedata.jokerfive_;
		readdata.close();
		//std::cout << "Data is fetched\n";
		return true;
	}
	return false;
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
		//std::cout << "\nBlackjack save opened";
		readdata.seekg(0, std::ios::end);
		if (readdata.tellg() == 0)
		{
			//std::cout << "\nBlackjack save is empty.\n";
			readdata.close();
			savedata.totalgames_ = 0, savedata.wins_ = 0, savedata.draws_ = 0, savedata.blackjacks_ = 0;
			return false;
		}
		readdata.seekg(0, std::ios::beg);
		//std::cout << "\nblackjack file is not empty.\n";
		readdata >> savedata.totalgames_ >> savedata.wins_ >> savedata.draws_ >> savedata.blackjacks_;
		readdata.close();
		//std::cout << "Blackjack data is fetched\n";
		return true;
	}
	return false;
}

bool SaveManager::SaveMoneyData(const SaveDataMoney& playermoney)
{
	std::fstream writedata(moneysavelocation_, std::ios::out);
	if (!writedata.is_open())
	{
		std::cout << "Error: money save doesn't exist\n";
		return false;
	}
	else
	{
		writedata << playermoney.money_ << "\n" << playermoney.highscore_ << "\n" << playermoney.biggestbet_;
		writedata.close();
	}
	return true;
}

bool SaveManager::LoadMoneyData(SaveDataMoney& playermoney)
{
	std::fstream readdata(moneysavelocation_, std::ios::in);
	if (!readdata.is_open())
	{
		std::cout << "Money data doesn't exist, creating one\n";
		std::fstream createsave(moneysavelocation_, std::ios::out);
		createsave.close();
	}
	else if (readdata.is_open())
	{
		std::cout << "\nMoney save opened";
		readdata.seekg(0, std::ios::end);
		if (readdata.tellg() == 0)
		{
			std::cout << "Money file empty\n";
			readdata.close();
			playermoney.money_ = 100, playermoney.highscore_ = 0, playermoney.biggestbet_ = 0;
			return false;
		}
		readdata.seekg(0, std::ios::beg);
		readdata >> playermoney.money_ >> playermoney.highscore_ >> playermoney.biggestbet_;
		readdata.close();
		return true;
	}
	return false;
}

bool SaveManager::SaveSettingData(const Settings& settings)
{
	std::fstream writedata(settingsavelocation_, std::ios::out | std::ios::trunc);
	if (!writedata.is_open())
	{
		std::cout << "Error: settings save doesn't exist\n";
		return false;
	}
	else
	{
		writedata << std::boolalpha << settings.moneysystem;
		writedata.close();
	}
	return true;
}

bool SaveManager::LoadSettingData(Settings& settings)
{
	std::fstream readdata(settingsavelocation_, std::ios::in);
	if (!readdata.is_open())
	{
		std::cout << "Settings data doesn't exist, creating one\n";
		std::fstream createsave(settingsavelocation_, std::ios::out);
		createsave.close();
	}
	else if (readdata.is_open())
	{
		//std::cout << "\Settings save opened";
		readdata.seekg(0, std::ios::end);
		if (readdata.tellg() == 0)
		{
			std::cout << "Settings file empty\n";
			settings.moneysystem = false;
			readdata.close();
			return false;
		}
		readdata.seekg(0, std::ios::beg);
		readdata >> std::boolalpha >> settings.moneysystem;
		readdata.close();
		return true;
	}
	return false;
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

void SaveManager::ClearBlackjackSaveData() const
{
	std::ofstream(blackjacksavelocation_, std::ios::trunc);
	std::cout << "Blackjack data has been cleared!\n";
}