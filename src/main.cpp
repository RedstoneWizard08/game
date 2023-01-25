#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "app.hpp"

using namespace std;

int main() {
    printf("Initializing game...");

    Game game;

    printf("Starting loop...");

    game.Run();

    return 0;
}
