#pragma once

#include "../card.h"
#include "scene.h"

struct GameDecks
{
	Deck additional;
	Deck draw_pile;
	Deck sorted[4];
	Deck columns[7];
};

class GameScene : public Scene
{
public:
	GameScene(const std::string &scene_name) : Scene(scene_name)
	{
		Deck full_deck = Deck::GenerateFullDeck();
		full_deck.Shuffle();

		// COLUMNS
		for (int column_idx = 0; column_idx < 7; column_idx++)
		{
			m_game_decks.columns[column_idx].draw_as_column = true;
			for (int card_idx = 0; card_idx < column_idx + 1; card_idx++)
			{
				bool hide_card = true;

				if (card_idx == column_idx)
					hide_card = false;

				m_game_decks.columns[column_idx].AppendCard(
				    full_deck.PopFrontCard(), hide_card);
			}
		}

		// ADDITIONAL DECK
		for (int card_idx = 0; card_idx < full_deck.GetSize(); card_idx++)
		{
			m_game_decks.additional.AppendCard(full_deck.PopFrontCard(), false);
		}
		m_game_decks.additional.hidden = true;

		// DRAW PILE DECK
		if (m_hard_mode)
		{
			m_game_decks.draw_pile.draw_as_column = true;
		}
		m_game_decks.draw_pile.max_column_size = 3;

		// SORTED DECKS
		m_game_decks.sorted[0].sort_deck = true;
		m_game_decks.sorted[1].sort_deck = true;
		m_game_decks.sorted[2].sort_deck = true;
		m_game_decks.sorted[3].sort_deck = true;
	}

	void Draw() override;
	void Process(const int &input) override;

private:
	std::string GetCardString(const Card &card);
	void DrawCard(int card_index, Deck &deck, int pos_y, int pos_x);
	void DrawDeck(Deck &deck, int pos_y, int pos_x);

	void DrawCardsFromAdditional(int ammount);
	void ReplenishCardsToAdditional();

	bool CanRepositionCard(Deck &new_deck, Card &card);
	void RepositionCards(Deck &original_deck, Deck &new_deck, int first_card_index, bool only_one_card = false);

	bool m_hard_mode = false;
	bool m_full_ascii = false;

	GameDecks m_game_decks;

	int m_cursor_x = 0;
	int m_cursor_y = 0;

	int m_hovered_pos_x = 0;
	int m_hovered_pos_y = 0;

	int m_selected_pos_x = -1;
	int m_selected_pos_y = -1;

	int m_selected_card_index = -1;
	Deck *m_selected_deck = nullptr;
	Deck *m_hovered_deck = nullptr;
};