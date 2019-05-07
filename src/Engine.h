#pragma once

#include "Window.h"

class Engine
{
public:
    Engine(int width, int height, HINSTANCE instance);
    int run();

private:
    Window mWindow;
};
