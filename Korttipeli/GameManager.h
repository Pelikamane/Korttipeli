#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

//Forward declarations
class Player;
class Dealer;
class SaveManager;
struct SaveDataPoker;
struct SaveDataBlackjack;
struct SaveDataMoney;
struct Settings;

class GameManager
{
public:
	GameManager()
	{

	}
	void Run();
	void ShowRules();
	void ShowStats(SaveManager&,SaveDataPoker&,SaveDataBlackjack&);
	void ShowSettings(SaveManager&, Settings&);
	void DevNotes();
	void SaveManagement(SaveManager&);
	void BlackJack(Player&, Dealer&, SaveDataBlackjack&, SaveDataMoney&, SaveManager&, Settings&);
	void Poker(Player&, Dealer&, SaveDataPoker&, SaveDataMoney&, SaveManager&, Settings&);
private:
};

#endif // !GAMEMANAGER_H
