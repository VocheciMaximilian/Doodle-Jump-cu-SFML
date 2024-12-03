#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

class RandomGenerator {
private:
    std::mt19937 rng;
    RandomGenerator() {
        std::random_device rd;
        rng.seed(rd());
    }

public:
    static RandomGenerator& getInstance() {
        static RandomGenerator instance;
        return instance;
    }

    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    int getInt(int minVal, int maxVal) {
        std::uniform_int_distribution<int> dist(minVal, maxVal);
        return dist(rng);
    }
};

#endif
