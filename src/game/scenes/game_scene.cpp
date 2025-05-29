#include "game_scene.h"
#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>

// Include the ncurses and pdcurses libraries
#ifdef _WIN32
#define PDC_FORCE_UTF8
#define PDC_WIDE
#define CHTYPE_64
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <cmath>
#include <cstring>

using std::string;

GameScene::GameScene(const std::string &scene_name) : Scene(scene_name)
{
	Deck full_deck = Deck::GenerateFullDeck();
	full_deck.Shuffle();

	// COLUMNS
	for (int column_idx = 0; column_idx < 7; column_idx++)
	{
		m_game_decks.columns[column_idx].draw_as_column = true;
		m_game_decks.columns[column_idx].pos_x = column_idx;
		// the Y positon is updated inside the process because it 
		// depends on the m_hard_mode setting
		for (int card_idx = 0; card_idx < column_idx + 1; card_idx++)
		{
			bool hide_card = true;

			if (card_idx == column_idx)
				hide_card = false;

			m_game_decks.columns[column_idx].AppendCard(
				full_deck.PopFrontCard(), hide_card);
		}
	}

	// ADDITIONAL DECK
	for (int card_idx = 0; card_idx < full_deck.GetSize(); card_idx++)
	{
		m_game_decks.additional.AppendCard(full_deck.PopFrontCard(), false);
	}
	m_game_decks.additional.hidden = true;
	m_game_decks.additional.pos_x = 0;
	m_game_decks.additional.pos_y = 0;

	// DRAW PILE DECK
	m_game_decks.draw_pile.max_column_size = 3;
	m_game_decks.draw_pile.disable_cards_append = true;
	m_game_decks.draw_pile.pos_x = 1;
	m_game_decks.draw_pile.pos_y = 0;

	// SORTED DECKS
	for (int i = 0; i < 4; i++) {
		m_game_decks.sorted[i].sort_deck = true;
		m_game_decks.sorted[i].pos_x = 3+i;
		m_game_decks.sorted[i].pos_y = 0;
	}

	m_states_history.reserve(3);
}


void GameScene::Draw()
{
	int size_y, size_x;
	getmaxyx(stdscr, size_y, size_x);
	int position_x;


	UpdateDecks();


	// SUITS INFORMATION
	const string SUITS_INFO = "[H]EARTS, [D]IAMONDS, [C]LUBS, [S]PADES";
	position_x = std::round((size_x - SUITS_INFO.length()) / 2.0);
	mvprintw(1, position_x, "%s", SUITS_INFO.c_str());


	// ADDITIONAL DECK, DRAW PILE & SORTED PILES
	position_x = std::ceil((size_x - 41) / 2.0);

	// converting the relative cursor position into
	// the global position
	m_hovered_pos_x = position_x + (6 * m_cursor_x);
	m_hovered_pos_y = 4 + m_cursor_y;
	if (m_cursor_y >= 1) {
		m_hovered_pos_y += 1;
	}

	DrawDecks(position_x);
	

	// CURSORS
	attron(COLOR_PAIR(1));
	// side cursor
	mvprintw(m_hovered_pos_y, 0, ">");
	// top cursor
	mvprintw(3, m_hovered_pos_x + 2, "\\");
	attroff(COLOR_PAIR(1));

	// UNDO INFORMATION
	mvprintw(size_y-1, 0, "Undos: %d / 3", (int)(m_states_history.size()));

	// MOVES INFORMATION
	string moves_info = "Moves: " + std::to_string(m_moves);
	position_x = std::round((size_x - moves_info.length()) / 2.0);
	mvprintw(size_y-1, position_x, "%s", moves_info.c_str());

	// RESET INFORMATION
	string reset_info = "Press `R` to reset";
	mvprintw(size_y-1, size_x - reset_info.length(), "%s", reset_info.c_str());
}

