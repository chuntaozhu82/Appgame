#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <functional>
#include <chrono>

namespace Appgame {

class GameLoop {
public:
    enum class TimeStepMode {
        FIXED,     // 固定时间步长
        VARIABLE   // 可变时间步长
    };

    struct Stats {
        float fps;              // 当前帧率
        float avgFrameTime;     // 平均帧时间（毫秒）
        float maxFrameTime;     // 最大帧时间（毫秒）
    };

    GameLoop();
    ~GameLoop();

    // 启动游戏循环
    void start();

    // 停止游戏循环
    void stop();

    // 暂停游戏循环
    void pause();

    // 恢复游戏循环
    void resume();

    // 设置时间步长模式
    void setTimeStepMode(TimeStepMode mode);

    // 设置目标帧率
    void setTargetFPS(int fps);

    // 设置固定时间步长（秒）
    void setFixedTimeStep(float step);

    // 获取当前统计信息
    const Stats& getStats() const;

    // 注册回调函数
    void setUpdateCallback(std::function<void(float)> callback);
    void setRenderCallback(std::function<void(float)> callback);

private:
    // 游戏循环主函数
    void run();

    // 计算帧率和性能统计
    void updateStats();

    // 回调函数
    std::function<void(float)> m_updateCallback;
    std::function<void(float)> m_renderCallback;

    // 时间步长设置
    TimeStepMode m_timeStepMode;
    float m_fixedTimeStep;
    float m_targetFrameTime;

    // 游戏状态
    bool m_running;
    bool m_paused;

    // 时间变量
    std::chrono::steady_clock::time_point m_lastTime;
    std::chrono::steady_clock::time_point m_currentTime;
    float m_accumulator;

    // 性能统计
    Stats m_stats;
    int m_frameCount;
    float m_frameTimeSum;
    float m_lastFrameTime;
};

} // namespace Appgame

#endif // GAMELOOP_H
