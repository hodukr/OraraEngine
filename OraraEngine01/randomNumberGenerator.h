#pragma once
#include <random>

class RandomNumberGenerator {
public:
    // コンストラクタ 
    RandomNumberGenerator() : generator(rd()) {}

    // 指定範囲内の整数乱数を生成 
    int GenerateInt(int min, int max) {
        uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    // 指定範囲内の浮動小数点数乱数を生成 
    float GenerateFloat(float min, float max) {
        uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

private:
    random_device rd;
    mt19937 generator;
};

