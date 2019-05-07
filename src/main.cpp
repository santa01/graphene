#include "stdafx.h"
#include "Engine.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    Engine engine(800, 600, hInstance);
    return engine.run();
}
