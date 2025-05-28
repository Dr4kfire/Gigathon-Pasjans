#include "card.h"
#include <cstdlib>
#include <algorithm>
#include <random>


void Deck::AppendCard(Card card, bool hide_card)
{
	if (m_cards.size() > 52)
		return;

	card.hidden = hide_card;
	m_cards.emplace_back(card);
}

Card Deck::PopFrontCard()
{
	Card drawn_card = std::move(m_cards.back());
	drawn_card.hidden = false;
	m_cards.pop_back();
	return drawn_card;
}

const Card& Deck::GetConstCardReference(int index)
{
	const Card& card = m_cards[index];
	return card;
}

Card& Deck::GetCardReference(int index) {
	Card& card = m_cards[index];
	return card;
}


void Deck::Shuffle()
{
    std::random_device rd;
    std::mt19937 rng(rd());

    std::shuffle(m_cards.begin(), m_cards.end(), rng);
}


size_t Deck::GetSize() 
{
	return m_cards.size();
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
