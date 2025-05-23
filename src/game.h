#pragma once

#include "card.h"
#include "display_mngr.h"
#include <string>

struct GameSettings
{
	enum Difficulties
	{
		NORMAL,
		HARD,
	};
	Difficulties difficulty = NORMAL;
};

struct GameDecks
{
	Deck column_decks[7];
	Deck suit_decks[4];
	Deck additional_deck;
	Deck draw_pile;
};

class Game
{
private:
	enum GameState
	{
		MAIN_MENU,
		GAME,
		WIN,
	};

public:
	Game();

	void Draw();
	std::string GetInput();

	void ChangeState(GameState state);

private:
	void PrepareDecks();

	Display display_manager;
	struct GameSettings settings;
	struct GameDecks decks;
	GameState state;
};