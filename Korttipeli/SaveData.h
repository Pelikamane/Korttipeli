#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <string>

struct SaveDataPoker
{
	int totalgames_ = 0, wins_ = 0, highcard_ = 0, pair_ = 0, twopair_ = 0, three_ = 0, straight_ = 0, flush_ = 0, fullhouse_ = 0,
		four_ = 0, straightflush_ = 0, royalflush_ = 0, five_ = 0, flushfive_ = 0, jokerfive_ = 0;
};
struct SaveDataBlackjack
{
	int totalgames_ = 0, wins_ = 0, draws_ = 0, blackjacks_ = 0;
};

class SaveManager
{
public:
	bool SavePokerData(const SaveDataPoker&);
	bool LoadPokerData(SaveDataPoker&);
	bool SaveBlackjackData(const SaveDataBlackjack&);
	bool LoadBlackjackData(SaveDataBlackjack&);
	void PokerDataToStruct(SaveDataPoker&, const int&);
	void ClearPokerSaveData() const;
	

private:
	std::string pokersavelocation_ = "PokerData.txt";
	std::string blackjacksavelocation_ = "BlackjackData.txt";
};

#endif //!SAVEDATA_H