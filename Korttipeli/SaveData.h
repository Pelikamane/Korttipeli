#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <string>
#include <iostream>

struct SaveDataPoker
{
	int totalgames_ = 0, wins_ = 0, highcard_ = 0, pair_ = 0, twopair_ = 0, three_ = 0, straight_ = 0, flush_ = 0, fullhouse_ = 0,
		four_ = 0, straightflush_ = 0, royalflush_ = 0, five_ = 0, flushfive_ = 0, jokerfive_ = 0;
};
struct SaveDataBlackjack
{
	int totalgames_ = 0, wins_ = 0, draws_ = 0, blackjacks_ = 0;
};

struct SaveDataMoney
{
	int money_ = 0, highscore_ = 0, lowscore = 0, biggestbet_ = 0;

	int SetBet() const
	{
		unsigned int bet;
		while (true)
		{
			std::cout << "\n\033[32mMoney:\033[0m " << money_ << "\n" << "Select your bet: ";
			std::cin >> bet;
			if (!std::cin.good())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Wrong input\n\n";
				continue;
			}
			else
			{
				std::cout << "Your bet is: " << bet << "\n\n";
			}
			break;
		}
		return bet;
	}
};

struct Settings
{
	bool moneysystem;
};

class SaveManager
{
public:
	bool SavePokerData(const SaveDataPoker&);
	bool LoadPokerData(SaveDataPoker&);
	bool SaveBlackjackData(const SaveDataBlackjack&);
	bool LoadBlackjackData(SaveDataBlackjack&);
	bool SaveMoneyData(const SaveDataMoney&);
	bool LoadMoneyData(SaveDataMoney&);
	bool SaveSettingData(const Settings&);
	bool LoadSettingData(Settings&);
	void PokerDataToStruct(SaveDataPoker&, const int&);
	void ClearPokerSaveData() const;
	void ClearBlackjackSaveData() const;

private:
	std::string pokersavelocation_ = "PokerData.txt";
	std::string blackjacksavelocation_ = "BlackjackData.txt";
	std::string moneysavelocation_ = "MoneyData.txt";
	std::string settingsavelocation_ = "SettingData.txt";
};

#endif //!SAVEDATA_H