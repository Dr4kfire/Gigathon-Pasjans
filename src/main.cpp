#include "card.h"
#include "display_mngr.h"
#include <iostream>

int main()
{
	Deck deck;
	Display display;

	deck.GenerateFullDeck();
	std::cout << "Hello";
	display.ClearConsole();
	std::cout << "welcome!";
	char aaa[56];
	std::cin >> aaa;
}
