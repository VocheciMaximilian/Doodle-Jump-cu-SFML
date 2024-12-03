#ifndef SCORE_STRATEGY_H
#define SCORE_STRATEGY_H

class ScoreStrategy {
public:
    virtual int calculateScore(int baseScore) const = 0;
    virtual ~ScoreStrategy() {}
};

class NormalScoreStrategy : public ScoreStrategy {
public:
    int calculateScore(int baseScore) const override { return baseScore; }
};

class DoubleScoreStrategy : public ScoreStrategy {
public:
    int calculateScore(int baseScore) const override { return baseScore * 2; }
};

#endif
