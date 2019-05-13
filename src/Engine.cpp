#include <Engine.h>
#include <OpenGL.h>

namespace Graphene {

Engine::Engine(int width, int height):
        window(width, height) {
    this->window.createContext();
    this->window.captureMouse(true);
}

Engine::~Engine() {
    this->window.destroyContext();
}

void Engine::run() {
    while (this->window.dispatchEvents()) {}
}

}  // namespace Graphene
