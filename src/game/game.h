#pragma once


#include "scenes/main_menu.h"


class Game
{
public:
    Game() = default;
    ~Game() = default;

    void Draw();
    void Process();

    void ChangeScene();

private:
    MainMenu main_menu;
};

