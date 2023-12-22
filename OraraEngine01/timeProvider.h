#pragma once
#include <chrono>

class ElapsedTimeTracker {
public: 
    // コンストラクタ 
    ElapsedTimeTracker() {
        Reset();
    }

    // 現在の時間を取得 
    void Reset() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    // 経過時間を秒単位で取得 
    double GetElapsedTimeInSeconds() const {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = currentTime - startTime;
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

private:
    std::chrono::high_resolution_clock::time_point startTime;
};
