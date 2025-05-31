#include "game.h"
#include "scenes/game_scene.h"
#include "scenes/scene.h"

void Game::Draw()
{
	m_current_scene->Draw();
}

void Game::Process(const int &input)
{
	// Input handling
	switch (input)
	{
	// Game resetting
	case 'R':
	case 'r':
		if (m_current_scene->scene_name != m_game_scene.scene_name)
		{
			break;
		}
		m_game_scene = GameScene("m_game_scene");
		m_game_scene.m_hard_mode = m_hard_mode;
		break;
	// Undo
	case 'U':
	case 'u':
		if (m_current_scene->scene_name != m_game_scene.scene_name)
		{
			break;
		}
		m_game_scene.LoadLastState();
		break;
	default:
		break;
	}
	// Pass inputs to the current scene
	m_current_scene->Process(input);

	// Change the scene and pass the inputed settings values
	if (m_current_scene->scene_name == m_main_menu.scene_name && m_main_menu.ShouldChangeScene())
	{
		m_hard_mode = m_main_menu.result["diff"];
		m_full_acii = m_main_menu.result["ascii"];
		ChangeScene(m_game_scene);

		if (m_current_scene->scene_name == m_game_scene.scene_name)
		{
			m_game_scene.m_hard_mode = m_hard_mode;
		}
	}
}

void Game::ChangeScene(Scene &new_scene)
{
	m_current_scene = &new_scene;
}