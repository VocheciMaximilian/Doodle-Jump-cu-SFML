#include "Game.h"
#include <iostream>
using namespace std;

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const std::runtime_error &e) {
        cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
