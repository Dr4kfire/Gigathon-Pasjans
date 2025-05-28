#include "scene.h"


class MainMenu : public Scene
{
public:
    MainMenu(const std::string& scene_name) : Scene(scene_name) {}

    void Draw() override;
    void Process(const int& input) override;

private:
    enum Options {
        START,
        DIFFICULTY,
        SETTINGS,
    };
    Options m_selected_option;
    int option_idx = 0;
    
    bool m_hard_mode = false;
    bool m_full_ascii = false;
};