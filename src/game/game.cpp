#include "game.h"
#include "scenes/scene.h"

Game::Game()
    : main_menu("main_menu"), game_scene("game_scene"),
      current_scene(&main_menu)
{
}

void Game::Draw() { current_scene->Draw(); }

void Game::Process(const int &input)
{
	current_scene->Process(input);

	if (current_scene->scene_name == main_menu.scene_name &&
	    main_menu.ShouldChangeScene())
	{
		m_hard_mode = main_menu.result["diff"];
		m_full_acii = main_menu.result["ascii"];
		ChangeScene(game_scene);
	}
}

void Game::ChangeScene(Scene &new_scene) { current_scene = &new_scene; }