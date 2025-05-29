#pragma once

#include <string>


// Stores all the information about the cards
struct CardData
{
	int rank;
	enum suits
	{
		HEARTS = 0,
		DIAMONDS = 1,
		CLUBS = 2,
		SPADES = 3,
	};
	suits suit;
	bool hidden = false;
};


// Used to store important data
class Card {
public:
	Card(CardData card_data) : card_data(card_data) {}


	// Draws the card to the screen
	void Draw(int pos_y, int pos_x) const;

	
	// SETTERS
	void SetHover(bool is_hovered) { m_hovered = is_hovered; }
	void SetSelected(bool is_selected) { m_selected = is_selected; }

public:
	CardData card_data;

private:
	// These constants are used for drawing the card on the screen
	static const char *CARD_TEMPLATE;
	static const char *CARD_HIDDEN;

	static const char SUIT_CHARS[4];
	static const std::string RANK_STRINGS[13];


	// Will be drawn with a diffrent color
	// when true
	bool m_hovered = false;

	// Will be drawn with a diffrent color
	// when true
	bool m_selected = false;
};