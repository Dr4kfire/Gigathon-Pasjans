#pragma once

#include "scenes/game_scene.h"
#include "scenes/main_menu.h"
#include "scenes/scene.h"

// Used for scene managememt and some basic 
// game wide inputs
class Game
{
public:
	// Constructor
	Game()
	    : m_main_menu("main_menu"), m_game_scene("game_scene"), m_current_scene(&m_main_menu) {}

	
	// Draw function - used to draw the characters
	// on the screen
	void Draw();

	// Used mostly for inputs - run every frame
	// after the Draw() method
	void Process(const int &input);


	// Used to change the m_current_scene
	void ChangeScene(Scene &new_scene);

private:
	// Changes the difficulty of the game (those
	// changes and more are implemented inside
	// of the game_scene class)
	bool m_hard_mode = false;

	// Not fully implemented - changes how the 
	// cards are rendered
	bool m_full_acii = false;

	// Currently used and drawn scene
	Scene *m_current_scene;

	// Main menu scene
	MainMenu m_main_menu;

	// Game scene - this class implements most
	// of the game's logic and mechanics
	GameScene m_game_scene;
};
