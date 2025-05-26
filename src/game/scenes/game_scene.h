#pragma once

#include "scene.h"

class GameScene : public Scene
{
public:
	GameScene(const std::string &scene_name) : Scene(scene_name) {}

	void Draw() override;
	void Process(const int &input) override;

private:
	bool m_hard_mode = false;
};