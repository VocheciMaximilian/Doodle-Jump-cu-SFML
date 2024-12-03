#ifndef BREAKABLEPLATFORM_H
#define BREAKABLEPLATFORM_H

#include "Platform.h"

template<typename T>
class BreakablePlatform : public Platform {
private:
    T durability;

public:
    BreakablePlatform(float platformX, float platformY, T initialDurability)
        : Platform(platformX, platformY),
          durability(initialDurability)
    {
        shape.setFillColor(sf::Color(220, 50, 50));
    }

    bool isBreakable() const override { return true; }

    bool handleLanding() override {
        durability = durability - 1;
        return durability <= 0;
    }

    T getDurability() const {
        return durability;
    }
};

#endif
