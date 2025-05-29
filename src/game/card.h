#pragma once

#include <vector>

struct Card
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

class Deck
{
public:
	static Deck GenerateFullDeck();

	Deck(bool draw_as_column = false)
	    : draw_as_column(draw_as_column)
	{
		m_cards.reserve(52);
	}

	void AppendCard(Card card, bool hide_card = true);

	Card PopFrontCard();
	Card PopAt(int index);
	const Card &GetConstCardReference(int index);
	Card &GetCardReference(int index);

	void Shuffle();

	size_t GetSize();

public:
	bool draw_as_column = false;
	int max_column_size = 13;
	bool hidden = false;

	bool sort_deck = false;
	bool draw_only = false;

private:
	std::vector<Card> m_cards;
};