#include "card.h"
#include <algorithm>
#include <cstdlib>
#include <random>

Deck::Deck(bool draw_as_column)
    : draw_as_column(draw_as_column)
{
	m_cards.reserve(52);
}

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

Card Deck::PopAt(int index)
{
	Card removed = std::move(m_cards[index]);
	m_cards.erase(m_cards.begin() + index);
	return removed;
}

const Card &Deck::GetConstCardReference(int index)
{
	const Card &card = m_cards[index];
	return card;
}

Card &Deck::GetCardReference(int index)
{
	Card &card = m_cards[index];
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

bool Deck::CanRepositionCard(Card &card)
{
	// Disable repositioning into draw_only decks
	if (draw_only)
	{
		return false;
	}

	// Make seperate checks for the sort_deck decks
	if (sort_deck)
	{
		// If the deck is empty allow only Aces
		if (GetSize() == 0 && card.rank == 0)
		{
			return true;
		}

		// Check with the top card of this deck
		const Card &top_card = GetConstCardReference(GetSize() - 1);
		// If the suit and rank matches - allow repositioning
		if ((top_card.suit == card.suit) && (top_card.rank + 1 == card.rank))
		{
			return true;
		}

		return false;
	}

	// If the deck is empty - allow only a King
	if (GetSize() == 0 && card.rank == 12)
	{
		return true;
	}

	// Check with the top card of this deck
	const Card &top_card = GetConstCardReference(GetSize() - 1);

	// Disable placing on hidden cards
	if (top_card.hidden)
	{
		return false;
	}

	// Allow only cards with the diffrent color (CLUBS, SPADES - HEARTS, DIAMONDS)
	if (top_card.suit == Card::CLUBS || top_card.suit == Card::SPADES)
	{
		if (card.suit == Card::CLUBS || card.suit == Card::SPADES)
		{
			return false;
		}
	}
	else
	{
		if (card.suit == Card::HEARTS || card.suit == Card::DIAMONDS)
		{
			return false;
		}
	}

	// Allow only cards with a lower rank
	if (top_card.rank - 1 != card.rank)
	{
		return false;
	}

	// Otherwise: player can place the card
	return true;
}