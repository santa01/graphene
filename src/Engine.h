#pragma once

#include <Window.h>

namespace Graphene {

class Engine {
public:
    Engine(int width, int height);

    void exit(int result);
    int exec();

private:
    void onQuit();

    Window window;

    bool running = true;
    int result = 0;
};

}  // namespace Graphene
