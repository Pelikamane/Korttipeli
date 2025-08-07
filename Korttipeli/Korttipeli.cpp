#include <string>
#include <limits> //for numeric_limits 
#include <numeric>
#include <vector>
#include <iostream>
#include <ios> //for <streamsize>
#define NOMINMAX //needed for windows.h not to interrupt <limits>
#include <Windows.h> //Probably not needed
#include <chrono> //Seeds rng
#include <random> //enables std::default_random_engine
#include <algorithm> //enables std::shuffle
#include <unordered_map> //Comparison for card point scoring

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
