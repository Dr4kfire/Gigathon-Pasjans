#include "game.h"

#include "card.h"
#include <utility>

Game::Game() { PrepareDecks(); }

void Game::Draw()
{
	display_manager.ClearConsole();
}

void Game::ChangeState(GameState state) {}

void Game::PrepareDecks()
{
	Deck full_deck = Deck::GenerateFullDeck();
	full_deck.Shuffle();

	for (int column = 0; column < 7; column++)
	{
		for (int card_idx = 0; card_idx < column + 1; card_idx++)
		{
			Card chosen_card = full_deck.PopFrontCard();
			decks.column_decks[column].AppendCard(std::move(chosen_card));
		}
	}

	for (int card_idx = 0; card_idx < full_deck.GetSize(); card_idx++)
	{
		Card left_card = full_deck.PopFrontCard();
		decks.additional_deck.AppendCard(std::move(left_card));
	}
}