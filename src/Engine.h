#pragma once

#include <Window.h>

namespace Graphene {

class Engine {
public:
    Engine(int width, int height, HINSTANCE instance);
    ~Engine();

    void run();

private:
    Window window;
};

}  // namespace Graphene