void GameScene::DrawDecks(int position_x) {
	// Draw the additional cards deck
	if (m_cursor_x == 0 && m_cursor_y == 0) 
	{
		attron(COLOR_PAIR(1));
	}
	m_game_decks.additional.DrawDeck(4, position_x);
	attroff(COLOR_PAIR(1));

	// Draw the draw_pile
	m_game_decks.draw_pile.DrawDeck(4, position_x + 6);

	// Draw the sorted decks
	for (int i = 0; i < 4; i++) {
		m_game_decks.sorted[i].DrawDeck(4, position_x + 18 + (6 * i));
	}

	// Draw the column decks
	int column_offset = 6;
	if (m_hard_mode) column_offset = 8;

	for (int i = 0; i < 7; i++)
	{
		m_game_decks.columns[i].DrawDeck(column_offset, position_x + (6 * i));
	}
}

void GameScene::Process(const int &input)
{
	UpdateDecks();

	switch (input)
	{
	case KEY_UP:
		m_cursor_y--;
		m_cursor_y = std::clamp(m_cursor_y, 0, (int)(m_game_decks.columns[m_cursor_x].GetSize()));
		break;
	case KEY_DOWN:
		m_cursor_y++;
		m_cursor_y = std::clamp(m_cursor_y, 0, (int)(m_game_decks.columns[m_cursor_x].GetSize()));
		break;
	case KEY_LEFT:
		m_cursor_x = std::max(0, --m_cursor_x);
		if (m_cursor_x == 2 && m_cursor_y == 0) {
			m_cursor_x = 1;
		}
		m_cursor_y = std::clamp(m_cursor_y, 0, (int)(m_game_decks.columns[m_cursor_x].GetSize()));
		break;
	case KEY_RIGHT:
		m_cursor_x = std::clamp(++m_cursor_x, 0, 6);
		if (m_cursor_x == 2 && m_cursor_y == 0) {
			m_cursor_x = 3;
		}
		m_cursor_y = std::clamp(m_cursor_y, 0, (int)(m_game_decks.columns[m_cursor_x].GetSize()));
		break;
	case '\n':
	case KEY_ENTER:
		// DRAW CARDS FROM ADDITIONAL
		if (m_hovered_deck == &m_game_decks.additional) {
			m_moves++;
			if (m_hard_mode)
			{
				DrawCardsFromAdditional(3);
				break;
			}
			DrawCardsFromAdditional(1);
			break;
		}
		// NOTHING SELECTED
		else if (m_selected_deck == nullptr)
		{
			m_selected_deck = m_hovered_deck;
			m_selected_deck->selected_card = m_cursor_y - m_selected_deck->pos_y;
		}
		// SELECTED SOMETHING THAT ALREADY IS SELECTED
		else if (m_selected_deck == m_hovered_deck)
		{
			m_selected_deck = nullptr;
		}
		// SELECTED SOMETHING THAT ISN'T SELECTED - CHECK IF CAN BE MOVED
		else if (m_hovered_deck && m_selected_deck && m_hovered_deck->CanRepositionCard(m_selected_deck->GetCardReference(m_selected_deck->selected_card)))
		{
			m_moves++;

			bool only_one_card = false;
			if (m_selected_deck == &m_game_decks.draw_pile)
			{
				only_one_card = true;
			}
			else if (m_selected_deck->sort_deck == true)
			{
				only_one_card = true;
			}

			UpdateStateHistory();
			m_hovered_deck->RepositionCards(*m_selected_deck, m_selected_deck->selected_card, only_one_card);

			m_selected_deck = nullptr;
		}
		else
		{
			m_selected_deck = m_hovered_deck;
			m_selected_deck->selected_card = m_cursor_y - m_selected_deck->pos_y;
		}
		break;
	default:
		break;
	}
}

