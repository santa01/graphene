#include <Engine.h>
#include <windows.h>
#include <cstdlib>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    Graphene::Engine engine(800, 600);
    engine.run();

    return EXIT_SUCCESS;
}
