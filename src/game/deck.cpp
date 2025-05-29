#include "deck.h"

// INCLUDE NCURSES (Unix) OR PDCURSES (Windows)
#ifdef _WIN32
#define PDC_FORCE_UTF8
#define PDC_WIDE
#define CHTYPE_64
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "card.h"
#include <algorithm>
#include <cstdlib>
#include <random>

Deck::Deck(bool draw_as_column)
    : draw_as_column(draw_as_column)
{
	m_cards.reserve(52);
}

void Deck::DrawDeck(int pos_y, int pos_x)
{
	// Draw empty space "[  ]"
	if (GetSize() == 0)
	{
		if (hovered) {
			attron(COLOR_PAIR(1));
		}

		mvprintw(pos_y, pos_x, "[   ]");
		attroff(COLOR_PAIR(1));
		return;
	}

	// Draw the whole deck hidden "[? ?]"
	if (hidden)
	{
		if (hovered) {
			attron(COLOR_PAIR(1));
		}

		mvprintw(pos_y, pos_x, "[? ?]");
		attroff(COLOR_PAIR(1));
		return;
	}

	// Draw only the top deck
	if (!draw_as_column)
	{
		Card &top_card = GetCardReference(GetSize()-1);
		top_card.SetHover(GetSize()-1 == hovered_card);
		top_card.SetSelected(GetSize()-1 == selected_card);
		top_card.Draw(pos_y, pos_x);
		return;
	}

	// Draw in a column
	int offset = 0;
	int first_index = std::clamp((int)(GetSize() - max_column_size), 0, (int)(GetSize()-1));

	for (int card_index = first_index; card_index < GetSize(); card_index++)
	{
		Card &card = GetCardReference(card_index);
		card.SetHover(card_index == hovered_card);
		card.SetSelected(card_index == selected_card);
		card.Draw(pos_y+offset, pos_x);
		offset++;
	}
}

void Deck::AppendCard(Card card, bool hide_card)
{
	if (m_cards.size() > 52)
		return;

	card.card_data.hidden = hide_card;
	m_cards.emplace_back(card);
}

Card Deck::PopFrontCard()
{
	Card drawn_card = std::move(m_cards.back());
	drawn_card.card_data.hidden = false;
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
		CardData::suits current_suit = static_cast<CardData::suits>(i);

		for (int j = 0; j < 13; j++)
		{
			new_deck.m_cards.push_back(Card({j, current_suit, true}));
		}
	}
	return new_deck;
}

bool Deck::CanRepositionCard(Card &card)
{
	// Disable repositioning into disable_cards_append decks
	if (disable_cards_append)
	{
		return false;
	}

	// Make seperate checks for the sort_deck decks
	if (sort_deck)
	{
		// If the deck is empty allow only Aces
		if (GetSize() == 0 && card.card_data.rank == 0)
		{
			return true;
		}

		// Check with the top card of this deck
		const Card &top_card = GetConstCardReference(GetSize() - 1);
		// If the suit and rank matches - allow repositioning
		if ((top_card.card_data.suit == card.card_data.suit) && (top_card.card_data.rank + 1 == card.card_data.rank))
		{
			return true;
		}

		return false;
	}

	// If the deck is empty - allow only a King
	if (GetSize() == 0 && card.card_data.rank == 12)
	{
		return true;
	}

	// Check with the top card of this deck
	const Card &top_card = GetConstCardReference(GetSize() - 1);

	// Disable placing on hidden cards
	if (top_card.card_data.hidden)
	{
		return false;
	}

	// Allow only cards with the diffrent color (CLUBS, SPADES - HEARTS, DIAMONDS)
	if (top_card.card_data.suit == CardData::CLUBS || top_card.card_data.suit == CardData::SPADES)
	{
		if (card.card_data.suit == CardData::CLUBS || card.card_data.suit == CardData::SPADES)
		{
			return false;
		}
	}
	else
	{
		if (card.card_data.suit == CardData::HEARTS || card.card_data.suit == CardData::DIAMONDS)
		{
			return false;
		}
	}

	// Allow only cards with a lower rank
	if (top_card.card_data.rank - 1 != card.card_data.rank)
	{
		return false;
	}

	// Otherwise: player can place the card
	return true;
}

void Deck::RepositionCards(Deck &original_deck, int first_card_index, bool only_one_card)
{
	if (only_one_card)
	{
		// Get the card from the original deck
		Card repositioned_card = original_deck.PopFrontCard();

		// Move it into this deck and unhide it
		AppendCard(std::move(repositioned_card), false);

		// Unhide the new top card in the original deck
		original_deck.GetCardReference(original_deck.GetSize() - 1).card_data.hidden = false;
		return;
	}

	int original_deck_size = original_deck.GetSize();
	// Loop through the cards starting from the selected
	// one and ending on the most top one in the array
	for (int card_idx = first_card_index; card_idx < original_deck_size; card_idx++)
	{
		// Get the card from the original deck
		// and reorganize the vector
		Card repositioned_card = original_deck.PopAt(first_card_index);
		// Move it into this deck and unhide it
		AppendCard(std::move(repositioned_card), false);
	}
	// Unhide the new top card in the original deck
	original_deck.GetCardReference(original_deck.GetSize() - 1).card_data.hidden = false;
}