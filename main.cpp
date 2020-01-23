#include <iostream>
#include <cstdio>
#include "Window.h"

int main()
{
    auto window = Window("DEMO", 1024, 768);

    window.Run(RAYWHITE);

    return 0;
}
