#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <iostream>
#include <cstdio>
#include "Window.h"
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    auto window = Window("DASH", 1280, 960);

    window.Run(RAYWHITE);

    return 0;
}
