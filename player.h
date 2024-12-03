#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
private:
    static const int width = 600;
    static const int height = 800;

    float posX, posY, width_, height_, velocityY, velocityX;
    int score;
    sf::CircleShape shape;

public:
    Player()
        : posX(width / 2), posY(height - 70), width_(40), height_(40),
          velocityY(0), velocityX(0), score(0)
    {
        shape.setRadius(width_ / 2);
        shape.setFillColor(sf::Color(220, 50, 100));
        shape.setPosition(posX - width_ / 2, posY - height_ / 2);
    }

    void reset() {
        posX = width / 2;
        posY = height - 70;
        velocityY = 0;
        velocityX = 0;
        score = 0;
        shape.setFillColor(sf::Color(220, 50, 100));
    }

    void show(sf::RenderWindow &window) {
        shape.setPosition(posX - width_ / 2, posY - height_ / 2);
        window.draw(shape);
    }

    bool lands(const sf::RectangleShape &platform) {
        if (velocityY > 0) {
            float platformLeft   = platform.getPosition().x - platform.getSize().x / 2;
            float platformRight  = platform.getPosition().x + platform.getSize().x / 2;
            float platformTop    = platform.getPosition().y - platform.getSize().y / 2;
            float platformBottom = platform.getPosition().y + platform.getSize().y / 2;

            if (posX + width_ / 4 >= platformLeft && posX - width_ / 4 <= platformRight) {
                if (posY + height_ / 2 >= platformTop && posY + height_ / 2 <= platformBottom) {
                    return true;
                }
            }
        }
        return false;
    }

    void changeColor(sf::Color newColor) {
        shape.setFillColor(newColor);
    }

    void jump() {
        velocityY = -500;
    }

    void move(float deltaTime) {
        velocityY += 800 * deltaTime;
        posY += velocityY * deltaTime;
        posX += velocityX * deltaTime;

        // Horizontal wrap
        if (posX > width)      posX = 0;
        else if (posX < 0)     posX = width;
    }

    float getX() const { return posX; }
    float getY() const { return posY; }
    float getVelocityX() const { return velocityX; }
    float getVelocityY() const { return velocityY; }
    int getScore() const { return score; }

    void setVelocityX(float value) { velocityX = value; }
    void incrementScore() { score++; }

    static int getWidth() { return width; }
    static int getHeight() { return height; }
};

#endif
