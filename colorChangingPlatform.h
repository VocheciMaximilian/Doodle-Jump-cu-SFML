#ifndef COLORCHANGINGPLATFORM_H
#define COLORCHANGINGPLATFORM_H

#include "Platform.h"
#include <cstdlib> // for rand()

class ColorChangingPlatform : public Platform {
public:
    ColorChangingPlatform(float platformX, float platformY)
        : Platform(platformX, platformY)
    {
        shape.setFillColor(sf::Color(100, 100, 255));
    }

    void applyEffect(Player &player) override {
        sf::Color randomColor(
            rand() % 256,
            rand() % 256,
            rand() % 256
        );
        player.changeColor(randomColor);
    }
};

#endif
