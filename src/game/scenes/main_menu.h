#include "scene.h"

// The greeting screen and the place where the 
// player can change some game setting
class MainMenu : public Scene
{
public:
	// Constructor
	MainMenu(const std::string &scene_name)
	    : Scene(scene_name) {}


	// Draw function - used to draw the characters
	// on the screen
	void Draw() override;

	// Used mostly for inputs - run every frame
	// after the Draw() method
	void Process(const int &input) override;

private:
	// Stores the options that the player chooses
	// to later pass onto the next scenes
	enum Options
	{
		START,
		DIFFICULTY,
		SETTINGS,
	};
	Options m_selected_option = Options::START;
	int m_option_idx = 0;

	// Changes the difficulty of the game (those
	// changes and more are implemented inside
	// of the game_scene class)
	bool m_hard_mode = false;

	// Not fully implemented - changes how the 
	// cards are rendered
	bool m_full_ascii = false;
};