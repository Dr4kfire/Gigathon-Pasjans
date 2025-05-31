#pragma once

#include "scene.h"

#include "../deck.h"
#include <vector>


struct GameDecks
{
	Deck additional;
	Deck draw_pile;
	Deck sorted[4];
	Deck columns[7];
};


struct GameState {
	GameDecks game_decks;
};


class GameScene : public Scene
{
public:
	GameScene(const std::string &scene_name);

	// Used to draw the scene
	void Draw() override;
	// Mostly used for input handling
	void Process(const int &input) override;

	void DrawDecks(int position_x);

	// Undo functionality
	void UpdateStateHistory();
	void LoadLastState();

public:
	bool m_hard_mode = false;

private:
	void DrawCardsFromAdditional(int ammount);
	void ReplenishCardsToAdditional();

	Deck* GetHoveredDeck();
	void UpdateDecks();

	// Game data
	std::vector<GameState> m_states_history;
	int m_moves = 0;
	GameDecks m_game_decks;

	// Cursor functionality variables
	int m_cursor_x = 0;
	int m_cursor_y = 0;

	int m_hovered_pos_x = 0;
	int m_hovered_pos_y = 0;

	Deck *m_selected_deck = nullptr;
	Deck *m_hovered_deck = nullptr;
};