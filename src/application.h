#pragma once

#include "game/game.h"

class Application
{
public:
	Application();
	~Application();

	void Process();
	void Draw();

	void Close();
	bool IsRunning();

private:
	bool m_is_running = true;
	
	Game game;
};