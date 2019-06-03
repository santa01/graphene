#include <Engine.h>
#include <Signals.h>

namespace Graphene {

Engine::Engine(int width, int height):
        window(width, height) {
    this->window.onQuit.connect(Signals::Slot<>(&Engine::onQuit, this));
}

void Engine::exit(int result) {
    this->result = result;
    this->running = false;
}

int Engine::exec() {
    while (this->running) {
        this->window.dispatchEvents();
    }

    return this->result;
}

void Engine::onQuit() {
    this->exit(0);
}

}  // namespace Graphene
