#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

//Forward declarations
class Player;
class Dealer;
class SaveManager;
struct SaveDataPoker;
struct SaveDataBlackjack;
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
	void BlackJack(Player&, Dealer&, SaveDataBlackjack&, SaveManager&);
	void Poker(Player&, Dealer&, SaveDataPoker&, SaveManager&);
private:
};

#endif // !GAMEMANAGER_H
