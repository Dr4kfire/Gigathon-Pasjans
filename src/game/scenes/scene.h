#pragma once

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Process() = 0;
    virtual void Draw() = 0;
};