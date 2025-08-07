#define NOMINMAX //needed for windows.h not to interrupt <limits>
#include <Windows.h>

#include "Dealer.h"
#include "Player.h"
#include "GameManager.h"
#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"

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
