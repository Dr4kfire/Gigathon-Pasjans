#pragma once

#include "card.h"
#include <cstdlib>
#include <vector>


// Stores the Card structs and contains usefull 
// methods for deck and card manipulation
class Deck
{
public:
	// Constructor
	Deck(bool draw_as_column = false);


	// Returns a new, full and sorted deck 
	static Deck GenerateFullDeck();


	// Draws the deck to the screen
	void DrawDeck(int pos_y, int pos_x);


	// Cards vector manipulation
	void Shuffle();
	size_t GetSize();

	void AppendCard(Card card, bool hide_card = true);

	Card PopFrontCard();

	Card PopAt(int index);


	// Card references
	const Card &GetConstCardReference(int index);
	Card &GetCardReference(int index);


	// Card repositioning logic
	bool CanRepositionCard(Card& card);

	void RepositionCards(Deck &original_deck, int first_card_index, bool only_one_card = false);

	void SetHoveredCard(int card_index);
	void SetSelectedCard(int card_index);
	int GetHoveredCard() { return m_hovered_card; }
	int GetSelectedCard() { return m_selected_card; }

public:
	// Draw information
	bool draw_as_column = false;
	int max_column_size = 13;
	bool hidden = false;
	bool hovered = false;

	// Deck position
	int pos_y, pos_x;

	// Deck behaviour
	bool sort_deck = false;
	bool disable_cards_append = false;
	bool only_top_movable = false;

private:
	// Deck's cards
	std::vector<Card> m_cards;

	int m_selected_card = -1;
	int m_hovered_card = -1;
};