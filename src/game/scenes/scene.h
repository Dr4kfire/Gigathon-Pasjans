#pragma once

#include <map>
#include <string>

class Scene
{
public:
	std::map<std::string, int> result;
	std::string scene_name;

public:
	Scene(const std::string &scene_name) : scene_name(scene_name) {}
	virtual ~Scene() = default;

	virtual void Draw() {}
	virtual void Process(const int &input) {}

	bool ShouldChangeScene() { return m_should_change_scene; }
	void SetChangeScene(bool new_value) { m_should_change_scene = new_value; }

private:
	bool m_should_change_scene = false;
};