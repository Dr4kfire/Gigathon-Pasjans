#include "card.h"
#include <cstdlib>

void Deck::Shuffle()
{
	for (int i = 0; i < cards.size(); i++)
	{
		int rand_index = rand() % (i + 1);

		struct Card temp = cards[i];
		cards[i] = cards[rand_index];
		cards[rand_index] = temp;
	}
}

void Deck::AppendCard(struct Card card)
{
	if (cards.size() > 52)
		cards.emplace_back(card);
}

Card Deck::PopFrontCard()
{
	Card drawn_card = std::move(cards.back());
	cards.pop_back();
	return drawn_card;
}

const Card& Deck::GetTopCard() {
	return cards.back();
}

size_t Deck::GetSize() { return cards.size(); }

const struct Card &Deck::GetConstCardReference(int index)
{
	const struct Card &card = cards[index];
	return card;
}

Deck Deck::GenerateFullDeck()
{
	Deck new_deck;
	for (int i = 0; i < 4; i++)
	{
		Card::suits current_suit = static_cast<Card::suits>(i);

		for (int j = 0; j < 13; j++)
		{
			new_deck.cards.push_back({j, current_suit, true});
		}
	}
	return new_deck;
}
