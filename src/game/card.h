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

	Deck(bool draw_as_column = false) : draw_as_column(draw_as_column)
	{
		m_cards.reserve(52);
	}

	void Shuffle();

	void AppendCard(struct Card card);
	Card PopFrontCard();
	const Card& GetTopCard();

	size_t GetSize();

	const struct Card &GetConstCardReference(int index);

public:
	bool draw_as_column = false;
	int max_column_size = 13;
	bool hidden = false;

private:
	std::vector<struct Card> m_cards;
};