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
	bool hidden = true;
};

class Deck
{
public:
	static Deck GenerateFullDeck();

	Deck(bool draw_as_column = false) : draw_as_column(draw_as_column)
	{
		cards.reserve(52);
	}

	void Shuffle();

	void AppendCard(struct Card card);
	Card PopFrontCard();

	int GetSize();

	const struct Card &GetConstCardReference(int index);

private:
	std::vector<struct Card> cards;
	bool draw_as_column = false;
};