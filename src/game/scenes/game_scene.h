#pragma once

#include "scene.h"
#include "../card.h"


struct GameDecks {
	Deck additional;
	Deck draw_pile;
	Deck sorted[4];
	Deck columns[7];
};


class GameScene : public Scene
{
public:
	GameScene(const std::string &scene_name) : Scene(scene_name) {}

	void Draw() override;
	void Process(const int &input) override;

private:
	void GetCardDrawBuffer(Card& card, char* buffer, size_t buffer_size);
	void GetDeckDrawBuffer(Deck& deck, char* buffer, size_t buffer_size);

	bool m_hard_mode = false;
	bool m_full_ascii = false;

	GameDecks m_game_decks;
};