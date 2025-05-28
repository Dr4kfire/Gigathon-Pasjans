#include "card.h"
#include <cstdlib>

void Deck::Shuffle()
{
	for (int i = 0; i < m_cards.size(); i++)
	{
		int rand_index = rand() % (i + 1);

		struct Card temp = m_cards[i];
		m_cards[i] = m_cards[rand_index];
		m_cards[rand_index] = temp;
	}
}

void Deck::AppendCard(struct Card card)
{
	if (m_cards.size() > 52)
		m_cards.emplace_back(card);
}

Card Deck::PopFrontCard()
{
	Card drawn_card = std::move(m_cards.back());
	m_cards.pop_back();
	return drawn_card;
}

const Card& Deck::GetTopCard() {
	return m_cards.back();
}

size_t Deck::GetSize() 
{
	return m_cards.size();
}

const struct Card &Deck::GetConstCardReference(int index)
{
	const struct Card &card = m_cards[index];
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
			new_deck.m_cards.push_back({j, current_suit, true});
		}
	}
	return new_deck;
}