void GameScene::UpdateDecks() {
	// Update the hover states of the decks
	m_game_decks.additional.hovered = false;
	m_game_decks.additional.selected_card = -1;
	m_game_decks.additional.hovered_card = -1;
	m_game_decks.draw_pile.hovered = false;
	m_game_decks.draw_pile.hovered_card = -1;
	m_game_decks.draw_pile.selected_card = -1;
	for (int i = 0; i < 4; i++) {
		m_game_decks.sorted[i].hovered = false;
		m_game_decks.sorted[i].selected_card = -1;
		m_game_decks.sorted[i].hovered_card = -1;
	}
	for (int i = 0; i < 7; i++) {
		m_game_decks.columns[i].hovered = false;
		m_game_decks.columns[i].selected_card = -1;
		m_game_decks.columns[i].hovered_card = -1;
	}

	m_game_decks.draw_pile.draw_as_column = m_hard_mode;

	// Update the pos_y of the column decks
	for (int i = 0; i < 7; i++) {
		m_game_decks.columns[i].pos_y = 1;
		if (m_hard_mode) m_game_decks.columns[i].pos_y = 3;
	}

	// Reset the hovered deck pointer
	m_hovered_deck = nullptr;

	// Check which deck is hovered
	if (m_game_decks.additional.pos_x == m_cursor_x && m_game_decks.additional.pos_y == m_cursor_y) {
		m_hovered_deck = &m_game_decks.additional;
	}
	else if (m_hard_mode && m_cursor_x == m_game_decks.draw_pile.pos_x) {
		int top = m_game_decks.draw_pile.pos_y;
		int bottom = top + std::min((int)m_game_decks.draw_pile.GetSize(), m_game_decks.draw_pile.max_column_size);
		if (m_cursor_y >= top && m_cursor_y <= bottom) {
			m_hovered_deck = &m_game_decks.draw_pile;
		}
	}
	else if (m_game_decks.draw_pile.pos_y == m_cursor_y && m_game_decks.draw_pile.pos_x == m_cursor_x) {
		m_hovered_deck = &m_game_decks.draw_pile;
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (m_game_decks.sorted[i].pos_x != m_cursor_x || m_game_decks.sorted[i].pos_y != m_cursor_y) {
				continue;
			}
			
			m_hovered_deck = &m_game_decks.sorted[i];
		}

		for (int i = 0; i < 7; i++) {
			Deck &col = m_game_decks.columns[i];
			col.hovered_card = -1;
			col.selected_card = -1;

			if (col.pos_x != m_cursor_x) {
				continue;
			}
			    
			int top = col.pos_y;
			int bottom = top + (int)col.GetSize() - 1;

			if (m_cursor_y < top || m_cursor_y > bottom) {
				continue;
			}

			col.hovered_card = m_cursor_y-col.pos_y;
			m_hovered_deck = &col;
			break;
		}
	}
}

void GameScene::UpdateStateHistory()
{
	GameState new_state = {m_game_decks};
	if (m_states_history.size() == 3)
	{
		m_states_history.erase(m_states_history.begin());
	}
	m_states_history.push_back(new_state);
}

void GameScene::LoadLastState()
{
	if (m_states_history.size() == 0)
	{
		return;
	}

	GameState last_state = std::move(m_states_history.back());
	m_states_history.pop_back();

	m_game_decks = last_state.game_decks;
	m_selected_deck = nullptr;
}

void GameScene::DrawCardsFromAdditional(int ammount)
{
	if (m_game_decks.additional.GetSize() == 0)
	{
		ReplenishCardsToAdditional();
		return;
	}

	if (m_game_decks.additional.GetSize() - ammount < 0)
	{
		for (int i = 0; i < m_game_decks.additional.GetSize(); i++)
		{
			Card new_card = m_game_decks.additional.PopFrontCard();
			m_game_decks.draw_pile.AppendCard(std::move(new_card), false);
		}
		return;
	}
	for (int i = 0; i < ammount; i++)
	{
		Card new_card = m_game_decks.additional.PopFrontCard();
		m_game_decks.draw_pile.AppendCard(std::move(new_card), false);
	}
}

void GameScene::ReplenishCardsToAdditional()
{
	for (int i = m_game_decks.draw_pile.GetSize() - 1; i >= 0; i--)
	{
		Card new_card = m_game_decks.draw_pile.PopAt(i);
		m_game_decks.additional.AppendCard(std::move(new_card), true);
	}
	m_game_decks.additional.Shuffle();
}