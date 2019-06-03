#include <Engine.h>

namespace Graphene {

Engine::Engine(int width, int height):
        window(width, height) {
}

void Engine::run() {
    while (true) {
        this->window.dispatchEvents();
    }
}

}  // namespace Graphene
