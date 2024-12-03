#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "RandomGenerator.h"
#include "Utils.h"

class Platform {
protected:
    float posX, posY, width_, height_;
    sf::RectangleShape shape;

public:
    Platform(float platformX, float platformY)
        : posX(platformX), posY(platformY), width_(100), height_(20)
    {
        shape.setSize(sf::Vector2f(width_, height_));
        shape.setFillColor(sf::Color(50, 220, 100));
        shape.setOrigin(width_ / 2, height_ / 2);
        shape.setPosition(posX, posY);
    }

    virtual ~Platform() {}

    virtual void show(sf::RenderWindow &window) {
        shape.setPosition(posX, posY);
        window.draw(shape);
    }

    virtual bool isBreakable() const { return false; }

    virtual bool handleLanding() { return false; }

    virtual void applyEffect(Player &) {}

    void move(float deltaY) {
        posY += deltaY;
        shape.setPosition(posX, posY);
    }

    void reposition(float minY) {
        posX = static_cast<float>(
            RandomGenerator::getInstance().getInt(75, Player::getWidth() - 75)
        );
        posX = clampValue(posX, 75.f, static_cast<float>(Player::getWidth() - 75));
        posY = minY - 120;

        shape.setSize(sf::Vector2f(100, 20));
        shape.setPosition(posX, posY);
    }

    const sf::RectangleShape &getShape() const { return shape; }
    float getY() const { return posY; }
};

#endif
