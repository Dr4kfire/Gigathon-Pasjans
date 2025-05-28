#pragma once

#include "scenes/game_scene.h"
#include "scenes/main_menu.h"
#include "scenes/scene.h"

class Game
{
public:
	Game();
	~Game() = default;

	void Draw();
	void Process(const int &input);

	void ChangeScene(Scene &new_scene);

private:
	bool m_hard_mode = false;
	bool m_full_acii = false;

	Scene *current_scene;

	MainMenu main_menu;
	GameScene game_scene;
};
