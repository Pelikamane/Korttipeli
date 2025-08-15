#define NOMINMAX //needed for windows.h not to interrupt <limits>
#include <Windows.h>

#include "Dealer.h"
#include "Player.h"
#include "GameManager.h"
#include "SaveData.h"
#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"

//Allow statistics even if the files are empty
// ClearPokerData() doesn't work, says that the file is empty but if you play 1 game and then look at the stats, they are still there
//Add clutch rounds to poker: 10 rounds, player = 2 and dealer = 7 (consecutive wins allow you to go to overtime and possibly win the game) <-- choice included when it happens
//Blackjack: fix dealer hitting at 21.

int main()
{
	bool programloop = true;
	SetConsoleOutputCP(CP_UTF8); //Probably needed

	while (programloop)
	{
		GameManager gm;
		gm.Run();
		return 0;
	}
}
