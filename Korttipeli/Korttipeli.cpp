#define NOMINMAX //needed for windows.h not to interrupt <limits>
#include <Windows.h>

#include "Dealer.h"
#include "Player.h"
#include "GameManager.h"
#include "SaveData.h"
#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"

//Add holdem and normal blackjack 
//Add colors to the easter eggs
//Blackjack: fix dealer hitting at 21. (might work now)

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
