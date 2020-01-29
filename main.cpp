#include <iostream>
#include <cstdio>
#include "Window.h"

int main()
{
    auto window = Window("DEMO", 1280, 960);

    window.Run(RAYWHITE);

    return 0;
}
