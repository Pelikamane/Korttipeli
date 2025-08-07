#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

//Forward declarations
class Player;
class Dealer;

class GameManager
{
public:
	GameManager()
	{

	}
	void Run();
	void ShowRules();
	void BlackJack(Player&, Dealer&);
	void Poker(Player&, Dealer&);
private:

};

#endif // !GAMEMANAGER_H
