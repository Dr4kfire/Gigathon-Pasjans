#pragma once

#include <vector>

// Stores all the information about the cards
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


// Stores the Card structs and contains usefull 
// methods for deck and card manipulation
class Deck
{
public:
	// Constructor
	Deck(bool draw_as_column = false);


	// Returns a new, full and sorted deck 
	static Deck GenerateFullDeck();


	// Shuffles the deck
	void Shuffle();

	// Returns a size_t size of the m_cards vector
	size_t GetSize();


	// Appends cards to the m_cards vector
	void AppendCard(Card card, bool hide_card = true);

	// Returns the front card (last one in the vector)
	// and removes it from the m_cards vector
	Card PopFrontCard();

	// Returns the index card, segregates the vector
	// and removes it from this deck
	Card PopAt(int index);

	// Returns a constant Card reference
	const Card &GetConstCardReference(int index);

	// Returns a non constant Card reference
	Card &GetCardReference(int index);

	// Checks if the card can be repositioned into this
	// deck
	bool CanRepositionCard(Card& card);

public:
	// If true the cards inside this deck will be drawn
	// like a column (the first index is on the top and 
	// the last one on the bottom)
	bool draw_as_column = false;

	// Used for limiting the number of cards that can be
	// drawn inside a column (works only if 
	// the draw_as_column variable is true) - draws only
	// the top max_column_size cards from the m_cards vector
	int max_column_size = 13;

	// If true the whole deck's cards will have their
	// information obscured using '?' and the color
	// will be the default one
	bool hidden = false;

	// If true the deck will behave diffrently when the
	// player will try to append cards to it - it will
	// accept cards with only the same suit and ranks
	// being sorted ascending with the 'A' on the bottom
	// and the 'K' on the top
	bool sort_deck = false;

	// If true this deck will not accept any cards that
	// player might want to append to this deck
	bool draw_only = false;

private:
	std::vector<Card> m_cards;
};