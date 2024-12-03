#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include "Player.h"
#include "Platform.h"
#include "PlatformFactory.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    Player player;
    std::vector<Platform*> platforms;
    sf::Font font;
    std::vector<int> topScores;
    int lastScore;

    void loadScores() {
        std::ifstream file("scores.txt");
        if (file.is_open()) {
            topScores.clear();
            int score;
            for (int i = 0; i < 3 && file >> score; ++i) {
                topScores.push_back(score);
            }
            if (file >> score) {
                lastScore = score;
            } else {
                lastScore = 0;
            }
            std::sort(topScores.rbegin(), topScores.rend());
            while (topScores.size() < 3) {
                topScores.push_back(0);
            }
            file.close();
        } else {
            topScores = {0, 0, 0};
            lastScore = 0;
        }
    }

    void saveScores() {
        std::ofstream file("scores.txt");
        if (file.is_open()) {
            for (auto sc : topScores) {
                file << sc << std::endl;
            }
            file << lastScore << std::endl;
            file.close();
        }
    }

    void showMenu() {
        while (window.isOpen()) {
            window.clear(sf::Color(30, 30, 30));

            sf::Text title("DOODLE JUMP", font, 50);
            title.setFillColor(sf::Color::White);
            title.setPosition((Player::getWidth() - title.getLocalBounds().width) / 2,
                              Player::getHeight() / 4);
            window.draw(title);

            sf::Text play("Press Enter to Play", font, 30);
            play.setFillColor(sf::Color::White);
            play.setPosition((Player::getWidth() - play.getLocalBounds().width) / 2,
                             Player::getHeight() / 2);
            window.draw(play);

            std::string topScoresText = "Top Scores:\n";
            for (size_t i = 0; i < topScores.size(); ++i) {
                topScoresText += std::to_string(i + 1) + ". " + std::to_string(topScores[i]) + "\n";
            }
            sf::Text scores(topScoresText, font, 30);
            scores.setFillColor(sf::Color::White);
            scores.setPosition(10, Player::getHeight() / 2 + 50);
            window.draw(scores);

            sf::Text last("Last Score: " + std::to_string(lastScore), font, 30);
            last.setFillColor(sf::Color::White);
            last.setPosition(10, Player::getHeight() - 100);
            window.draw(last);

            window.display();

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    return;
                }
            }
        }
    }

    void playGame() {
        player.reset();
        initializePlatforms();
        bool running = true;
        clock.restart();
        try {
            while (window.isOpen() && running) {
                float deltaTime = clock.restart().asSeconds();
                handleEvents(running);
                update(deltaTime);
                render();
            }
        } catch (const std::runtime_error &e) {
            lastScore = player.getScore();
            topScores.push_back(lastScore);
            std::sort(topScores.rbegin(), topScores.rend());
            topScores.erase(std::unique(topScores.begin(), topScores.end()), topScores.end());
            if (topScores.size() > 3) {
                topScores.pop_back();
            }
        }
    }

    void initializePlatforms() {
        clearPlatforms();

        float currentY = Player::getHeight() - 30;
        float platformSpacing = 120.f;

        for (int i = 0; i < 5; ++i) {
            float randomX = static_cast<float>(
                RandomGenerator::getInstance().getInt(75, Player::getWidth() - 75)
            );

            if (i == 0) {
                platforms.push_back(
                    PlatformFactory::createPlatform(
                        PlatformType::Normal,
                        Player::getWidth() / 2,
                        currentY
                    )
                );
            } else if (i % 2 == 0) {
                platforms.push_back(
                    PlatformFactory::createPlatform(
                        PlatformType::Breakable,
                        randomX,
                        currentY - i * platformSpacing
                    )
                );
            } else {
                platforms.push_back(
                    PlatformFactory::createPlatform(
                        PlatformType::ColorChanging,
                        randomX,
                        currentY - i * platformSpacing
                    )
                );
            }
        }
    }

    void clearPlatforms() {
        for (auto p : platforms) {
            delete p;
        }
        platforms.clear();
    }

    void handleEvents(bool &running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    player.setVelocityX(-300.f);
                } else if (event.key.code == sf::Keyboard::Right) {
                    player.setVelocityX(300.f);
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    running = false;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left ||
                    event.key.code == sf::Keyboard::Right) {
                    player.setVelocityX(0.f);
                }
            }
        }
    }

    void update(float deltaTime) {
        player.move(deltaTime);

        for (size_t i = 0; i < platforms.size(); ++i) {
            if (player.lands(platforms[i]->getShape())) {
                player.jump();

                if (platforms[i]->isBreakable()) {
                    if (platforms[i]->handleLanding()) {
                        delete platforms[i];
                        platforms.erase(platforms.begin() + i);
                        --i;
                    }
                } else {
                    platforms[i]->applyEffect(player);
                }
                break;
            }
        }

        // If player is above halfway, scroll platforms
        if (player.getY() < Player::getHeight() / 2 && player.getVelocityY() < 0) {
            for (auto &p : platforms) {
                p->move(-player.getVelocityY() * deltaTime);
                if (p->getY() > Player::getHeight()) {
                    float maxY = static_cast<float>(Player::getHeight());
                    for (auto &pp : platforms) {
                        maxY = std::min(maxY, pp->getY());
                    }
                    p->reposition(maxY);
                    player.incrementScore();
                }
            }
        }

        // Keep at least 7 platforms
        while (platforms.size() < 7) {
            float randomX = static_cast<float>(
                RandomGenerator::getInstance().getInt(75, Player::getWidth() - 75)
            );
            float minY = static_cast<float>(Player::getHeight());
            for (auto &p : platforms) {
                minY = std::min(minY, p->getY());
            }
            platforms.push_back(
                PlatformFactory::createPlatform(
                    PlatformType::Normal,
                    randomX,
                    minY - 120
                )
            );
        }

        // Game Over
        if (player.getY() - 40 / 2 > Player::getHeight()) {
            throw std::runtime_error("Game Over");
        }
    }

    void render() {
        window.clear();

        player.show(window);
        for (auto &p : platforms) {
            p->show(window);
        }

        sf::Text scoreText("Score: " + std::to_string(player.getScore()), font, 30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);

        window.display();
    }

public:
    Game()
      : window(sf::VideoMode(Player::getWidth(), Player::getHeight()), "Doodle Jump Clone"),
        lastScore(0)
    {
        window.setFramerateLimit(60);
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }
        initializePlatforms();
        loadScores();
    }

    ~Game() {
        clearPlatforms();
        saveScores();
    }

    void run() {
        while (window.isOpen()) {
            showMenu();
            playGame();
        }
    }
};

#endif
